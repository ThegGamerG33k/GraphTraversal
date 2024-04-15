#include <gtest/gtest.h>
#include <array_list.h>

#define EXECUTE_ARRAY_LIST_TESTS 0
#if EXECUTE_ARRAY_LIST_TESTS


TEST(ArrayListTests, InitiallyEmpty)
{
	ssuds::ArrayList<float> slist1;
	EXPECT_EQ(slist1.size(), 0);
}


// Tests many of the low-level operations we need for most other tests
TEST(ArrayListTests, FoundationalItems)
{
	ssuds::ArrayList<int> slist1;
	EXPECT_NO_THROW(slist1.append(10));
	EXPECT_NO_THROW(slist1.append(11));
	EXPECT_NO_THROW(slist1.append(12));
	ASSERT_EQ(slist1.size(), 3);
	EXPECT_EQ(slist1[0], 10);
	EXPECT_EQ(slist1[1], 11);
	EXPECT_EQ(slist1[2], 12);
	EXPECT_THROW(slist1[3], std::out_of_range);
	slist1.clear();
	EXPECT_EQ(slist1.size(), 0);
}

// Test to make sure grow is properly being handled
TEST(ArrayListTests, ManyAppends)
{
	const int num = 1000;
	const int base = 10;
	ssuds::ArrayList<int> s;
	for (int i = 0; i < num; i++)
		EXPECT_NO_THROW(s.append(i + base));
	EXPECT_EQ(s.size(), num);
	for (int i = 0; i < num; i++)
		EXPECT_EQ(s[i], i + base);
	EXPECT_GE(s.capacity(), s.size());
}

TEST(ArrayListTests, Insert)
{
	ssuds::ArrayList<int> s;
	s.insert(10, 0);
	s.insert(15, 1);
	s.insert(9, 0);
	s.insert(14, 2);
	s.insert(11, 2);
	s.insert(12, 3);
	s.insert(13, 4);
	s.insert(16, 7);
	for (unsigned int i = 0; i <= 7; i++)
		EXPECT_EQ(s[i], 9 + i);
	
	EXPECT_EQ(s.size(), 8);
	EXPECT_GE(s.capacity(), s.size());
}

TEST(ArrayListTests, StringStream)
{
	ssuds::ArrayList<int> s;
	std::stringstream ss;
	ss << s;
	EXPECT_EQ(ss.str(), "[]");
	s.append(5);
	ss.str("");
	ss << s;
	EXPECT_EQ(ss.str(), "[5]");
	s.append(6);
	s.append(7);
	s.append(8);
	ss.str("");
	ss << s;
	EXPECT_EQ(ss.str(), "[5, 6, 7, 8]");
}

TEST(ArrayListTests, CopyAndEqual)
{
	ssuds::ArrayList<int> s;
	s.append(5);
	s.append(6);
	s.append(7);

	ssuds::ArrayList<int> t(s);
	EXPECT_EQ(t.size(), s.size());
	for (int i = 0; i < 3; i++)
		EXPECT_EQ(s[i], t[i]);

	ssuds::ArrayList<int> u = s;
	EXPECT_EQ(u.size(), s.size());
	for (int i = 0; i < 3; i++)
		EXPECT_EQ(s[i], u[i]);

	s.append(8);
	s[0] = 12;
	EXPECT_NE(t.size(), s.size());
	EXPECT_NE(u.size(), s.size());
	EXPECT_EQ(s[0], 12);
	EXPECT_EQ(t[0], 5);

	t.append(9);
	EXPECT_EQ(t.size(), s.size());
	EXPECT_EQ(t[3], 9);
	EXPECT_EQ(s[3], 8);

	t = s;
	EXPECT_EQ(t.size(), s.size());
	for (int i = 0; i < 4; i++)
		EXPECT_EQ(t[i], s[i]);
}

TEST(ArrayListTests, Find)
{
	ssuds::ArrayList<int> a;
	a.append(5);
	a.append(6);
	a.append(5);
	a.append(7);
	a.append(5);
	a.append(5);
	a.append(8);
	a.append(9);

	int result = a.find(5);
	EXPECT_EQ(result, 0);
	result = a.find(5, 1);
	EXPECT_EQ(result, 2);
	result = a.find(5, 2);
	EXPECT_EQ(result, 2);
	result = a.find(5, 3);
	EXPECT_EQ(result, 4);
	result = a.find(5, 4);
	EXPECT_EQ(result, 4);
	result = a.find(5, 5);
	EXPECT_EQ(result, 5);
	result = a.find(5, 6);
	EXPECT_EQ(result, -1);
	EXPECT_THROW(a.find(5, 99), std::out_of_range);

	EXPECT_EQ(a.find(2000), -1);
}


TEST(ArrayListTests, Remove)
{
	ssuds::ArrayList<int> a;
	a.append(5);
	a.append(6);
	a.append(5);
	a.append(7);
	a.append(5);
	a.append(5);
	a.append(8);
	a.append(9);

	EXPECT_EQ(a.size(), 8);
	EXPECT_EQ(a.remove(0), 5);
	EXPECT_EQ(a.size(), 7);
	EXPECT_EQ(a.remove(0), 6);
	EXPECT_EQ(a.size(), 6);
	EXPECT_EQ(a.remove(5), 9);
	EXPECT_EQ(a.size(), 5);

	EXPECT_THROW(a.remove(500), std::out_of_range);
}


TEST(ArrayListTests, RemoveAll)
{
	ssuds::ArrayList<int> a;
	a.append(5);
	a.append(6);
	a.append(5);
	a.append(7);
	a.append(5);
	a.append(5);
	a.append(8);
	a.append(9);

	EXPECT_EQ(a.size(), 8);
	EXPECT_EQ(a.remove_all(5), 4);
	EXPECT_EQ(a.size(), 4);
	EXPECT_EQ(a[0], 6);
	EXPECT_EQ(a[1], 7);
	EXPECT_EQ(a[2], 8);
	EXPECT_EQ(a[3], 9);
}


TEST(ArrayListTests, EmptyManualIteration)
{
	ssuds::ArrayList<int> slist1;
	ssuds::ArrayList<int>::ArrayListIterator it = slist1.begin();
	ssuds::ArrayList<int>::ArrayListIterator it_end = slist1.end();
	ASSERT_EQ(it, it_end);
}


TEST(ArrayListTests, ManualIteration)
{
	ssuds::ArrayList<std::string> slist2;
	slist2.append("A");
	slist2.append("B");
	slist2.append("C");
	slist2.append("D");
	ssuds::ArrayList<std::string>::ArrayListIterator it = slist2.begin();
	ssuds::ArrayList<std::string>::ArrayListIterator it_end = slist2.end();
	std::string val1, val2, val3, val4;
	ASSERT_NE(it, it_end);
	val1 = *it;
	++it;
	EXPECT_EQ(val1, "A");
	val2 = *it;
	++it;
	val3 = *it;
	++it;
	val4 = *it;
	++it;
	EXPECT_EQ(val2, "B");
	EXPECT_EQ(val3, "C");
	EXPECT_EQ(val4, "D");
	EXPECT_EQ(it, it_end);
}

TEST(ArrayListTests, ReverseIteration)
{
	ssuds::ArrayList<std::string> slist2;
	slist2.append("A");
	slist2.append("B");
	slist2.append("C");
	slist2.append("D");
	ssuds::ArrayList<std::string>::ArrayListIterator it = slist2.rbegin();
	ssuds::ArrayList<std::string>::ArrayListIterator it_end = slist2.rend();
	std::string val1, val2, val3, val4;
	ASSERT_NE(it, it_end);
	val1 = *it;
	++it;
	EXPECT_EQ(val1, "D");
	val2 = *it;
	++it;
	val3 = *it;
	++it;
	val4 = *it;
	++it;
	EXPECT_EQ(val2, "C");
	EXPECT_EQ(val3, "B");
	EXPECT_EQ(val4, "A");
	EXPECT_EQ(it, it_end);

	// Make sure r/not-r object interact right
	it = slist2.rbegin();
	it_end = slist2.rend();
	ssuds::ArrayList<std::string>::ArrayListIterator fit = slist2.begin();
	ssuds::ArrayList<std::string>::ArrayListIterator fit_end = slist2.end();
	EXPECT_NE(it, fit_end);
	EXPECT_NE(it_end, fit);
}

TEST(ArrayListTests, PrettyIteration)
{
	ssuds::ArrayList<std::string> slist2;
	slist2.append("A");
	slist2.append("B");
	slist2.append("C");
	slist2.append("D");
	int i = 0;
	for (std::string s : slist2)
	{
		if (i == 0)
			EXPECT_EQ(s, "A");
		else if (i == 1)
			EXPECT_EQ(s, "B");
		else if (i == 2)
			EXPECT_EQ(s, "C");
		else
			EXPECT_EQ(s, "D");
		i++;
	}
}



TEST(ArrayListTests, CopyConstructor)
{
	ssuds::ArrayList<std::string> slist2;
	slist2.append("A");
	slist2.append("B");
	slist2.append("C");
	slist2.append("D");

	ssuds::ArrayList<std::string> slist_copy1 = slist2;
	ssuds::ArrayList<std::string> slist_copy2(slist2);
	ASSERT_EQ(slist_copy1.size(), 4);
	EXPECT_EQ(slist_copy2[0], "A");
	EXPECT_EQ(slist_copy2[1], "B");
	EXPECT_EQ(slist_copy2[2], "C");
	EXPECT_EQ(slist_copy2[3], "D");
	slist2.remove(0);
	EXPECT_EQ(slist_copy1.size(), 4);

	// Same thing, really, but just to make sure...
	ASSERT_EQ(slist_copy2.size(), 4);
	EXPECT_EQ(slist_copy2[0], "A");
	EXPECT_EQ(slist_copy2[1], "B");
	EXPECT_EQ(slist_copy2[2], "C");
	EXPECT_EQ(slist_copy2[3], "D");
	EXPECT_EQ(slist_copy2.size(), 4);
}


TEST(ArrayListTests, Assignment)
{
	ssuds::ArrayList<std::string> slist2;
	slist2.append("A");
	slist2.append("B");
	slist2.append("C");
	slist2.append("D");

	ssuds::ArrayList<std::string> slist3;
	slist3.append("X");
	ASSERT_EQ(slist3.size(), 1);
	ASSERT_EQ(slist3[0], "X");
	slist3 = slist2;
	ASSERT_EQ(slist3.size(), 4);
	EXPECT_EQ(slist3[0], "A");
	EXPECT_EQ(slist3[1], "B");
	EXPECT_EQ(slist3[2], "C");
	EXPECT_EQ(slist3[3], "D");
	slist2.remove(0);
	EXPECT_EQ(slist3.size(), 4);
}


TEST(ArrayListTests, InitializerListConstructor)
{
	ssuds::ArrayList<int> test{ 5, 8, 9, 3, 1, 2, 7, 0 };
	ASSERT_EQ(test.size(), 8);
	EXPECT_EQ(test[0], 5);
	EXPECT_EQ(test[1], 8);
	EXPECT_EQ(test[2], 9);
	EXPECT_EQ(test[3], 3);
	EXPECT_EQ(test[4], 1);
	EXPECT_EQ(test[5], 2);
	EXPECT_EQ(test[6], 7);
	EXPECT_EQ(test[7], 0);
}


ssuds::ArrayList<float> move_func()
{
	ssuds::ArrayList<float> result;
	result.append(3.1f);
	result.append(4.2f);
	return result;
}

TEST(ArrayListTests, MoveConstructor)
{
	ssuds::ArrayList<float> my_arr = move_func();
	ASSERT_EQ(my_arr.size(), 2);
	EXPECT_EQ(my_arr[0], 3.1f);
	EXPECT_EQ(my_arr[1], 4.2f);
	my_arr.remove(0);
	EXPECT_EQ(my_arr.size(), 1);
}


TEST(ArrayListTests, SimpleIterator)
{
	ssuds::ArrayList<int> a;
	ssuds::ArrayList<int>::ArrayListIterator it = a.begin();
	EXPECT_EQ(it, a.end());
	a.append(14);
	it = a.begin();
	++it;
	EXPECT_EQ(it, a.end());
}

#endif