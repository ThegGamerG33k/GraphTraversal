#include <gtest/gtest.h>
#include <stack.h>
#include <sstream>

#define DO_STACK_TESTS 0
#if DO_STACK_TESTS

TEST(StackTests, BasicOperations)
{
	ssuds::Stack<int> S;
	EXPECT_EQ(S.size(), 0u);
	S.push(42);
	S.push(43);
	S.push(44);
	S.push(45);
	EXPECT_EQ(S.size(), 4);
	EXPECT_EQ(S.top(), 45);
	EXPECT_EQ(S.pop(), 45);
	EXPECT_EQ(S.size(), 3);
	EXPECT_EQ(S.top(), 44);
	EXPECT_FALSE(S.empty());
	EXPECT_EQ(S.pop(), 44);
	EXPECT_EQ(S.size(), 2);
	EXPECT_FALSE(S.empty());
	S.clear();
	EXPECT_EQ(S.size(), 0);
	EXPECT_TRUE(S.empty());
	EXPECT_THROW(S.top(), std::out_of_range);
}


TEST(StackTests, StreamOverload)
{
	ssuds::Stack<int> S;
	S.push(10);
	S.push(11);
	S.push(12);
	std::stringstream ss;
	ss << S;
	EXPECT_EQ(ss.str(), "[10, 11, 12]");
	ss.str("");
	S.clear();
	ss << S;
	EXPECT_EQ(ss.str(), "[]");
}


TEST(StackTests, Iterator)
{
	ssuds::Stack<int> S;
	S.push(10);
	S.push(11);
	S.push(12);
	int index = 0;
	ssuds::LinkedList<int>::LinkedListIterator it = S.begin();
	while (it != S.end())
	{
		int val = *it;
		if (index == 0)
			EXPECT_EQ(val, 10);
		else if (index == 1)
			EXPECT_EQ(val, 11);
		else if (index == 2)
			EXPECT_EQ(val, 12);
		++it;
		if (index == 2)
			EXPECT_EQ(it, S.end());
		index++;
	}
}

#endif