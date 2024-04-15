#pragma once
#include <linked_list.h>

namespace ssuds
{
	/// This Stack classes uses inheritance (from LinkedList) to create a LIFO structure (Last-in-first-out)
	template <class T>
	class Stack : private LinkedList<T>
	{
	public:
		/// <summary>
		///  Adds a new element to the top of the stack
		/// </summary>
		/// <param name="val">the value to add to the stack</param>
		void push(const T& val)
		{
			this->append(val);
		}


		/// <summary>
		/// Removes the top element from the stack and returns it.  This method will raise an out_of_range exception if the 
		/// stack is empty
		/// </summary>
		/// <returns></returns>
		T pop()
		{
			// See https://isocpp.org/wiki/faq/templates#nondependent-name-lookup-members for a description of why we need this-> in front
			// of all base-class members
			if (this->mSize == 0)
				throw std::out_of_range("You cannot pop from an empty stack");
			T return_val = this->mEnd->mData;
			this->remove(this->rbegin());
			return return_val;
		}


		/// <summary>
		/// Returns a references to the top element (but does not remove it).  This method will throw an exception if the stack is empty.
		/// </summary>
		/// <returns></returns>
		T& top()
		{
			if (this->mSize == 0)
				throw std::out_of_range("You cannot get the top of an empty queue");
			return this->mEnd->mData;
		}


		/// <summary>
		/// Returns true if the stack is empty
		/// </summary>
		/// <returns></returns>
		bool empty()
		{
			return this->mSize == 0;
		}

		using LinkedList<T>::clear;
		using LinkedList<T>::begin;
		using LinkedList<T>::end;
		using LinkedList<T>::size;

		friend std::ostream& operator<<(std::ostream& os, const Stack& S)
		{
			os << dynamic_cast<const LinkedList<T> &>(S);

			return os;
		}
	};
}

