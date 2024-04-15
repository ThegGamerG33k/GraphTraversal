#pragma once
#include <ostream>

namespace ssuds
{
	template <class T>
	/// A LinkedList is a collection of non-contiguous Nodes.  To get to
	/// a certain spot, you must traverse the list.
	class LinkedList
	{
	protected:
		// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ //
		// @ LINKED-LIST-Node CLASS                       @ //
		// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ // 
		/// This is a *hidden* class.  It is used to represent a single
		/// "link" in the list.  It contains the data (payload) and pointers
		/// to the next and previous Nodes.
		class Node
		{
		public:
			/// The actual data we're storing in each element of the list
			T mData;

			/// A pointer to the next Node in the list (or nullptr if there is none)
			Node * mNext;

			/// A pointer to the previous Node in the list (or nullptr if there is none)
			Node * mPrev;

			/// Constructor that initializes this Node, but sets both pointers to nullptr
			Node(T val) : mData(val), mNext(nullptr), mPrev(nullptr)
			{
				// Intentionally blank
			}

			/// Constructor that sets all the values of the Node
			Node(T val, Node * prev, Node * next) : mData(val), mNext(next), mPrev(prev)
			{
				// Intentionally blank
			}
		};      // END of Node class


	public:
		/// The iterator used to traverse our linked list
		class LinkedListIterator
		{
		protected:
			/// Generally, this isn't used much (the remove method does)
			const LinkedList* mParent;

			/// The Node we're currently at in our traversal
			Node* mCurrentNode;

			/// Are we going forward or backwards?
			bool mForward;

			/// The "position" of the iterator if this were an array (forward iterators start at 0, reverse at mSize-1)
			int mCurrentIndex;

		
			// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ //
			// @ LINKED-LIST-ITERATOR CLASS                   @ //
			// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ // 
		protected:
			/// Constructor that sets the current Node to either the beginning (if is_forward is true) or end (if not)
			/// Meant to be called internally
			LinkedListIterator(const LinkedList* parent, Node* start, int start_index, bool is_forward): mParent(parent), mCurrentNode(start), 
				mForward(is_forward), mCurrentIndex(start_index)
			{
				// intentionally empty
			}
		public:
			/// Constructor that's meant to be called internally that sets the current Node to NULL
			LinkedListIterator() : mParent(nullptr), mForward(true), mCurrentNode(nullptr), mCurrentIndex(-1)
			{
				// intentionally empty
			}


		

		public:
			/// Advances the iterator.  Note this goes in the direction of mForward (so if mForward is false, this
			/// actually goes to the previous Node in the list).
			LinkedListIterator operator++()
			{
				if (mForward)
				{
					mCurrentNode = mCurrentNode->mNext;
					mCurrentIndex++;
				}
				else
				{
					mCurrentNode = mCurrentNode->mPrev;
					mCurrentIndex--;
				}
				return *this;
			}

			/// Advances the iterator using post-fix notation (just in case you want to try it) -- it normally does just
			/// what the prefix++ operator does
			LinkedListIterator operator++(int dummy)
			{
				// The "dummy" parameter is just a "hack" -- it's so the compiler can distinguish between the two ++ operators
				// You never do anything with it

				// This is really the only funcional difference between this ++ and the other.  This one, returns the value of the
				// iterator before changing it.
				LinkedListIterator cur(mParent, mCurrentNode, mCurrentIndex, mForward);
				
				if (mForward)
				{
					mCurrentNode = mCurrentNode->mNext;
					if (mCurrentNode == nullptr)
						mCurrentIndex = -1;
					else
						mCurrentIndex++;
				}
				else
				{
					mCurrentNode = mCurrentNode->mPrev;
					if (mCurrentNode == nullptr)
						mCurrentIndex = -1;
					else
						mCurrentIndex--;
				}
				return cur;
			}


			/// Returns an iterator that's offset spots away from the current (possibly setting to the sentinel value)
			/// This value is relative to the direction (i.e. in a forward iterator -3 would move us 3 steps back)
			LinkedListIterator operator+(int offset) const
			{
				bool add = (mForward && offset > 0) || (!mForward && offset < 0);
				int amt = abs(offset);
				Node* cur = mCurrentNode;

				for (int i = 0; i < amt; i++)
				{
					if (add)
						cur = cur->mNext;
					else
						cur = cur->mPrev;

					if (cur == NULL)
						return LinkedListIterator(mParent, NULL, -1, true);
				}

				return LinkedListIterator(mParent, cur, mCurrentIndex + offset, mForward);
			}


			/// Is this iterator equal to the given one?
			bool operator==(const LinkedListIterator& it) const
			{
				// Note: No need to check that the list is the same -- each list has its
				// own Nodes, so we couldn't have the same Node* but a different list.
				return (mParent == it.mParent) && (mCurrentNode == it.mCurrentNode) && (mForward == it.mForward);
			}


			/// Is this iterator not equal to the given one?
			bool operator!=(const LinkedListIterator& it) const
			{
				return (mParent != it.mParent) || (mCurrentNode != it.mCurrentNode) || (mForward != it.mForward);
			}

			/// Gets a reference to the data held by the current Node.
			T& operator*() const
			{
				if (mCurrentNode)
					return mCurrentNode->mData;
				else
					throw std::out_of_range("can't dereference an \"end\" iterator");
			}

			/// Returns the "index" of this iterator's current value in the main list
			int index() const
			{
				return mCurrentIndex;
			}

			// Make the LinkedList a friend of this class so it can access our protected/private members
			friend class LinkedList;
		};	// END of LinkedListIterator class


	protected:
		// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ //
		// @ LINKED-LIST ATTRIBUTES                       @ //
		// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ // 
		/// This is the pointer to the first Node in the linked list (NULL if the list is empty)
		Node * mStart;

		/// This is the pointer to the last Node in the linked list (NULL if the list is empty)
		Node * mEnd;

		/// The number of elements in the list
		unsigned int mSize;


	public:
		/// The default constructor
		LinkedList() : mSize(0), mStart(NULL), mEnd(NULL)
		{
			// Intentionally blank
		}


		/// Copy constructor.  Makes a deep copy of the given LinkedList
		LinkedList(const LinkedList& L) : mStart(NULL), mEnd(NULL), mSize(0)
		{
			LinkedListIterator it = L.begin();
			while (it != L.end())
			{
				append(*it);
				++it;
			}
		}

		/// Initializer-list constructor
		LinkedList(const std::initializer_list<T>& initial_vals) : mStart(nullptr), mEnd(nullptr), mSize(0)
		{
			for (T val : initial_vals)
				append(val);
		}

		/// Move constructor (the noexcept "decorator" indicates we won't be throwing exceptions -- customary for move-constructors)
		LinkedList(LinkedList&& other) noexcept : mSize(other.mSize), mStart(other.mStart), mEnd(other.mEnd)
		{
			// Since we're "stealing" the data from the other one, we set their array pointers to
			// NULL (so when the destructor runs, it won't free up what is now our memory)
			other.mStart = other.mEnd = nullptr;
			other.mSize = 0;
		}

		/// Destructor
		~LinkedList()
		{
			// Very important -- we have to clean up all our memory!
			clear();
		}


		/// Gets an element at the given "position".  Note: since LinkedList's don't support
		/// random-access (unlike ArrayList), this is a costly operation
		T& operator[](const int index) const
		{
			if (index < 0 || index >= (int)mSize)
				throw std::out_of_range("Invalid index");
			Node* cur = mStart;
			for (int i = 0; i <= index; i++)
			{
				if (i == index)
					return cur->mData;
				cur = cur->mNext;
			}
			// We should ever get here, but if we do, it means something's wrong with our mSize
			throw std::out_of_range("Invalid index");
		}


		/// Replaces this linked list with a copy of the data in the other LinkedList.  This *should* work
		/// even if the (silly) user assigns a LinkedList to itself
		LinkedList& operator=(const LinkedList& L)
		{
			if (&L != this)
			{
				clear();
				for (T val : L)
					append(val);
			}

			// Return a reference to ourself
			return *this;
		}


		/// Stream operator definition for our linked list class
		friend std::ostream& operator<<(std::ostream& os, const LinkedList& L)
		{
			os << "[";
			unsigned int i = 0;

			//Node* temp = nullptr;
			Node* temp = L.mStart;

			for (T val : L)
			{
				//os << val;
				os << temp->mData;
				i++;
				if (i < L.size())
					os << ", ";
				temp = temp->mNext;
			}
			os << "]";
			return os;
		}


		/// Adds a new Node containing this data to the end of the linked list
		void append(const T& val)
		{
			if (mStart == NULL)
			{
				// Case I: This is the first Node.  start and end will both point towards it
				mStart = mEnd = new Node(val);
			}
			else
			{
				// Case II -- we already have a head -- add the new Node to the end
				Node* new_Node = new Node(val);
				new_Node->mPrev = mEnd;
				mEnd->mNext = new_Node;
				mEnd = new_Node;
			}
			mSize++;
		}


		/// Returns an iterator to the beginning of the list
		LinkedListIterator begin() const
		{
			if (mSize == 0)
				return end();
			return LinkedListIterator(this, mStart, 0, true);
		}


		/// Resets the linked list back to its original state
		void clear()
		{
			Node* temp = mStart;
			while (temp != nullptr)
			{
				// We need this because after we delete temp, the memory isn't ours anymore!
				Node* next = temp->mNext;

				// Delete the current Node
				delete temp;

				// Advance to the next Node (if any) using the saved pointer.
				temp = next;
			}

			// Reset all of our attributes
			mStart = mEnd = nullptr;
			mSize = 0;
		}


		/// Returns a "sentinel" value -- if an iterator is equal to this, we're done traversing
		LinkedListIterator end() const
		{
			return LinkedListIterator(this, NULL, -1, true);
		}


		/// Finds the first occurrence of the given value starting at the given iterator
		LinkedListIterator find(const T& val, const LinkedListIterator& start) const
		{
			if (start.mCurrentNode == NULL)
				return end();
			Node* cur = start.mCurrentNode;
			for (int i = start.mCurrentIndex; i < (int)mSize; i++)
			{
				if (cur->mData == val)
					return LinkedListIterator(this, cur, i, true);
				cur = cur->mNext;
			}
			return end();
		}


		/// Finds the first occurrence of the value, starting at the beginning of the list
		LinkedListIterator find(const T& val) const
		{
			return find(val, begin());
		}



		/// Inserts the given value at the currently occupied spot.  There's some extra overhead
		/// here to get to the given spot
		void insert(const T& item, const int index)
		{
			if (index == 0)
				prepend(item);
			else if (index == mSize)
				append(item);
			else if (index < 0 || index > (int)mSize)
				throw std::out_of_range("Invalid index");
			else
			{
				// Get to the Node at spot "index"
				Node* cur = mStart;
				for (int i = 0; i < index; i++)
					cur = cur->mNext;

				insert_internal(item, cur);
			}
		}


		/// This is an alternate version of insert.  It skips the iteration step (nice if we
		/// were to already have an iterator pointing to the correct spot for insertion)
		void insert(const T& item, const LinkedListIterator& iter)
		{
			if (iter.index() == 0)
				prepend(item);
			else if (iter.index() == mSize)
				append(item);
			else if (iter.mCurrentNode == nullptr)
				throw std::out_of_range("Invalid iterator");
			else
				insert_internal(item, iter.mCurrentNode);
		}

	protected:
		/// An internal method to insert a new value before an existing Node -- this Node should not
		/// be either the start / end Node (use prepend/append in these situtations)
		void insert_internal(const T& item, Node* cur)
		{
			// Because of our initial checks, we can be sure that this Node will be between two 
			// existing Nodes
			Node* new_Node = new Node(item);
			new_Node->mPrev = cur->mPrev;
			new_Node->mNext = cur;
			cur->mPrev->mNext = new_Node;
			cur->mPrev = new_Node;

			// Increase our size
			mSize++;
		}

	public:
		

		/// Adds a new Node containing this data to the beginning of the linked list
		void prepend(const T& val)
		{
			if (mStart == NULL)
			{
				// Case I: This is the first Node.  start and end will both point towards it
				mStart = mEnd = new Node(val);
			}
			else
			{
				// Case II -- we already have a head -- add the new Node before it
				Node * new_Node = new Node(val);
				new_Node->mNext = mStart;
				mStart->mPrev = new_Node;
				mStart = new_Node;
			}
			mSize++;
		}


		/// Returns a reverse iterator starting at the end of the list.
		LinkedListIterator rbegin() const
		{
			if (mSize == 0)
				return end();
			return LinkedListIterator(this, mEnd, mSize - 1, false);
		}


		/// Returns a "sentinel" value -- if a reverse-iterator is equal to this, we're done traversing
		LinkedListIterator rend() const
		{
			return LinkedListIterator(this, NULL, -1, false);
		}



		/// Removes the Node at the given iterator "position".  
		LinkedListIterator remove(const LinkedListIterator& spot)
		{
			if (spot.mCurrentNode == nullptr || this != spot.mParent)
				throw std::invalid_argument("Invalid iterator");

			if (mSize == 1)
			{
				// There's only one element here.  We're clearing the list
				T val = mStart->mData;
				delete mStart;
				mStart = mEnd = NULL;
				mSize = 0;
				return end();
			}
			else if (spot.mCurrentIndex == 0)
			{
				// Remove the first element
				Node* next = mStart->mNext;
				next->mPrev = NULL;
				T val = mStart->mData;
				delete mStart;
				mStart = next;
				mSize--;
				if (spot.mForward)
					return LinkedListIterator(this, mStart, 0, true);
				else
					return rend();
			}
			else if (spot.mCurrentIndex == mSize - 1)
			{
				// Remove the last element
				Node* prev = mEnd->mPrev;
				prev->mNext = NULL;
				T val = mEnd->mData;
				delete mEnd;
				mEnd = prev;
				mSize--;
				if (spot.mForward)
					return end();
				else
					return LinkedListIterator(this, mEnd, mSize - 1, false);
			}
			else
			{
				Node* next = spot.mCurrentNode->mNext;
				Node* prev = spot.mCurrentNode->mPrev;
				next->mPrev = prev;
				prev->mNext = next;
				Node* goner = spot.mCurrentNode;
				LinkedListIterator rv;
				if (spot.mForward)
					rv = LinkedListIterator(this, goner->mNext, spot.mCurrentIndex, true);
				else
					rv = LinkedListIterator(this, goner->mPrev, spot.mCurrentIndex, false);
				delete goner;
				mSize--;
				return rv;
			}
		}


		/// <summary>
		/// Removes all occurrences of the given value (if any)
		/// </summary>
		/// <param name="value">the value to remove</param>
		/// <returns>the number of items removed</returns>
		unsigned int remove_all(const T& value)
		{
			unsigned int num = 0;
			LinkedListIterator it = find(value);
			while (it != end())
			{
				// Remove the occurrence we just found.  it will refer to the
				// thing after the item we removed
				it = remove(it);

				// Find another item with that value (if any).  Note that since it
				// already (By remove's logic) referring to thing after the one we
				// just removed, no +1 necessary
				it = find(value, it);

				++num;
			}

			return num;
		}

	public:


		/// Gets the number of elements in the linked list.  Don't implement it this way, but interesting
		/// challenge: how would you get the size of a linked-list if you weren't storing our mSize int?
		unsigned int size() const
		{
			return mSize;
		}

		


		

		
	};	// END of LinkedList class
}	// END of ssuge namespace