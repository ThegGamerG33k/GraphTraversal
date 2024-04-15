#include <gtest/gtest.h>
#include <array_list.h>
#include <array_list_utility.h>

#define DO_ARRAY_LIST_UTILITY_TESTS 0
#if DO_ARRAY_LIST_UTILITY_TESTS

TEST(ArrayListUtilityTests, BasicBubblesortTest)
{
	ssuds::ArrayList<float> s = { 5.5f, 3.3f, 7.7f, 4.4f, 2.2f, 8.8f, 6.6f };
	ssuds::ArrayList<float> t(s);
	float initial_values[] = { 5.5f, 3.3f, 7.7f, 4.4f, 2.2f, 8.8f, 6.6f };
	float sorted_values[] = { 2.2f, 3.3f, 4.4f, 5.5f, 6.6f, 7.7f, 8.8f };
	float rev_sorted_values[] = { 8.8f, 7.7f, 6.6f, 5.5f, 4.4f, 3.3f, 2.2f };
	for (int i = 0; i < 7; i++)
		ASSERT_EQ(s[i], initial_values[i]);
	ssuds::bubblesort(s, ssuds::SortOrder::ASCENDING);
	ssuds::bubblesort(t, ssuds::SortOrder::DESCENDING);
	ASSERT_EQ(s.size(), 7);
	ASSERT_EQ(t.size(), 7);
	for (int i = 0; i < 7; i++)
	{
		EXPECT_EQ(s[i], sorted_values[i]);
		EXPECT_EQ(t[i], rev_sorted_values[i]);
	}
}

TEST(ArrayListUtilityTests, BasicQuicksortTest)
{
	ssuds::ArrayList<float> s = { 5.5f, 3.3f, 7.7f, 4.4f, 2.2f, 8.8f, 6.6f };
	ssuds::ArrayList<float> t(s);
	float initial_values[] = { 5.5f, 3.3f, 7.7f, 4.4f, 2.2f, 8.8f, 6.6f };
	float sorted_values[] = { 2.2f, 3.3f, 4.4f, 5.5f, 6.6f, 7.7f, 8.8f };
	float rev_sorted_values[] = { 8.8f, 7.7f, 6.6f, 5.5f, 4.4f, 3.3f, 2.2f };
	for (int i = 0; i < 7; i++)
		ASSERT_EQ(s[i], initial_values[i]);
	ssuds::quicksort(s, ssuds::SortOrder::ASCENDING);
	ssuds::quicksort(t, ssuds::SortOrder::DESCENDING);
	ASSERT_EQ(s.size(), 7);
	ASSERT_EQ(t.size(), 7);
	for (int i = 0; i < 7; i++)
	{
		EXPECT_EQ(s[i], sorted_values[i]);
		EXPECT_EQ(t[i], rev_sorted_values[i]);
	}
}


TEST(ArrayListUtilityTests, AlreadySortedTest)
{
	ssuds::ArrayList<int> s;
	ssuds::ArrayList<int> t;
	const int num = 1000;

	for (int i = 0; i < num; i++)
	{
		s.append(i);
		t.append(i);
	}

	ssuds::bubblesort(s, ssuds::SortOrder::ASCENDING);
	ssuds::bubblesort(t, ssuds::SortOrder::ASCENDING);

	ASSERT_EQ(s.size(), num);
	ASSERT_EQ(t.size(), num);

	for (int i = 0; i < num; i++)
	{
		EXPECT_EQ(s[i], i);
		s[i] = num - i;
		EXPECT_EQ(t[i], i);
		t[i] = num - i;
	}

	ssuds::bubblesort(s, ssuds::SortOrder::DESCENDING);
	ssuds::bubblesort(t, ssuds::SortOrder::DESCENDING);

	ASSERT_EQ(s.size(), num);
	ASSERT_EQ(t.size(), num);

	for (int i = 0; i < num; i++)
	{
		EXPECT_EQ(s[i], num - i);
		EXPECT_EQ(t[i], num - i);
	}
}


TEST(ArrayListUtilityTests, Shuffle)
{
	// Shuffle is a bit hard to test since it's random.  We shouldn't, however
	// have any items that are in there original spots if we have a list of
	// at least size 2
	ssuds::ArrayList<int> s;
	const int num = 1000;
	for (int i = 0; i < num; i++)
		s.append(i);

	ssuds::shuffle(s);
	ASSERT_EQ(s.size(), num);

	for (int i = 0; i < num; i++)
		EXPECT_NE(s[i], i);
}


TEST(ArrayListUtilityTests, BiggerQuicksort)
{
	// Note: this test depends on proper shuffle behavior
	ssuds::ArrayList<int> s;
	const int num = 1000;
	for (int i = 0; i < num; i++)
		s.append(i);

	ssuds::shuffle(s);
	ssuds::ArrayList<int> t(s);

	ssuds::quicksort(s, ssuds::SortOrder::ASCENDING);
	ssuds::quicksort(t, ssuds::SortOrder::DESCENDING);

	for (int i = 0; i < num; i++)
	{
		EXPECT_EQ(s[i], i);
		EXPECT_EQ(t[i], num - 1 - i);
	}
}


TEST(ArrayListUtilityTests, BiggerBubblesort)
{
	// Note: this test depends on proper shuffle behavior
	ssuds::ArrayList<int> s;
	const int num = 1000;
	for (int i = 0; i < num; i++)
		s.append(i);

	ssuds::shuffle(s);
	ssuds::ArrayList<int> t(s);

	ssuds::bubblesort(s, ssuds::SortOrder::ASCENDING);
	ssuds::bubblesort(t, ssuds::SortOrder::DESCENDING);

	for (int i = 0; i < num; i++)
	{
		EXPECT_EQ(s[i], i);
		EXPECT_EQ(t[i], num - 1 - i);
	}
}


TEST(ArrayListUtilityTests, BinarySearch)
{
	ssuds::ArrayList<int> s, t;
	const int num = 10000;
	unsigned long int op_count;
	long int result;
	for (int i = 0; i < num; i++)
	{
		s.append(i);
		t.append(num - 1 - i);
	}

	for (int i = 0; i < num; i++)
	{
		result = ssuds::find_binary_search(s, ssuds::SortOrder::ASCENDING, i, &op_count);
		EXPECT_EQ(result, i);
		EXPECT_GE(op_count, 1u);
		result = ssuds::find_binary_search(t, ssuds::SortOrder::DESCENDING, i, &op_count);
		EXPECT_EQ(result, num - 1 - i);
		EXPECT_GE(op_count, 1u);
	}

	// Look for values not in the list
	result = ssuds::find_binary_search(s, ssuds::SortOrder::ASCENDING, -1);
	EXPECT_EQ(result, -1);
	result = ssuds::find_binary_search(s, ssuds::SortOrder::ASCENDING, num + 1);
	EXPECT_EQ(result, -1);

	result = ssuds::find_binary_search(t, ssuds::SortOrder::ASCENDING, -1);
	EXPECT_EQ(result, -1);
	result = ssuds::find_binary_search(t, ssuds::SortOrder::ASCENDING, num + 1);
	EXPECT_EQ(result, -1);
}


#endif