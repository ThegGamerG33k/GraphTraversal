#include <text_circle.h>
#include <SFML/Graphics.hpp>


sf::TextCircle::TextCircle() : mHMode(TextCircleHighlightMode::NONE)
{
	// Doesn't initialize anything.
}

sf::TextCircle::TextCircle(float x, float y, const Font& font_ref, std::string text) : mHMode(TextCircleHighlightMode::NONE)
{
	// Set properties except position of the text object
	mText.setFont(font_ref);
	mText.setString(text);
	mText.setCharacterSize(24);
	mText.setFillColor(sf::Color::White);

	
	//mCircle.setFillColor(sf::Color(rand() % 128 + 127, rand() % 128 + 127, rand() % 128 + 127, 255));
	mCircle.setFillColor(sf::Color::Red);
	//mCircle.setPosition(sf::Vector2f(x, y));

	set_string(text);
	mText.setPosition(sf::Vector2f(x, y));
	float radius = mCircle.getRadius();
	mCircle.setPosition(sf::Vector2f(x - radius, y - radius));

	//mCircle.setOutlineColor(sf::Color::Cyan);
	//mCircle.setOutlineThickness(2.0f);
}



void sf::TextCircle::draw(RenderTarget& target, RenderStates states) const
{
	target.draw(mCircle);
	target.draw(mText);
}

void sf::TextCircle::set_circle_color(const sf::Color& c)
{
	mCircle.setFillColor(c);

}

sf::Vector2f sf::TextCircle::get_position() const
{
	return mText.getPosition();
}


std::string sf::TextCircle::get_string() const
{
	return mText.getString();
}

sf::Color sf::TextCircle::get_color() const
{
	return mCircle.getFillColor();
}

void sf::TextCircle::set_position(const sf::Vector2f& p)
{
	float radius = mCircle.getRadius();
	mCircle.setPosition(p.x - radius, p.y - radius);
	mText.setPosition(p.x, p.y);
}

void sf::TextCircle::set_string(const std::string& s)
{
	mText.setString(s);

	// Center the text
	// reference: https://stackoverflow.com/questions/14505571/centering-text-on-the-screen-with-sfml
	mTextRect = mText.getLocalBounds();
	mText.setOrigin(mTextRect.left + mTextRect.width / 2.0f, mTextRect.top + mTextRect.height / 2.0f);

	// Set the position / radius of the circle
	float radius = (mTextRect.width > mTextRect.height ? mTextRect.width : mTextRect.height) / 2.0f + 3.0f;
	mCircle.setRadius(radius);
	sf::Vector2f cur_pos = mText.getPosition();
	mCircle.setPosition(cur_pos.x - radius, cur_pos.y - radius);
}

bool sf::TextCircle::point_inside(sf::Vector2f v) const
{
	sf::Vector2f p = get_position();
	float dist = powf(powf(v.x - p.x, 2.0f) + powf(v.y - p.y, 2.0f), 0.5f);
	return dist <= mCircle.getRadius();
}

void sf::TextCircle::set_highlight(const TextCircleHighlightMode& m)
{
	mHMode = m;
	mCircle.setOutlineThickness(2.0f);
	switch (mHMode)
	{
	case TextCircleHighlightMode::NONE:
		mCircle.setOutlineColor(sf::Color::Black);
		mCircle.setOutlineThickness(0.0f);
		break;
	case TextCircleHighlightMode::RED:
		mCircle.setOutlineColor(sf::Color::Red);
		break;
	case TextCircleHighlightMode::GREEN:
		mCircle.setOutlineColor(sf::Color::Green);
		break;
	case TextCircleHighlightMode::BLUE:
		mCircle.setOutlineColor(sf::Color::Blue);
		break;
	case TextCircleHighlightMode::WHITE:
		mCircle.setOutlineColor(sf::Color::White);
		break;
	}
}
