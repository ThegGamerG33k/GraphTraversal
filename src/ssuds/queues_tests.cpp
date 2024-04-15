#include <gtest/gtest.h>
#include <queue.h>
#include <sstream>

#define DO_QUEUE_TESTS 0
#if DO_QUEUE_TESTS

TEST(QueueTests, BasicOperations)
{
	ssuds::Queue<int> Q;
	EXPECT_EQ(Q.size(), 0u);
	Q.enqueue(42);
	Q.enqueue(43);
	Q.enqueue(44);
	Q.enqueue(45);
	EXPECT_EQ(Q.size(), 4);
	EXPECT_EQ(Q.head(), 42);
	EXPECT_EQ(Q.dequeue(), 42);
	EXPECT_EQ(Q.size(), 3);
	EXPECT_EQ(Q.head(), 43);
	EXPECT_FALSE(Q.empty());
	EXPECT_EQ(Q.dequeue(), 43);
	EXPECT_EQ(Q.size(), 2);
	EXPECT_FALSE(Q.empty());
	Q.clear();
	EXPECT_EQ(Q.size(), 0);
	EXPECT_TRUE(Q.empty());
	EXPECT_THROW(Q.head(), std::out_of_range);
}


TEST(QueueTests, StreamOverload)
{
	ssuds::Queue<int> Q;
	Q.enqueue(10);
	Q.enqueue(11);
	Q.enqueue(12);
	std::stringstream ss;
	ss << Q;
	EXPECT_EQ(ss.str(), "[10, 11, 12]");
	ss.str("");
	Q.clear();
	ss << Q;
	EXPECT_EQ(ss.str(), "[]");
}


TEST(QueueTests, Iterator)
{
	ssuds::Queue<int> Q;
	Q.enqueue(10);
	Q.enqueue(11);
	Q.enqueue(12);
	int index = 0;
	ssuds::LinkedList<int>::LinkedListIterator it = Q.begin();
	while (it != Q.end())
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
			EXPECT_EQ(it, Q.end());
		index++;
	}
}

#endif