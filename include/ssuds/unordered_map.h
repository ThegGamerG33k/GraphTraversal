#pragma once
#include <functional>
#include <array_list.h>

#define INITIAL_MAP_CAPACITY 5
#define MAX_LOAD_FACTOR 0.7f

namespace ssuds
{
	// An unordered map is a hash-table-based container for holdling keys and associated
	// values.  It internally uses an array to hold nodes (similar to std::pair)
	template <class K, class V>
	class UnorderedMap
	{
	public:
		/// This is the iterator, which returns std::pairs (rather than our hidden node class)
		class UnorderedMapIterator
		{
		protected:
			/// A pointer to the map we're iterating through
			const UnorderedMap* mMapPtr;

			/// The current **valid** spot we're on (-1 if we've finished iterating)
			long long mIndex;
		public:
			/// Default constructor (generally used for the end iterator value)
			UnorderedMapIterator() : mMapPtr(nullptr), mIndex(-1)
			{
				// empty, intentionally
			}


			/// Constructor to use to do *real* iteration
			UnorderedMapIterator(const UnorderedMap* m)
			{
				mMapPtr = m;
				if (m)
					mIndex = find_next_index(0);
				else
					mIndex = -1;
			}


			/// Creates an iterator pointing to a specific element.  No attempt is made to ensure this is a *real* element
			UnorderedMapIterator(const UnorderedMap* m, long long index) : mMapPtr(m), mIndex(index)
			{
				// empty, on purpose
			}

			/// compares two iterators
			bool operator == (const UnorderedMapIterator& mi) const
			{
				return mIndex == mi.mIndex && (mMapPtr == mi.mMapPtr || mIndex == -1);
			}

			/// compares two iterators (negation)
			bool operator != (const UnorderedMapIterator& mi) const
			{
				return mIndex != mi.mIndex || (mMapPtr != mi.mMapPtr && mIndex != -1);
			}

			/// Returns the current *node*'s values, but as a std::pair (so we don't have
			/// to give the user access to our node class).  If we had made the map's
			/// internal mData as an array of pair pointers, this translation wouldn't be necessary
			std::pair<K, V>& operator*() const
			{
				return *mMapPtr->mData[mIndex];
			}

			/// Advance the iteration
			void operator++()
			{
				if (mIndex >= 0)
					mIndex = find_next_index(mIndex + 1);
			}

			/// Attempts to advance us amount steps and creates a new iterator at that "spot"
			UnorderedMapIterator operator+(int amount) const
			{
				int index = mIndex;
				int steps_performed = 0;
				while (index >= 0 && steps_performed < amount)
				{
					index = find_next_index(index + 1);
					steps_performed++;
				}
			}

		protected:
			/// A helper method to find the next non-null entry at / after start
			long long find_next_index(long long start) const
			{
				while (start < (long long)mMapPtr->mCapacity)
				{
					if (mMapPtr->mData[start] != NULL)
						return start;
					start++;
				}
				return -1;
			}
		};



	protected:
		/// An array of pair pointers.  In a default constructor this is initialized to the size INITIAL_MAP_CAPACITY, and
		/// it "grows" when the load factor (mSize / mCapacity) reaches or exceeds MAX_LOAD_FACTOR
		std::pair<K, V>** mData;

		/// The generator we use to create hash codes for our keys
		std::hash<K> mHashGenerator;

		/// The current number of key-value pairs in this container
		size_t mSize;

		/// The number of elements in mData
		size_t mCapacity;

	public:
		/// Default constructor
		UnorderedMap() : mSize(0), mCapacity(INITIAL_MAP_CAPACITY)
		{
			mData = new std::pair<K,V>* [mCapacity];
			memset(mData, 0, sizeof(std::pair<K,V>*) * mCapacity);
		}


		/// Copy-constructor
		UnorderedMap(const UnorderedMap& m)
		{
			mData = new std::pair<K,V>* [m.mCapacity];
			mCapacity = m.mCapacity;
			mSize = m.mSize;
			// We shouldn't need to re-hash these since our table capacity is the same size
			// We do, however, need to make a copy of their nodes
			for (int i = 0; i < mCapacity; i++)
			{
				if (m.mData[i])
					mData[i] = new std::pair<K,V>(*m.mData[i]);
				else
					mData[i] = nullptr;
			}
		}


		/// Move-constructor.  
		UnorderedMap(UnorderedMap&& m)
		{
			mData = m.mData;
			mSize = m.mSize;
			mCapacity = m.mCapacity;
			m.mData = nullptr;
			m.mSize = 0;
			m.mCapacity = 0;
		}


		~UnorderedMap()
		{
			// Very important -- deleting our array would introduce a big memory leak.  We
			//  need to delete the elements of our array first
			for (int i = 0; i < mCapacity; i++)
			{
				// Note: many of these will be nullptr, but the delete properly handles that case
				delete mData[i];
			}

			// Delete the array itself
			delete[] mData;
		}

		/// =operator
		UnorderedMap& operator=(const UnorderedMap& other)
		{
			// Delete the non-null nodes in our table
			for (int i = 0; i < mCapacity; i++)
				delete mData[i];

			// Delete the array itself
			delete[] mData;

			// Make a copy
			mData = new std::pair<K,V>* [other.mCapacity];
			mCapacity = other.mCapacity;
			mSize = other.mSize;
			for (int i = 0; i < mCapacity; i++)
			{
				if (other.mData[i])
					mData[i] = new std::pair<K,V>(*other.mData[i]);
				else
					mData[i] = nullptr;
			}

			// Return us
			return *this;
		}


		/// Outputs the unorderd map in this form: {keyA: valueA, keyC: valueC, keyB: valueB}
		/// Note that this is an unordered set, so the output is in some arbitrary order
		friend std::ostream& operator<<(std::ostream& os, const UnorderedMap& m)
		{
			m.debug_output(os, false);
			return os;
		}


		/// This is the most fundamental, but also trickier methods.  It is the way maps add
		/// and modify elements.  The trick is, we use the key to find the right spot.  If the
		/// elements not there (which means this is an add), we make a new entry with the value
		/// uninitialized BUT also return a reference to the value.  If the user changes it like
		/// my_list[my_key] = my_value;
		/// then the new entry is fully initialized.  But if the user does
		/// my_list[my_key];
		/// it would create the entry, but the value will be some uninitialized value.  std::unordered_map's
		/// also have this same behavior.
		V& operator[](const K& key)
		{
			size_t index = find_spot(key);
			if (mData[index] == NULL)
			{
				// That value isn't in here.  Create it and return 
				// a reference to the new node's value.
				mData[index] = new std::pair<K, V>;
				mData[index]->first = key;
				mSize++;
				if (check_for_capacity())
				{
					// The index will almost surely be different if we grew
					index = find_spot(key);
				}
				return mData[index]->second;
			}
			else
			{
				// We do have that item.  Return it to them -- they
				// can use it or modify it (since it's a reference)
				return mData[index]->second;
			}
		}


		/// Removes all elements and sets the array back to its initial (INITIAL_MAP_CAPACITY) capacity
		void clear() 
		{
			// Delete the non-null nodes
			for (int i = 0; i < mCapacity; i++)
				delete mData[i];

			// Delete the array itself
			delete[] mData;

			// Reset things
			mCapacity = INITIAL_MAP_CAPACITY;
			mData = new std::pair<K,V> * [mCapacity];
			mSize = 0;
			memset(mData, 0, sizeof(std::pair<K,V>*) * mCapacity);
		}


		/// Returns true if we have a pair with this key
		UnorderedMapIterator find(const K& key) const
		{
			size_t index = find_spot(key);
			if (mData[index])
				return UnorderedMapIterator(this, index);
			else
				return end();
		}


		/// my "augmented" stream operator "overload".  I did this so I could show the null entries if desired
		/// The real stream operator just calls this, passing false for the second parameter
		void debug_output(std::ostream& os, bool show_nulls = true) const
		{
			os << "{";
			bool first = true;
			if (show_nulls)
			{
				for (int i = 0; i < mCapacity; i++)
				{
					if (!first)
						os << ", ";
					else
						first = false;

					if (mData[i] == NULL)
						os << "NULL";
					else
						os << mData[i]->first << ":" << mData[i]->second;
				}
			}
			else
			{
				for (int i = 0; i < mCapacity; i++)
				{
					if (mData[i] != NULL)
					{
						if (!first)
							os << ", ";
						else
							first = false;

						os << mData[i]->first << ":" << mData[i]->second;
					}
				}
			}
			os << "}";
		}


		/// Removes the pair with the given key and returns the corresponding value.  It 
		/// raises an out_of_range exception if not (the user should check for existence first
		/// with the find method)
		V remove(const K& key)
		{
			size_t index = find_spot(key);
			size_t start_index = index;
			bool found_it = false;
			V found_val;
			bool found_null = false;

			// First, collect all the values we find after that index until we
			// find a NULL.
			ArrayList<K> keys_to_readd;
			ArrayList<V> values_to_readd;

			while (true)
			{
				if (mData[index] == NULL)
				{
					found_null = true;
					break;
				}
				else
				{
					if (mData[index]->first == key)
					{
						found_val = mData[index]->second;
						found_it = true;
					}
					else
					{
						keys_to_readd.append(mData[index]->first);
						values_to_readd.append(mData[index]->second);
					}
					delete mData[index];
					mData[index] = NULL;
				}
				++index;
				if (index == start_index)
					throw std::out_of_range("invalid heap state");
				else if (index == mCapacity)
					index = 0;
			}

			// Now, re-add any key-value pairs that we removed along the way
			mSize -= keys_to_readd.size() + 1;
			for (int i = 0; i < (int)keys_to_readd.size(); i++)
				(*this)[keys_to_readd[i]] = values_to_readd[i];

			// Return the removed element
			if (found_it)
				return found_val;
			else
				throw std::out_of_range("The value doesn't exist in the map");
		}


		/// Returns the number of key-value pairs currently being stored
		size_t size() const
		{
			return mSize;
		}


		/// Returns the number of available slots in the table.
		size_t capacity() const
		{
			return mCapacity;
		}
		

	protected:
		/// This method is used internally by check_for_capacity.  If we decide 
		/// that we need to grow, we can remove and re-add (using the [] operator),
		/// but if the thing we're adding is complex, deleting it and re-adding it
		/// could cause problems (in that case, this method is better)
		void internal_add(std::pair<K,V>* pair)
		{
			size_t index = find_spot(pair->first);
			if (mData[index] == NULL)
			{
				// That value isn't in here.  Create it and return 
				// a reference to the new node's value.
				mData[index] = pair;
			}
			else
				throw std::runtime_error("Invalid heap state in grow");
		}


		/// A helper method which returns the index at which the key-value pair with the given key
		/// *does* appear or *would* appear (if it were in the table)
		size_t find_spot(const K& key) const
		{
			size_t start_index = mHashGenerator(key) % mCapacity;
			size_t cur_index = start_index;
			for (; cur_index < (int)mCapacity; cur_index++)
			{
				if (mData[cur_index] == NULL || mData[cur_index]->first == key)
					return cur_index;
			}

			// If we get here, we didn't find it yet, loop back to the beginning
			for (cur_index = 0; cur_index < start_index; cur_index++)
			{
				if (mData[cur_index] == NULL || mData[cur_index]->first == key)
					return cur_index;
			}

			// If we get here, the grow operation is not working properly
			throw std::out_of_range("Invalid state: empty hash table");
		}


		/// Helper function used to "grow" the table.  I could (but currently don't) shrink the table
		bool check_for_capacity()
		{
			float cur_capacity = (float)mSize / mCapacity;
			if (cur_capacity > MAX_LOAD_FACTOR)
			{
				// Collect all the key-value pairs
				ArrayList<std::pair<K,V>*> pairs;
				for (int i = 0; i < mCapacity; i++)
				{
					if (mData[i] != NULL)
						pairs.append(mData[i]);
				}

				// Re-allocate the array (doubling)
				mCapacity <<= 1;					// bit-shift one place (double)
				delete[] mData;
				mData = new std::pair<K,V> * [mCapacity];
				memset(mData, 0, sizeof(std::pair<K,V>*) * mCapacity);

				// Re-add all the key-value pairs
				for (int i = 0; i < (int)pairs.size(); i++)
					internal_add(pairs[i]);

				return true;
			}
			else
				return false;
		}

	public:
		
		/// Returns an iterator to the first value in the map, if there is one (this iterator will
		/// be equal to the end iterator if there are none)
		UnorderedMapIterator begin() const
		{
			return UnorderedMapIterator(this);
		}


		/// Return a special iterator which indicates the end of iteration, not finding a value, etc.
		UnorderedMapIterator end() const
		{
			return UnorderedMapIterator(NULL);
		}
	};
}
