#pragma once
#include <SFML/Graphics.hpp>
#include <queue.h>
#include <stack.h>
#include <fstream>
#include <string>
#include <random>
#include <text_circle.h>
#include <array_list.h>

namespace misc
{
	class WordDrawer
	{
	protected:
		/// <summary>
		/// Which of the 3 states this object is in.
		/// MOVING: User is moving an existing circle. 
		/// PLACING: The user is in the process of placing a circle they just spawned
		/// WAITING: Waiting for the user to click on an empty area or an existing circle
		/// </summary>
		enum ActionState { MOVING, PLACING, WAITING };

		/// <summary>
		/// The data file connected to the word list.  We keep this open the duration of this objects existence
		/// </summary>
		std::ifstream mDataFile;

		/// <summary>
		/// The active queue of words to pull from
		/// </summary>
		ssuds::Queue<std::string> mWordQueue;

		/// <summary>
		/// The size, in bytes, of the word data file
		/// </summary>
		unsigned long long mDataFileSize;

		/// <summary>
		/// When we load a batch of words to the queue, load this many
		/// </summary>
		int mWordChunkSize;

		/// <summary>
		/// The RNG device (for picking spots in the word file)
		/// </summary>
		std::random_device mRandomDevice;
		
		/// <summary>
		/// The RNG algorithm object
		/// </summary>
		std::mt19937 mRandomGenerator;

		/// <summary>
		/// The RNG distribution
		/// </summary>
		std::uniform_int_distribution<long long> mRandomDistribution;

		/// <summary>
		/// The font object to use for circle text's
		/// </summary>
		const sf::Font& mFontRef;

		/// <summary>
		/// The main list of all placed circles
		/// </summary>
		ssuds::ArrayList<sf::TextCircle> mTextCircleList;

		/// <summary>
		/// Current State of our system.  Transitions are as follows:
		/// WAITING (default)
		/// =================
		///    => PLACING if the user left-clicks not on any existing circle   
		///    => MOVING if the user left-clicks on an existing circle         
		/// 
		/// MOVING
		/// ======
		///    => WAITING if user right clicks (delete current item)           
		///    => WAITING if user left-clicks (to place it)                    
		/// 
		/// PLACING
		/// =======
		///    => WAITING if user left-clicks to place                         
		/// </summary>
		ActionState mCurState;

		/// <summary>
		/// If we're placing a circle, this is the one that we might add to mTextCircleList if we left-click
		/// </summary>
		sf::TextCircle mActiveCircle;

		/// <summary>
		/// If we've clicked and are moving an existing circle, this is its index in mTextCircleList
		/// </summary>
		int mSelectedCircleIndex;

		/// <summary>
		/// If we've clicked and are moving an existing circle, this is a Vector that indicates the offset 
		/// from the mouse to the center of the moving circle.
		/// </summary>
		sf::Vector2f mSelectedCircleOffset;

		/// <summary>
		/// Some lines of status text to show
		/// </summary>
		ssuds::ArrayList<std::string> mStatusTextLines;

		/// <summary>
		/// A text object to use for showing mStatusTextLines
		/// </summary>
		sf::Text mStatusTextObj;

		/// <summary>
		/// An internal method to extract one single word from the file and store in the mWordQueue
		/// </summary>
		void extract_word();

		/// <summary>
		/// An internal method to collect, from the opened file, a batch of words to store in the mWordQueue.
		/// </summary>
		void extract_word_chunk();

		/// <summary>
		/// The different types of actions we can undo
		/// </summary>
		enum class ActionItemType {SPAWN, CREATE, DESTROY_NEW, DESTROY_EXISTING, MOVE};

		/// <summary>
		/// A simple class to keep track of one item on the undo stack that can be undone.
		/// </summary>
		class ActionItem
		{
		public:
			/// <summary>
			/// The type of action this was
			/// </summary>
			ActionItemType mType;

			/// <summary>
			/// The word involved in the action
			/// </summary>
			std::string mWord;

			/// <summary>
			/// For a move operation or a spawn the last / initial position of the word
			/// </summary>
			sf::Vector2f mOldPos;

			/// <summary>
			/// For a move-existing operation, the index in the mTextCircle ArrayList of this word
			/// </summary>
			unsigned int mWordPosition;

			/// <summary>
			/// Utility function to convert an sf::Vector2f to a string of the form (x, y)
			/// </summary>
			/// <param name="v">vector to convert to a string</param>
			/// <returns>string form of that vector</returns>
			std::string v2s(const sf::Vector2f& v) const;

			/// <summary>
			/// Simple constructor
			/// </summary>
			/// <param name="tp"></param>
			/// <param name="w"></param>
			/// <param name="op"></param>
			/// <param name="wp"></param>
			ActionItem(ActionItemType tp, std::string w, sf::Vector2f op, unsigned int wp);

			/// <summary>
			/// A summary string showing relevant info about this item
			/// </summary>
			/// <returns></returns>
			std::string summary_string() const;
		};

		/// <summary>
		/// Create an undo ActionItem to add to the undo stack and udpate the status strings
		/// </summary>
		/// <param name="ai"></param>
		void add_undo_item(const ActionItem& ai);

		/// <summary>
		/// Pops an item off the undo stack, if there is one, and undoes the operation
		/// </summary>
		void undo();
		
		/// <summary>
		/// The stack of undo ActionItems
		/// </summary>
		ssuds::Stack<ActionItem> mUndoStack;
	public:

		/// <summary>
		/// The constructor
		/// </summary>
		/// <param name="fname">the path and filename of the scowl-like word file to open</param>
		/// <param name="font">the font object to use for text circles</param>
		/// <param name="word_chunk_size">how many words to pull out from the file at one time</param>
		WordDrawer(std::string fname, const sf::Font& font, int word_chunk_size = 10);

		/// <summary>
		/// Destructor
		/// </summary>
		~WordDrawer();

		/// <summary>
		/// Handles SFML events to control the spawning and manipulation of TextCircle objects
		/// </summary>
		/// <param name="evt"></param>
		/// <returns></returns>
		bool handle_event(const sf::Event& evt);

		/// <summary>
		/// Draws the text circles, active circle (if any) and status text
		/// </summary>
		/// <param name="rw"></param>
		void draw(sf::RenderWindow& rw);


		/// <summary>
		/// Loads another chunk of words if necessary and gets one word from the mWordQueue
		/// </summary>
		/// <returns></returns>
		std::string serve_word();

	protected:
		/// <summary>
		/// Internal method to find the index of a placed TextCircle if the position is over it 
		/// </summary>
		/// <param name="v">The position (probably the mouse) to look for circles under</param>
		/// <returns>the index of the selected circle or -1 if none were under the point</returns>
		int find_selected(sf::Vector2f v) const;

		/// <summary>
		/// Changes mCurState and also updates the status text
		/// </summary>
		/// <param name="new_state"></param>
		void update_status(const ActionState& new_state);
	};
}
