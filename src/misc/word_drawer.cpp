#include <word_drawer.h>

void misc::WordDrawer::extract_word()
{
	long long pos = mRandomDistribution(mRandomGenerator);
	mDataFile.seekg(pos, std::ios_base::_Seekbeg);
	while (mDataFile.peek() != '\n')
		mDataFile.seekg(-1, std::ios_base::_Seekcur);
	mDataFile.seekg(1, std::ios_base::_Seekcur);
	std::string temp_str;
	std::getline(mDataFile, temp_str);
	if (temp_str.size() > 2)
	{
		temp_str[temp_str.size() - 1] = '\0';
		mWordQueue.enqueue(temp_str);
	}
	else
		throw std::ifstream::failure("Unexpectedly couldn't read a string from file?");
}


void misc::WordDrawer::extract_word_chunk()
{
	for (int i = 0; i < mWordChunkSize; i++)
		extract_word();
}
	


misc::WordDrawer::WordDrawer(std::string fname, const sf::Font& font, int word_chunk_size) :
			mDataFile(fname, std::ios::binary), mWordChunkSize(word_chunk_size),
			mRandomGenerator(mRandomDevice()), mFontRef(font), mCurState(ActionState::WAITING)
{
	mDataFile.seekg(0, std::ios_base::_Seekend);
	mDataFileSize = mDataFile.tellg();
	mRandomDistribution = std::uniform_int_distribution<long long>(1, mDataFileSize);
	extract_word_chunk();

	// Set up the status text
	mStatusTextObj.setFont(font);
	mStatusTextObj.setString("xyz");
	mStatusTextObj.setCharacterSize(24);
	mStatusTextObj.setFillColor(sf::Color::White);
	mStatusTextLines.append("Status: Left-click to spawn");
	mStatusTextLines.append("Undo Stack Size: 0");
	mStatusTextLines.append("Last Undo Item: ");
}

		
misc::WordDrawer::~WordDrawer()
{
	mDataFile.close();
}

bool misc::WordDrawer::handle_event(const sf::Event& evt)
{
	if (evt.type == sf::Event::Closed)
		return false;
	else if (evt.type == sf::Event::KeyPressed && evt.key.control && evt.key.code == sf::Keyboard::Z && !mUndoStack.empty())
		undo();
	else if (evt.type == sf::Event::MouseButtonPressed)
	{
		// The user just clicked.  First see if they clicked over an existing circle
		int sel_index = find_selected(sf::Vector2f((float)evt.mouseButton.x, (float)evt.mouseButton.y));
		if (evt.mouseButton.button == sf::Mouse::Left)
		{
			// The user left-clicked
			if (sel_index == -1 && mCurState == ActionState::WAITING)
			{
				// The user did NOT left-click on an existing circle
				mActiveCircle = sf::TextCircle((float)evt.mouseButton.x, (float)evt.mouseButton.y, mFontRef, serve_word());
				update_status(ActionState::PLACING);

				// Add to the undo stack
				add_undo_item(ActionItem(ActionItemType::SPAWN, mActiveCircle.get_string(), mActiveCircle.get_position(), 0));
			}
			else if (mCurState == ActionState::PLACING)
			{
				// The user is placing down the circle they are currently moving
				mTextCircleList.append(mActiveCircle);
				update_status(ActionState::WAITING);

				// Add to the undo stack
				add_undo_item(ActionItem(ActionItemType::CREATE, mActiveCircle.get_string(), mActiveCircle.get_position(), mTextCircleList.size() - 1));
			}
			else if (sel_index >= 0 && mCurState == ActionState::WAITING)
			{
				// The user has clicked on an existing circle to start moving it / deleting it
				mSelectedCircleIndex = sel_index;
				sf::Vector2f mpos((float)evt.mouseButton.x, (float)evt.mouseButton.y);
				sf::Vector2f cpos = mTextCircleList[mSelectedCircleIndex].get_position();
				mSelectedCircleOffset = sf::Vector2f(cpos.x - mpos.x, cpos.y - mpos.y);
				update_status(ActionState::MOVING);

				// Add to the undo stack
				add_undo_item(ActionItem(ActionItemType::MOVE, mTextCircleList[sel_index].get_string(), mTextCircleList[sel_index].get_position(), 0));
			}
		}
		else if (evt.mouseButton.button == sf::Mouse::Right)
		{
			// The User right-clicked
			if (mCurState == ActionState::MOVING)
			{
				// Add to the undo stack
				add_undo_item(ActionItem(ActionItemType::DESTROY_EXISTING, mTextCircleList[mSelectedCircleIndex].get_string(),
					mTextCircleList[mSelectedCircleIndex].get_position(), mSelectedCircleIndex));

				// We're in the process of moving an existing circle.  Remove it
				mTextCircleList.remove(mSelectedCircleIndex);
			}
			else
			{
				// The user is destroying the temporary TextCircle
				add_undo_item(ActionItem(ActionItemType::DESTROY_NEW, mActiveCircle.get_string(), mActiveCircle.get_position(), 0));
			}
			update_status(ActionState::WAITING);
		}
	}
	else if (evt.type == sf::Event::MouseButtonReleased)
	{
		// User is releasing a mouse button
		if (evt.mouseButton.button == sf::Mouse::Left && mCurState == ActionState::MOVING)
		{
			update_status(ActionState::WAITING);
		}
		
	}
	else if (evt.type == sf::Event::MouseMoved)
	{
		sf::Vector2f mpos((float)evt.mouseMove.x, (float)evt.mouseMove.y);
		if (mCurState == ActionState::PLACING)
			mActiveCircle.set_position(mpos);
		else if (mCurState == ActionState::MOVING)
			mTextCircleList[mSelectedCircleIndex].set_position(sf::Vector2f(mpos.x + mSelectedCircleOffset.x,
				mpos.y + mSelectedCircleOffset.y));
	}


	return true;
}

		
void misc::WordDrawer::draw(sf::RenderWindow& rw) 
{
	// Draw from first to last
	ssuds::ArrayList<sf::TextCircle>::ArrayListIterator it = mTextCircleList.begin();
	while (it != mTextCircleList.end())
	{
		rw.draw(*it);
		++it;
	}

	if (mCurState == ActionState::PLACING)
		rw.draw(mActiveCircle);

	float y = 0.0f;
	mStatusTextObj.setPosition(sf::Vector2f(0.0f, y));
	for (unsigned int i = 0; i < mStatusTextLines.size(); i++)
	{
		mStatusTextObj.setString(mStatusTextLines[i]);
		y += mStatusTextObj.getLocalBounds().height + 3;
		mStatusTextObj.setPosition(sf::Vector2f(0.0f, y));
		rw.draw(mStatusTextObj);
	}
}


std::string misc::WordDrawer::serve_word()
{
	if (mWordQueue.empty())
		extract_word_chunk();
	return mWordQueue.dequeue();
}


misc::WordDrawer::ActionItem::ActionItem(ActionItemType tp, std::string w, sf::Vector2f op, unsigned int wp) : mType(tp),
mWord(w), mOldPos(op), mWordPosition(wp)
{
	// empty, on purpose
}

std::string misc::WordDrawer::ActionItem::summary_string() const
{
	switch (mType)
	{
	case ActionItemType::SPAWN:
		return "Spawned new word '" + mWord + "' at position " + v2s(mOldPos);
	case ActionItemType::CREATE:
		return "New String '" + mWord + "' created at position " + v2s(mOldPos);
	case ActionItemType::DESTROY_NEW:
		return "Destroyed temporary word '" + mWord + "' which was at " + v2s(mOldPos);
	case ActionItemType::DESTROY_EXISTING:
		return "Destroyed created word '" + mWord + "' index=" + std::to_string(mWordPosition) + " and position=" + v2s(mOldPos);
	case ActionItemType::MOVE:
		return "Moved word '" + mWord + "' from position " + v2s(mOldPos);
	}
	return "";
}

void misc::WordDrawer::add_undo_item(const ActionItem & ai)
{
	mUndoStack.push(ai);
	mStatusTextLines[1] = "Undo stack size: " + std::to_string(mUndoStack.size());
	mStatusTextLines[2] = "<Last undo added: " + ai.summary_string() + ">";
}

void misc::WordDrawer::undo()
{
	if (!mUndoStack.empty())
	{
		ActionItem item = mUndoStack.pop();

		switch (item.mType)
		{
		case ActionItemType::SPAWN:
			update_status(ActionState::WAITING);
			break;
		case ActionItemType::CREATE:
			mActiveCircle = mTextCircleList.remove(item.mWordPosition);
			update_status(ActionState::PLACING);
			break;
		case ActionItemType::DESTROY_NEW:
			mActiveCircle = sf::TextCircle(item.mOldPos.x, item.mOldPos.y, mFontRef, item.mWord);
			update_status(ActionState::PLACING);
			break;
		case ActionItemType::DESTROY_EXISTING:
			mTextCircleList.insert(sf::TextCircle(item.mOldPos.x, item.mOldPos.y, mFontRef, item.mWord), item.mWordPosition);
			break;
		case ActionItemType::MOVE:
			mTextCircleList[item.mWordPosition].set_position(item.mOldPos);
			break;
		}
	}

	mStatusTextLines[1] = "Undo stack size: " + std::to_string(mUndoStack.size());
}


std::string misc::WordDrawer::ActionItem::v2s(const sf::Vector2f& v) const
{
	return "(" + std::to_string((int)v.x)+ "," + std::to_string((int)v.y) + ")";
}

	
int misc::WordDrawer::find_selected(sf::Vector2f v) const
{
	for (int i = mTextCircleList.size() - 1; i >= 0; i--)
	{
		if (mTextCircleList[i].point_inside(v))
			return i;
	}
	return -1;
}

	

void misc::WordDrawer::update_status(const ActionState& new_state)
{
	mCurState = new_state;
	switch (new_state)
	{
	case ActionState::WAITING:
		if (mTextCircleList.size() > 0)
			mStatusTextLines[0] = "Left click on empty area to spawn, Left-drag to move existing";
		else
			mStatusTextLines[0] = "Left click to spawn";
		break;
	case ActionState::PLACING:
		mStatusTextLines[0] = "Left-click to place, Right-click to discard";
		break;
	case ActionState::MOVING:
		mStatusTextLines[0] = "Left-click to finalize, Right-click to delete";
		break;
	}
}