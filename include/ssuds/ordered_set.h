#pragma once
#include <array_list.h>
#include <sstream>
#include <stack.h>
#include <iostream>

namespace ssuds
{
	/// Used to indicate a type of traversal
	enum class TraversalType {IN_ORDER, PRE_ORDER, POST_ORDER};

	/// <summary>
	/// An ordered set is a collection of unique values with the ability to access those
	/// values in-order (plus a few other ways)
	/// </summary>
	/// <typeparam name="T">The type of data we'll store in the set</typeparam>
	template <class T>
	class OrderedSet
	{
		/// <summary>
		/// In this implementation, the node is where most of the work happens.  All methods
		/// are coded recursively
		/// </summary>
		class node
		{
		public:
			T mData;
			node* mLeft;
			node* mRight;
	
			/// Constructor
			node(T val) : mData(val), mLeft(nullptr), mRight(nullptr)
			{
				// empty, on purpose
			}


			/// Destructor
			~node()
			{
				// Delete my children (note: i've come to realize [thanks, Zach Kerner!] that calling
				/// delete on a null-pointer is allowed (it does nothing)
				delete mLeft;
				delete mRight;

				// Nothing else to do!  Someone else has deleted me (which is how I got here)
			}

			/// <summary>
			/// Clears the children pointers without deleting them (useful if we want to destroy just
			/// a node)
			/// </summary>
			void clear_children()
			{
				mLeft = mRight = nullptr;
			}

			/// <summary>
			/// Determines if the given value is anywhere in the subtree rooted with us
			/// </summary>
			/// <param name="val">The value we're looking for</param>
			/// <returns>true if we have it, false if not</returns>
			bool contains_recursive(const T& val)
			{
				if (mData == val)
					return true;
				else if (val < mData && mLeft)
					return mLeft->contains_recursive(val);
				else if (val > mData && mRight)
					return mRight->contains_recursive(val);

				return false;
			}


			/// <summary>
			/// Removes the given element from this sub-tree, if it exists
			/// </summary>
			/// <param name="val">the value to remove</param>
			/// <returns>the new value this node should be set to</returns>
			node* erase_recursive(const T& val, bool& found)
			{
				// Do we contain the data to remove?
				if (mData == val)
				{
					found = true;

					// Are we a leaf node
					if (mLeft == nullptr && mRight == nullptr)
					{
						// We are a leaf -- signal to the caller that they should destroy us and set that
						// child to null
						return nullptr;
					}
					else if (mLeft == nullptr || mRight == nullptr)
					{
						// We are not a leaf, but contain just one child -- return the value of our child
						// -- the caller should destroy our node
						if (mLeft != nullptr)
							return mLeft;
						else
							return mRight;
					}
					else
					{
						// The more difficult case -- we have two children
						node* successor = find_successor();			// should never be null based on our
																	// if check's above
						mData = successor->mData;					// We now (for a moment) have two copies
																	// of that value in our set.

						// This will remove the DUPLICATE value.
						node* result = mRight->erase_recursive(mData, found);				
						if (result != mRight)
						{
							mRight->clear_children();
							delete mRight;
							mRight = result;
						}

						// We return ourself because we don't actually want to destroy any nodes (other than the
						// one that should've been destroyed
						return this;
					}
				}
				else if (mLeft && val < mData)
				{
					// Tell our left child to attempt to erase it
					node* result = mLeft->erase_recursive(val, found);
					if (result != mLeft && found)
					{
						mLeft->clear_children();
						delete mLeft;
						mLeft = result;
					}
					return this;
				}
				else if (mRight && val > mData)
				{
					// Tell our right child to attempt to erase it
					node* result = mRight->erase_recursive(val, found);
					if (result != mRight && found)
					{
						mRight->clear_children();
						delete mRight;
						mRight = result;
					}
					return this;
				}
				else
				{
					// We don't have the value!
					found = false;
					return this;
				}
			}


			/// <summary>
			/// Helper method for erase -- finds the node that value-wise comes after this node (or null if
			/// there are none)
			/// </summary>
			/// <returns>this node's successor node</returns>
			node* find_successor()
			{
				if (mRight)
				{
					node* cur = mRight;
					while (cur->mLeft)
						cur = cur->mLeft;
					return cur;
				}
				else
					return nullptr;
			}


			/// <summary>
			/// Used to generate a text-visualization of this node and its descendents of this form
			///         [my_data]      (shows more whitespace for deeper nodes)
			///             L: [my-left-child]
			///               ...
			///             R: [my-right-child]
			///               ...
			/// </summary>
			/// <param name="ss">a reference to a string-stream which is added to by this method</param>
			/// <param name="recursion_depth">controls the amount of whitespace before my data</param>
			/// <param name="prefix">a label to put before my data (generally "", "L: ", or "R: ")</param>
			void generate_string_recursive(std::stringstream& ss, int recursion_depth, std::string prefix)
			{
				for (int i = 0; i < recursion_depth; i++)
					ss << "   ";
				ss << prefix << mData << std::endl;
				if (mLeft)
					mLeft->generate_string_recursive(ss, recursion_depth + 1, "L: ");
				if (mRight)
					mRight->generate_string_recursive(ss, recursion_depth + 1, "R: ");
			}


			/// <summary>
			/// Gets the height of this tree (1 if we just have the root)
			/// </summary>
			/// <returns></returns>
			unsigned int get_height_recursive() const
			{
				unsigned int left_ht = 0, right_ht = 0;
				if (mLeft)
					left_ht = mLeft->get_height_recursive();
				if (mRight)
					right_ht = mRight->get_height_recursive();
				
				return (left_ht > right_ht ? left_ht : right_ht) + 1;
			}


			/// <summary>
			/// Actually does an insert operation.  If the given value exists already, this method
			/// does nothing
			/// </summary>
			/// <param name="val">The new value to insert</param>
			/// <returns>the number of new nodes created (0 or 1)</returns>
			int insert_recursive(const T& val)
			{
				if (val < mData)
				{
					if (mLeft)
						return mLeft->insert_recursive(val);
					else
					{
						mLeft = new node(val);
						return 1;
					}
				}
				else if (val > mData)
				{
					if (mRight)
						return mRight->insert_recursive(val);
					else
					{
						mRight = new node(val);
						return 1;
					}
				}
				else
				{
					// This node already exists -- just return 0 since there's nothing else we need to do
					return 0;
				}
			}


			/// <summary>
			///  Used to generate a copy of all data in this set in a certain order
			/// </summary>
			/// <param name="data">a reference parameter used to store all data</param>
			/// <param name="tp">which type of traversal?</param>
			void traversal_recursive(ssuds::ArrayList<T>& data, TraversalType tp)
			{
				if (tp == TraversalType::PRE_ORDER)
					data.append(mData);
				if (mLeft)
					mLeft->traversal_recursive(data, tp);
				if (tp == TraversalType::IN_ORDER)
					data.append(mData);
				if (mRight)
					mRight->traversal_recursive(data, tp);
				if (tp == TraversalType::POST_ORDER)
					data.append(mData);
			}
		};

	public:
		/// <summary>
		/// This class is used to iterate through an OrderedSet instance
		/// </summary>
		class OrderedSetIterator
		{
		protected:
			/// <summary>
			///  Used to keep track of which nodes we still have to explore
			/// </summary>
			ssuds::Stack<node*> mNodeStack;

			/// <summary>
			/// Keeps track of which node we are currently on (or null if we are done)
			/// </summary>
			node* mCurrentNode;

		public:
			/// <summary>
			/// Default constructor
			/// </summary>
			OrderedSetIterator() : mCurrentNode(nullptr)
			{
				// empty, on purpose
			}


			/// <summary>
			///  Constructor
			/// </summary>
			/// <param name="root">the starting node (or null for end iterators)</param>
			OrderedSetIterator(node* root)
			{
				mCurrentNode = root;
				while (mCurrentNode)
				{
					mNodeStack.push(mCurrentNode);
					mCurrentNode = mCurrentNode->mLeft;
				}
				if (!mNodeStack.empty())
				{
					mCurrentNode = mNodeStack.top();
					mNodeStack.pop();
				}
			}

			/// <summary>
			/// Gets the current value of the iterator
			/// </summary>
			/// <returns>Value held by the current node</returns>
			T& operator*()
			{
				if (mCurrentNode)
					return mCurrentNode->mData;
				else
					throw std::invalid_argument("iterator is not in a proper state!");
			}

			/// <summary>
			/// Advances the iteration
			/// </summary>
			void operator++()
			{
				mCurrentNode = mCurrentNode->mRight;
				while (mCurrentNode)
				{
					mNodeStack.push(mCurrentNode);
					mCurrentNode = mCurrentNode->mLeft;
				}

				if (!mNodeStack.empty())
				{
					mCurrentNode = mNodeStack.top();
					mNodeStack.pop();
				}
			}

			/// <summary>
			/// Returns true if two iterators are unlike each other
			/// </summary>
			/// <param name="other">the other iterator</param>
			/// <returns>true if they are unequal</returns>
			bool operator!=(const OrderedSetIterator& other)
			{
				return mCurrentNode != other.mCurrentNode;
			}
		};


	protected:
		/// <summary>
		/// A pointer to the root node, or nullptr if the tree is empty
		/// </summary>
		node* mRoot;

		/// <summary>
		/// Number of nodes in the set
		/// </summary>
		unsigned int mSize;

	public:

		/// Constructor
		OrderedSet() : mRoot(nullptr), mSize(0)
		{
			// empty, on purpose
		}


		/// <summary>
		/// Copy-constructor
		/// </summary>
		/// <param name="other">the set we wish to copy</param>
		OrderedSet(const OrderedSet& other) : mRoot(nullptr), mSize(0)
		{
			for (T val : other)
				insert(val);
		}


		/// <summary>
		/// The initializer-list constructor
		/// </summary>
		/// <param name="ilist">contains the initial data</param>
		OrderedSet(const std::initializer_list<T>& ilist) : mRoot(nullptr), mSize(0)
		{
			for (T val : ilist)
				insert(val);
		}


		/// <summary>
		/// The move-constructor
		/// </summary>
		/// <param name="other">the set we are stealing data from</param>
		OrderedSet(OrderedSet&& other) : mRoot(other.mRoot), mSize(other.mSize)
		{
			other.mRoot = nullptr;
			other.mSize = 0;
		}

		/// Destructor
		~OrderedSet()
		{
			clear();
		}


		/// <summary>
		/// A method to generate a text visualization of the tree in this form:
		/// [root-value]
		///    L: [root's-left-value]
		///        L: [root's lowest grandchild]
		///    R: [root's-right-value]
		/// </summary>
		/// <param name="os">a string-stream to write to</param>
		/// <param name="the_set">the set we want to output</param>
		/// <returns>a possibly modified ostream (not modified currently) </returns>
		friend std::ostream& operator<<(std::ostream& os, const OrderedSet& the_set)
		{
			ssuds::ArrayList<T> result = the_set.traversal(TraversalType::IN_ORDER);
			os << result;
			return os;
		}


		/// <summary>
		/// Makes us a copy of the other set
		/// </summary>
		/// <param name="other">the set we wish to copy</param>
		/// <returns>A reference to us</returns>
		OrderedSet& operator=(const OrderedSet& other)
		{
			clear();
			for (T item : other)
				insert(item);
			return *this;
		}


		/// <summary>
		/// Create an iterator that is "pointing" to the first (lowest) element
		/// </summary>
		/// <returns>A valid iterator if the set is non-empty, or the end iterator if not</returns>
		OrderedSetIterator begin() const
		{
			return OrderedSetIterator(mRoot);
		}



		/// <summary>
		///  Removes all elements from the set and returns it to its original state
		/// </summary>
		void clear()
		{
			delete mRoot;
			mRoot = nullptr;
			mSize = 0;
		}


		/// <summary>
		/// Determines if the given value is in the set or not
		/// </summary>
		/// <param name="val">value we're looking for</param>
		/// <returns>true if it's in the set, false if not</returns>
		bool contains(const T& val) const
		{
			if (mRoot)
				return mRoot->contains_recursive(val);
			else
				return false;
		}


		/// <summary>
		/// Returns a special iterator which marks the end of iteration
		/// </summary>
		/// <returns>An end iterator</returns>
		OrderedSetIterator end() const
		{
			return OrderedSetIterator(nullptr);
		}



		/// <summary>
		/// Removes an element (if it exists) from the set
		/// </summary>
		/// <param name="val">the value to remove</param>
		/// <returns>true if that item was remove, false if it wasn't in the set</returns>
		bool erase(const T& val)
		{
			bool result = false;

			if (mRoot)
			{
				node* new_root = mRoot->erase_recursive(val, result);
				if (new_root != mRoot)
				{
					mRoot->clear_children();
					delete mRoot;
					mRoot = new_root;
				}
			}
			if (result)
				mSize--;
			return result;
		}


		/// <summary>
		/// Gets the height of this tree (1 if we just have the root)
		/// </summary>
		/// <returns></returns>
		unsigned int get_height() const
		{
			if (mRoot)
				return mRoot->get_height_recursive();
			else
				return 0;
		}

		/// <summary>
		/// Inserts a new element into the set
		/// </summary>
		/// <param name="val">the new value to add</param>
		/// <returns>true if a new value was added</returns>
		bool insert(const T& val)
		{
			int num_added = 0;
			if (mRoot)
				num_added = mRoot->insert_recursive(val);
			else
			{
				mRoot = new node(val);
				num_added = 1;
			}
			mSize += num_added;
			return num_added > 0;
		}



		/// <summary>
		///  A very costly operation which makes this an optimal tree (lowest depth)
		/// </summary>
		void rebalance()
		{
			ssuds::ArrayList<T> data = traversal(TraversalType::IN_ORDER);
			clear();
			if (data.size() > 0)
				mRoot = rebalance_helper(data, 0, data.size() - 1);
			mSize = data.size();
		}

	protected:
		/// <summary>
		///  An internal method, structured something like binary search that is used to rebalance
		/// this tree
		/// </summary>
		/// <param name="data">an in-order collection of all data that was originally in the tree</param>
		/// <param name="left">the index of the starting value this new subtree should contain</param>
		/// <param name="right">the index of the ending value this new subtree should contain</param>
		/// <returns>an optimal root node that contains all the data in the given range.</returns>
		node* rebalance_helper(ssuds::ArrayList<T>& data, int left, int right)
		{
			if (left <= right)
			{
				int mid = (left + right) / 2;
				node* cur_root = new node(data[mid]);
				cur_root->mLeft = rebalance_helper(data, left, mid - 1);
				cur_root->mRight = rebalance_helper(data, mid + 1, right);
				return cur_root;
			}
			else
				return nullptr;
		}

	public:

		/// <summary>
		/// Returns the number of elements in a set
		/// </summary>
		/// <returns></returns>
		unsigned int size()
		{
			return mSize;
		}

		/// <summary>
		/// Used to create an in-order, pre-order, or post-order copy of all the data in this tree
		/// </summary>
		/// <param name="tp">desired type of traversal</param>
		/// <returns>a copy of all data</returns>
		ssuds::ArrayList<T> traversal(TraversalType tp) const
		{
			ssuds::ArrayList<T> results;
			if (mRoot)
				mRoot->traversal_recursive(results, tp);
			return results;
		}


		/// <summary>
		/// Generates a string of this form:
		/// Root-val
		///     L: left-child of root
		///     R: right-child of root
		/// </summary>
		/// <returns>the string from above</returns>
		std::string tree_string()
		{
			std::stringstream ss;
			if (mRoot)
				mRoot->generate_string_recursive(ss, 0, "");
			return ss.str();
		}
	};



	/// <summary>
	/// Returns the UNION of two sets
	/// </summary>
	/// <typeparam name="T">the type of the set</typeparam>
	/// <param name="A">set1</param>
	/// <param name="B">set2</param>
	/// <returns>The resulting union</returns>
	template <class T>
	OrderedSet<T> operator|(const OrderedSet<T>& A, const OrderedSet<T>& B)
	{
		OrderedSet<T> result = A;
		for (T item : B)
			result.insert(item);
		return result;
	}


	/// <summary>
	/// Returns the INTERSECTION of two sets
	/// </summary>
	/// <typeparam name="T">the type of the set</typeparam>
	/// <param name="A">set1</param>
	/// <param name="B">set2</param>
	/// <returns>The resulting intersection</returns>
	template <class T>
	OrderedSet<T> operator&(const OrderedSet<T>& A, const OrderedSet<T>& B)
	{
		OrderedSet<T> result;
		for (T item : B)
		{
			if (A.contains(item))
				result.insert(item);
		}
		return result;
	}


	/// <summary>
	/// Returns the RELATIVE DIFFERENCE of two sets
	/// </summary>
	/// <typeparam name="T">the type of the set</typeparam>
	/// <param name="A">set1</param>
	/// <param name="B">set2</param>
	/// <returns>The resulting difference</returns>
	template <class T>
	OrderedSet<T> operator-(const OrderedSet<T>& A, const OrderedSet<T>& B)
	{
		OrderedSet<T> result;
		for (T item : A)
		{
			if (!B.contains(item))
				result.insert(item);
		}
		return result;
	}
}
