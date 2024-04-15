#pragma once
#include <linked_list.h>

namespace ssuds
{
	/// <summary>
	/// This Queue classes uses LinkedList in a "has-a" relationship to implement a FIFO structure (First-in-first-out)
	/// </summary>
	/// <typeparam name="T">The type of all elements in the queue</typeparam>
	template <class T>
	class Queue
	{
	protected:
		LinkedList<T> mInternalList;

	public:
		/// <summary>
		/// Shallow wrapper around the underlying linked list clear method
		/// </summary>
		void clear()
		{
			mInternalList.clear();
		}

		/// <summary>
		/// Adds a new element to the tail of the queue
		/// </summary>
		/// <param name="val">the new element to add</param>
		void enqueue(const T& val)
		{
			mInternalList.append(val);
		}


		/// <summary>
		/// Removes and returns the head element in the queue.  This method will raise an exception if the queue is empty.
		/// </summary>
		/// <returns>The (old) head element</returns>
		T dequeue()
		{
			if (mInternalList.size() == 0)
				throw std::out_of_range("You cannot dequeue from an empty queue");
			// See https://pages.cs.wisc.edu/~driscoll/typename.html for a discussion of why the "typename" keyword is necessary here.  If you
			// don't include it, you get an error like this (in Visual Studio 2022):
			// error C3878: syntax error: unexpected token 'identifier' following 'expression'
			typename LinkedList<T>::LinkedListIterator it = mInternalList.begin();
			T return_val = *it;
			mInternalList.remove(it);
			return return_val;
		}


		/// <summary>
		/// Returns a reference to the head element without removing it
		/// </summary>
		/// <returns>A reference to the head element</returns>
		T& head() const
		{
			if (mInternalList.size() == 0)
				throw std::out_of_range("You cannot get the top of an empty queue");
			return mInternalList[0];
		}


		/// <summary>
		/// Returns true if the Queue is empty
		/// </summary>
		/// <returns>a boolean (is the queue empty?)</returns>
		bool empty() const
		{
			return mInternalList.size() == 0;
		}


		/// <summary>
		/// A shallow wrapper around LinkedList's ostream operator
		/// </summary>
		/// <param name="os">the output stream</param>
		/// <param name="Q">the queue</param>
		/// <returns></returns>
		friend std::ostream& operator<<(std::ostream& os, const Queue& Q)
		{
			os << Q.mInternalList;
			return os;
		}


		/// <summary>
		/// A shallow wrapper around LinkedList's begin method
		/// </summary>
		/// <returns>A LinkedListIterator capable of walking through all values in this queue</returns>
		typename LinkedList<T>::LinkedListIterator begin()
		{
			return mInternalList.begin();
		}

		/// <summary>
		/// A shallow wrapper around LinkedList's end method
		/// </summary>
		/// <returns>A LinkedListIterator end value</returns>
		typename LinkedList<T>::LinkedListIterator end()
		{
			return mInternalList.end();
		}


		/// <summary>
		/// Returns the size of the internal list.
		/// </summary>
		/// <returns>number of elements in the queue</returns>
		unsigned int size()
		{
			return mInternalList.size();
		}
	};
}
