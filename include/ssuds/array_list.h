#pragma once
#include <string>
#include <ostream>

// Note: in C++, a general tempate (like this one) must be defined inline
// entirely in the .h file (no .cpp files).  So, in this lab, array_list.cpp
// contents are moved here

namespace ssuds
{
	// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
	// @ ENUM CLASSES                           @
	// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
	enum class ArrayListIteratorType { forward, backwards };

	/// An ArrayList is an array-based data structure. 
	template <class T>
	class ArrayList
	{
	// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
	// @ NESTED CLASSES                         @
	// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
	public:
		/// <summary>
		///  This is a forward/backwards iterator to traverse the values in an ArrayList
		/// </summary>
		class ArrayListIterator
		{
		protected:
			/// <summary>
			/// A pointer to the controlling array list
			/// </summary>
			const ArrayList* mArrayList;

			/// <summary>
			///  The current position / index in our traversal
			/// </summary>
			int mPosition;

			/// <summary>
			/// The type of traversal we're doing (forward or backwards)
			/// </summary>
			ArrayListIteratorType mType;

		public:
			/// <summary>
			///  The constructor (called indirectly by begin and rbegin in the ArrayList)
			/// </summary>
			/// <param name="arr">The array we're traversing</param>
			/// <param name="tp">The type of traversal to do</param>
			/// <param name="start_index">The starting index</param>
			ArrayListIterator(const ArrayList* arr, ArrayListIteratorType tp, int start_index) : mArrayList(arr), mPosition(start_index), mType(tp)
			{
				// intentionally empty
			}

			/// Comparison with another iterator
			bool operator==(const ArrayListIterator& other) const
			{
				return mArrayList == other.mArrayList && mPosition == other.mPosition && mType == other.mType;
			}

			/// Not-comparison with another iterator
			bool operator!=(const ArrayListIterator& other) const
			{
				return mArrayList != other.mArrayList || mPosition != other.mPosition || mType != other.mType;
			}

			/// <summary>
			/// Advance operator.  Called by doing ++it
			/// </summary>
			void operator++()
			{
				if (mType == ArrayListIteratorType::forward)
					++mPosition;
				else
					--mPosition;
			}

			/// <summary>
			/// Advance operator.  Called by doing it++
			/// </summary>
			/// <param name=""></param>
			void operator++(int)
			{
				return ++(*this);
			}

			/// <summary>
			///  De-reference style operator
			/// </summary>
			/// <returns>A reference to the value at the current spot</returns>
			T& operator*()
			{
				return (*mArrayList)[mPosition];
			}
		};

	// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
	// @ ATTRIBUTES                              @
	// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
	protected:
		/// The default (and minimum) capacity of an ArrayList
		static const int msMinCapacity = 5;

		/// The current number of "slots" AVAILABLE in mData (i.e. the array size)
		unsigned int mCapacity;

		/// How many slots are we USING?  This will always be less than or equal to mCapacity
		unsigned int mSize;

		/// The array of data we're currently holding
		T* mData;


	// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
	// @ OPERATOR OVERLOADS                      @
	// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
	public:

	/// <summary>
	/// Overloads the stream operator for ArrayLists.
	/// </summary>
	/// <param name="os">an ostream object (ofstream, stringstream, cout, etc.) </param>
	/// <param name="alist">the ArrayList</param>
	/// <returns>the (possibly modified) os that was given to us</returns>
	friend std::ostream& operator <<(std::ostream& os, const ArrayList<T>& alist)
	{
		os << "[";
		for (unsigned int i = 0; i < alist.size(); i++)
		{
			os << alist[i];
			if (i < alist.size() - 1)
				os << ", ";
		}
		os << "]";
		return os;
	}

	/// <summary>
	/// Gets the data item at the given index.  This will throw an std::out_of_bounds exception if index is invalid (less than 0 or >= size)
	/// </summary>
	/// <param name="index">the index of the thing to return</param>
	/// <returns>a reference to the value at the given index</returns>
	T& operator[](unsigned int index) const
	{
		if (index >= mSize)
			throw std::out_of_range("Invalid index: " + std::to_string(index));

		return mData[index];
	}


	ArrayList<T>& operator= (const ArrayList<T>& other)
	{
		clear();
		mData = new T[other.mCapacity];
		for (unsigned int i = 0; i < other.mSize; i++)
			mData[i] = other[i];
		mCapacity = other.mCapacity;
		mSize = other.mSize;
		return *this;
	}


	// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
	// @ CONSTRUCTORS / DESTRUCTORS              @
	// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
	public:
		/// Default constructor
		ArrayList() : mSize(0), mCapacity(0), mData(nullptr)
		{
			// intentionally empty
		};

		/// Copy-constructor
		ArrayList(const ArrayList& other) : mCapacity(other.mCapacity), mSize(other.mSize)
		{
			mData = new T[mCapacity];
			for (unsigned int i = 0; i < other.size(); i++)
				mData[i] = other[i];
		}

		/// Move-constructor: "steals" the data (shallow copy) from a soon-to-be-destroyed other ArrayList
		ArrayList(ArrayList&& other) : mCapacity(other.mCapacity), mSize(other.mSize), mData(other.mData)
		{
			other.mData = NULL;
			other.mCapacity = 0;
			other.mSize = 0;
		}

		/// Initializer-list constructor
		ArrayList(std::initializer_list<T> ilist) : mCapacity((int)ilist.size()), mSize((int)ilist.size())
		{
			mData = new T[mCapacity];
			int i = 0;
			for (T val : ilist)
				mData[i++] = val;
		}

		/// Destructor
		~ArrayList() 
		{
			if (mData)
				delete[] mData;
		}


	// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
	// @ OTHER METHODS (alphabetical)            @
	// @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
	public:
		/// <summary>
		///  Inserts a new element at the end of the array
		/// </summary>
		/// <param name="val">the new value to add</param>
		void append(const T val)
		{
			// check to see if we need to increase our capacity
			grow();

			// Stick our new element in the last slot and (sneakily) increase our size in the process
			mData[mSize] = val;
			mSize++;
		}


		/// <summary>
		/// Create and return a forward iterator 
		/// </summary>
		/// <returns>a forward iterator</returns>
		ArrayListIterator begin() const
		{
			return ArrayListIterator(this, ArrayListIteratorType::forward, 0);
		}


		/// <summary>
		/// Gets the current capacity of the array list
		/// </summary>
		/// <returns>current capacity (which should always be >= size)</returns>
		unsigned int capacity() const
		{
			return mCapacity;
		}

		/// <summary>
		/// Clears the array back to its original state
		/// </summary>
		void clear()
		{
			if (mData)
				delete[] mData;
			mData = nullptr;
			mSize = 0;
			mCapacity = 0;
		}

		/// <summary>
		/// Returns a special iterator value indicating we've reached the end of iteration
		/// </summary>
		/// <returns>special end iterator value</returns>
		ArrayListIterator end() const
		{
			return ArrayListIterator(this, ArrayListIteratorType::forward, mSize);
		}

		/// <summary>
		/// Finds the index of the first occurrence of the given value
		/// </summary>
		/// <param name="val">the value to search for</param>
		/// <param name="start_index">the index to start searching at</param>
		/// <returns></returns>
		int find(const T val, unsigned int start_index = 0) const
		{
			if (start_index >= mSize)
				throw std::out_of_range("Invalid index: " + std::to_string(start_index));

			for (unsigned int i = start_index; i < mSize; i++)
			{
				if (mData[i] == val)
					return i;
			}

			// We didn't find it
			return -1;
		}

	

		/// <summary>
		/// Inserts a new data item at a given index
		/// </summary>
		/// <param name="val">the new value to insert</param>
		/// <param name="index">the index at which to insert (must be >= 0 and <= size) </param>
		void insert(T val, unsigned int index)
		{
			if (index > mSize)
				throw std::out_of_range("Invalid index: " + std::to_string(index));

			// check to see if we need to increase capacity first
			grow();

			// Move all the elements that come *after* index up one spot
			for (unsigned int i = mSize; i > index; i--)
				mData[i] = mData[i - 1];

			// Put our new elements in spot index and increase our size
			mData[index] = val;
			mSize++;
		}


		/// <summary>
		/// Create and return a reverse iterator 
		/// </summary>
		/// <returns>a reverse iterator</returns>
		ArrayListIterator rbegin() const
		{
			return ArrayListIterator(this, ArrayListIteratorType::backwards, mSize - 1);
		}


		/// <summary>
		/// Returns a special reverse end iterator value indicating we've reached the end of iteration
		/// </summary>
		/// <returns>special reverse end iterator value</returns>
		ArrayListIterator rend() const
		{
			return ArrayListIterator(this, ArrayListIteratorType::backwards, -1);
		}


		/// <summary>
		/// Removes a data item at the given index
		/// </summary>
		/// <param name="index">the index of the thing to remove (will return a std::out_of_bounds exception if invalid (<0 or >= size)</param>
		/// <param name="resize_if_necessary">if true, the array will be resized if it is now below half capacity</param>
		/// <returns>the data item that was just removed</returns>
		T remove(unsigned int index, bool resize_if_necessary = true)
		{
			if (index >= mSize)
				throw std::out_of_range("Invalid index: " + std::to_string(index));

			// Get the value we'll return at the end (the element removed)
			T result = mData[index];

			// Move all elements that come after index down one spot
			for (unsigned int i = index; i < mSize - 1; i++)
				mData[i] = mData[i + 1];

			// Decrement our size
			mSize--;

			// Shrink, if applicable and requested
			if (resize_if_necessary)
				shrink();

			// Return the result
			return result;
		}

		/// <summary>
		/// Removes all occurrences of a given value.  Uses find and remove internally to do the removal
		/// </summary>
		/// <param name="val">the value to remove</param>
		/// <param name="resize_if_necessary">if true, the array will be resized if it is now below half capacity</param>
		/// <returns>the number of occurrences of that data item that were removed</returns>
		int remove_all(const T val, bool resize_if_necessary=true)
		{
			int cur_index = 0;
			int num_removed = 0;
			while (cur_index >= 0 && cur_index < (int)mSize)
			{
				// Find the next occurrence of val, starting at the current position
				cur_index = find(val, cur_index);
				if (cur_index >= 0)
				{
					// We found one!
					remove(cur_index, false);
					num_removed++;
				}
			}

			if (resize_if_necessary)
				shrink();

			return num_removed;
		}

		/// <summary>
		/// Returns the size of the internal array (i.e.) how many things are being stored in the ArrayList
		/// </summary>
		/// <returns>the size of the ArrayList</returns>
		unsigned int size() const
		{
			return mSize;
		}


	protected:
		/// <summary>
		/// An internal method to resize the array if we are currently at capacity (if we are not, nothing is done)
		/// </summary>
		void grow()
		{
			if (mSize == mCapacity)
			{
				// Allocate what will become the new array
				T* new_array = nullptr;
				if (mCapacity == 0)
					new_array = new T[msMinCapacity];
				else
					new_array = new T[mCapacity * 2];

				// Copy over data from the old array (if any)
				if (mData != nullptr)
				{
					for (unsigned int i = 0; i < mSize; i++)
						new_array[i] = mData[i];

					// Destroy the old array
					delete[] mData;
				}

				// Make the new array *the* array
				mData = new_array;

				// Note that our capacity is now double what it used to be
				if (mCapacity == 0)
					mCapacity = msMinCapacity;
				else
					mCapacity *= 2;
			}
		}


		/// <summary>
		/// An internal method to see if the array can be shrunk (capacity reduced by half, down to msMinCapacity)
		/// </summary>
		void shrink()
		{
			if (mSize < mCapacity / 2 && mCapacity >= msMinCapacity * 2)
			{
				// Allocate what will become the new array
				T* new_array = new T[mCapacity / 2];

				// Copy over data from the old array (if any)
				for (unsigned int i = 0; i < mSize; i++)
					new_array[i] = mData[i];
			

				// Destroy the old array
				delete[] mData;

				// Make the new array *the* array
				mData = new_array;

				// Note that our capacity is now double what it used to be
				mCapacity /= 2;
			}
		}
	};
}
