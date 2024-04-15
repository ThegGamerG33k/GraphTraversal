#pragma once
#include <array_list.h>
#include <random>

namespace ssuds
{
	/// <summary>
	/// Used to indicate, when sorting an ordered collection, whether we want small -> big (ASCENDING)
	/// or big -> small (DESCENDING)
	/// </summary>
	enum class SortOrder { ASCENDING, DESCENDING };

	/// <summary>
	/// A method meant to internally support quicksort.  This helper method does most of the
	/// work of actually sorting the list.
	/// </summary>
	/// <typeparam name="T">The type of datatype held by the ArrayList</typeparam>
	/// <param name="aList">The ArrayList we're sorting</param>
	/// <param name="type">The type of sort we're performing</param>
	/// <param name="left">The left-most index to consider</param>
	/// <param name="right">The right-most index to consider (inclusive)</param>
	/// <param name="op_count">Used to track the number of swaps</param>
	template <class T>
	int _quicksort_pivot(ArrayList<T>& aList, SortOrder type, int left, int right, unsigned long& op_count)
	{
		// Pick a value to pivot around.  Some authors choose the first element, but I don't
		// think that will perform well for an already-sorted list.  I generally pick the middle 
		// element.  Put this value in the last spot so we leave it alone for a bit
		int mid_index = (left + right) / 2;
		T pivot_val = aList[mid_index];
		aList[mid_index] = aList[right];
		aList[right] = pivot_val;

		// Scan indicies from left...right.  If we see a value less than the pivot value, swap
		// it with the swap_index (and add one to swap_index).  Once we reach the end, the swap index
		// will hold the index of the pivot_val (or one of those values if there is more than one).  Note
		// that the element at right should always trigger a swap since we put the pivot value there.
		int swap_index = left;
		for (int i = left; i <= right; i++)
		{
			if (pivot_val == aList[i] || _out_of_order(type, pivot_val, aList[i]))
			{
				++op_count;
				T temp = aList[i];
				aList[i] = aList[swap_index];
				aList[swap_index++] = temp;
			}
		}

		return swap_index - 1;
	}

	/// <summary>
	/// An internal support method used by quicksort.  This method calls pivot (which
	/// does most of the actual work of quicksort).  Then this method
	/// recursively calls quicksort on the two halves
	/// </summary>
	/// <typeparam name="T">The type of ArrayList we're working on</typeparam>
	/// <param name="alist">The ArrayList we're sorting</param>
	/// <param name="type">The type of sort we're trying to implement</param>
	/// <param name="left">The left-most index of the portion of alist we're modifying</param>
	/// <param name="right">The right-most index of the portion of alist we're modifying</param>
	/// <param name="op_count">used to track the total number of operations performed by quicksort</param>
	template <class T>
	void _quicksort_recursive(ArrayList<T>& aList, SortOrder type, int left, int right, unsigned long& op_count)
	{
		if (left >= right)
			return;
		int pivot_index = _quicksort_pivot(aList, type, left, right, op_count);
		_quicksort_recursive(aList, type, left, pivot_index - 1, op_count);
		_quicksort_recursive(aList, type, pivot_index + 1, right, op_count);
	}


	/// <summary>
	/// This method is very similar to the other _out_of_order implementation, but rather than 
	/// using the ArrayList and indices, a reference to two values (one / both of which might really
	/// exist within an ArrayList)
	/// </summary>
	/// <typeparam name="T">The type of ArrayList we're working on</typeparam>
	/// <param name="sort_type">The type of sort we're trying to implement</param>
	/// <param name="val1">The first value in the comparison</param>
	/// <param name="val2">The second value in the comparison</param>
	/// <returns></returns>
	template <class T>
	bool _out_of_order(SortOrder sort_type, const T& val1, const T& val2)
	{
		return ((sort_type == SortOrder::ASCENDING && val1 > val2) || (sort_type == SortOrder::DESCENDING && val1 < val2));
	}


	/// <summary>
	/// If we have a sorted ArrayList, we can find a particular value much more quickly using
	/// this binary search function (as opposed to a linear search, which is what the ArrayList::find
	/// method implements)
	/// </summary>
	/// <typeparam name="T">The type of ArrayList we're working on</typeparam>
	/// <param name="alist">The sorted ArrayList to search within</param>
	/// <param name="sort_order">The way alist is sorted</param>
	/// <param name="search_value">The value to search for</param>
	/// <param name="num_ops">If not nullptr, the number of comparisons performed is written here</param>
	/// <returns>The index of an occurrence of search_value (or -1 if none are present in alist)</returns>
	template <class T>
	int find_binary_search(const ArrayList<T>& alist, SortOrder sort_order, const T& search_value, unsigned long* num_ops = nullptr)
	{
		long comparisons = 0;
		int left = 0;
		int right = alist.size() - 1;
		while (left <= right)
		{
			int mid = (left + right) >> 1;			// divide by 2 (the average)
			comparisons++;
			if (alist[mid] == search_value)
			{
				if (num_ops != nullptr)
					*num_ops = comparisons;
				return mid;
			}
			else if (_out_of_order(sort_order, search_value, alist[mid]))
				left = mid + 1;
			else
				right = mid - 1;
		} 
		if (num_ops != nullptr)
			*num_ops = comparisons;
		return -1;
	}

	
	/// <summary>
	/// Performs an in-place quicksort operation, O(n log n), on alist
	/// </summary>
	/// <typeparam name="T">The type of ArrayList we're working on</typeparam>
	/// <param name="alist">the ArrayList we wish to sort</param>
	/// <param name="type">The type of sort to perform</param>
	/// <returns>The number of swaps performed while sorting</returns>
	template <class T>
	unsigned long quicksort(ArrayList<T>& alist, SortOrder type)
	{
		// Reference: https://en.wikipedia.org/wiki/Quicksort
		unsigned long op_count = 0;
		_quicksort_recursive(alist, type, 0, alist.size() - 1, op_count);
		return op_count;
	}


	/// <summary>
	/// Performs an in-place bubble-sort sort on the given ArrayList, O(n^2)
	/// </summary>
	/// <typeparam name="T">The type of ArrayList we're working on</typeparam>
	/// <param name="alist">the ArrayList we wish to sort</param>
	/// <param name="type">The type of sort to perform</param>
	/// <returns>The number of swaps performed during bubble-sort</returns>
	template <class T>
	long bubblesort(ArrayList<T>& alist, SortOrder type)
	{
		long swaps = 0;
		for (unsigned int z = 0; z < alist.size(); z++)
		{
			bool sorted = true;
			for (unsigned int i = 0; i < alist.size() - 1 - z; i++)
			{
				if (_out_of_order(type, alist[i], alist[i + 1]))
				{
					sorted = false;
					std::swap(alist[i], alist[i + 1]);
					swaps++;
				}
			}
			if (sorted)
				break;
		}
		return swaps;
	}


	/// <summary>
	/// Applies an in-place Fisher-Yates shuffle on the given ArrayList, using the <random> module
	/// </summary>
	/// <typeparam name="T">The type of ArrayList we're working on</typeparam>
	/// <param name="alist">the ArrayList we wish to sort</param>
	template <class T>
	void shuffle(ArrayList<T>& alist)
	{
		// Reference: https://www.cplusplus.com/reference/random/
		// Reference: https://en.wikipedia.org/wiki/Fisher%E2%80%93Yates_shuffle
		std::default_random_engine generator;
		for (int i = alist.size() - 1; i > 0; i--)
		{
			std::uniform_int_distribution<int> distribution(0, i);
			int j = distribution(generator);  // generates number in the range 0...i
			
			std::swap(alist[i], alist[j]);
		}
	}
}
