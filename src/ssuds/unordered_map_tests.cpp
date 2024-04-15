#include <gtest/gtest.h>
#include <unordered_map.h>

class UnorderedMapTestFixture : public ::testing::Test
{
protected:
	void SetUp() override
	{
		um["Bob"] = 42;
		um["Sue"] = 99;
		um["Carl"] = -1;
	}

	void TearDown() override
	{

	}


	ssuds::UnorderedMap<std::string, int> um;
	ssuds::UnorderedMap<std::string, int> um_empty;
};

TEST_F(UnorderedMapTestFixture, InitiallyEmpty)
{
	EXPECT_EQ(um_empty.size(), 0);
}


TEST_F(UnorderedMapTestFixture, AddingElements)
{
	EXPECT_EQ(um_empty.size(), 0);
	size_t cap = um_empty.capacity();
	int i = 0;
	while ((float)i / cap < MAX_LOAD_FACTOR)
	{
		um_empty[std::to_string(i)] = i;
		i++;
	}

	EXPECT_EQ(um_empty.size(), i);
	EXPECT_GT(um_empty.capacity(), cap);
}




TEST_F(UnorderedMapTestFixture, IteratorTests)
{
	um_empty["a"] = 4;
	um_empty["b"] = 6;
	um_empty["c"] = 3;
	int total = 0;
	// Note: we can't count on the values being in any certain order!
	ssuds::UnorderedMap<std::string, int>::UnorderedMapIterator it = um_empty.begin();
	while (it != um_empty.end())
	{
		total += (*it).second;
		++it;
	}

	EXPECT_EQ(total, 13);

	it = um_empty.find("b");
	EXPECT_EQ((*it).first, "b");
	EXPECT_EQ((*it).second, 6);

	it = um_empty.find("x");
	EXPECT_EQ(it, um_empty.end());
}


TEST_F(UnorderedMapTestFixture, Constructors)
{
	um_empty = um;
	ssuds::UnorderedMap<std::string, int>::UnorderedMapIterator it = um_empty.find("Bob");
	EXPECT_NE(it, um_empty.end());
	it = um_empty.find("Sue");
	EXPECT_NE(it, um_empty.end());
	it = um_empty.find("Carl");
	EXPECT_NE(it, um_empty.end());

	// If we add to the original, it should not be in the copy (i.e. we didn't make a shallow copy)
	um["Xavier"] = 99;
	it = um_empty.find("Xavier");
	EXPECT_EQ(it, um_empty.end());

	// Do the same thing, but using the copy-constructor
	ssuds::UnorderedMap<std::string, int> um2(um);
	it = um2.find("Bob");
	EXPECT_NE(it, um2.end());
	it = um2.find("Sue");
	EXPECT_NE(it, um2.end());
	it = um2.find("Carl");
	EXPECT_NE(it, um2.end());
	it = um2.find("Xavier");
	EXPECT_NE(it, um2.end());
	
	um["Yolanda"] = 100;
	it = um2.find("Yolanda");
	EXPECT_EQ(it, um2.end());
}


TEST_F(UnorderedMapTestFixture, Clear)
{
	EXPECT_EQ(um.size(), 3);
	um.clear();
	EXPECT_EQ(um.size(), 0);
}

		
TEST_F(UnorderedMapTestFixture, StreamOperator)
{
	// Remember, with an unordered list, we have no guarantees about order.
	// Here I'm seeing if we get ONE of the possibly 6 strings from um
	int count = 0;
	std::stringstream ss;
	ss << um;
	std::string result = ss.str();

	/*um["Bob"] = 42;
		um["Sue"] = 99;
		um["Carl"] = -1;*/

	std::vector<std::string> possibilities = { "{Bob:42, Sue:99, Carl:-1}",
											  "{Bob:42, Carl:-1, Sue:99}",
											  "{Sue:99, Carl:-1, Bob:42}",
											  "{Sue:99, Bob:42, Carl:-1}",
											  "{Carl:-1, Bob:42, Sue:99}",
											  "{Carl:-1, Sue:99, Bob:42}" };
	for (std::string s : possibilities)
	{
		if (s == result)
			count++;
	}

	EXPECT_EQ(count, 1);
}



TEST_F(UnorderedMapTestFixture, Contains)
{
	std::pair<std::string, int>p;

	ssuds::UnorderedMap<std::string, int>::UnorderedMapIterator it = um.find("Bob");
	p.first = "Bob";
	p.second = 42;
	EXPECT_EQ(*it, p);

	it = um.find("Sue");
	p.first = "Sue";
	p.second = 99;
	EXPECT_EQ(*it, p);

	it = um.find("Carl");
	p.first = "Carl";
	p.second = -1;
	EXPECT_EQ(*it, p);

	it = um.find("Xavier");
	EXPECT_EQ(it, um.end());
}
		


TEST_F(UnorderedMapTestFixture, Remove)
{
	ssuds::UnorderedMap<std::string, int>::UnorderedMapIterator it = um.find("Bob");

	EXPECT_NE(it, um.end());
	um.remove("Bob");
	EXPECT_EQ(um.size(), 2);
	it = um.find("Bob");
	EXPECT_EQ(it, um.end());
}
