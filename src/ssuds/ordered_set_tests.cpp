#include <gtest/gtest.h>
#include <ordered_set.h>

#define DO_ORDERED_SET_TESTS 1
#if DO_ORDERED_SET_TESTS

class OrderedSetTests : public testing::Test
{
protected:
	void SetUp() override
	{
        s1.insert("Doug");
        s1.insert("Abe");
        s1.insert("Bob");
        s1.insert("Carl");
        s1.insert("Erin");
        s1.insert("Eric");
        s1.insert("Erica");
        s1.insert("Ericaz");
        s1.insert("Carline");
        s1.insert("Dan");
	}

	ssuds::OrderedSet<std::string> s1;
};


TEST_F(OrderedSetTests, BasicOperations)
{
    EXPECT_EQ(s1.size(), 10);
    for (std::string s : {"Doug", "Abe", "Bob", "Carl", "Dan", "Erin", "Eric", "Erica", "Ericaz", "Carline"})
    {
        EXPECT_FALSE(s1.insert(s));
        EXPECT_EQ(s1.size(), 10);
    }
    s1.clear();
    EXPECT_EQ(s1.size(), 0);
}


TEST_F(OrderedSetTests, RemoveAndContains)
{
    EXPECT_EQ(s1.size(), 10);
    // Items that shouldn't be removed
    for (std::string s : {"xyz", "aaa", "zzZ", "carl", "daN"})
    {
        EXPECT_FALSE(s1.contains(s));
        EXPECT_FALSE(s1.erase(s));
        EXPECT_EQ(s1.size(), 10);
    }
    int num = 0;
    for (std::string s : {"Doug", "Abe", "Bob", "Carl", "Dan", "Erin", "Eric", "Erica", "Ericaz", "Carline"})
    {
        EXPECT_TRUE(s1.contains(s));
        EXPECT_TRUE(s1.erase(s));
        num++;
        EXPECT_EQ(s1.size(), 10 - num);
    }
}


TEST_F(OrderedSetTests, TreeString)
{
    std::stringstream strue;
    strue << "Doug" << std::endl;
    strue << "   L: Abe" << std::endl;
    strue << "      R: Bob" << std::endl;
    strue << "         R: Carl" << std::endl;
    strue << "            R: Carline" << std::endl;
    strue << "               R: Dan" << std::endl;
    strue << "   R: Erin" << std::endl;
    strue << "      L: Eric" << std::endl;
    strue << "         R: Erica" << std::endl;
    strue << "            R: Ericaz" << std::endl;

    std::string s = s1.tree_string();
    EXPECT_EQ(s, strue.str());
}


TEST_F(OrderedSetTests, TraversalTest)
{
    ssuds::ArrayList<std::string> in_true {"Abe", "Bob", "Carl", "Carline", "Dan", "Doug", "Eric", "Erica", "Ericaz", "Erin"};
    ssuds::ArrayList<std::string> in_test = s1.traversal(ssuds::TraversalType::IN_ORDER);

    ssuds::ArrayList<std::string> pre_true {"Doug", "Abe", "Bob", "Carl", "Carline", "Dan", "Erin", "Eric", "Erica", "Ericaz"};
    ssuds::ArrayList<std::string> pre_test = s1.traversal(ssuds::TraversalType::PRE_ORDER);

    ssuds::ArrayList<std::string> post_true {"Dan", "Carline", "Carl", "Bob", "Abe", "Ericaz", "Erica", "Eric", "Erin", "Doug"};
    ssuds::ArrayList<std::string> post_test = s1.traversal(ssuds::TraversalType::POST_ORDER);

    ASSERT_EQ(in_true.size(), in_test.size());
    ASSERT_EQ(pre_true.size(), pre_test.size());
    ASSERT_EQ(post_true.size(), post_test.size());
    for (unsigned int i = 0; i < in_true.size(); i++)
    {
        EXPECT_EQ(in_test[i], in_true[i]);
        EXPECT_EQ(pre_test[i], pre_true[i]);
        EXPECT_EQ(post_test[i], post_true[i]);
    }
}


TEST_F(OrderedSetTests, IteratorTest)
{
    ssuds::ArrayList<std::string> in_true {"Abe", "Bob", "Carl", "Carline", "Dan", "Doug", "Eric", "Erica", "Ericaz", "Erin"};
    ssuds::OrderedSet<std::string>::OrderedSetIterator it;

    it = s1.begin();
    int i = 0;
    while (it != s1.end())
    {
        EXPECT_EQ(*it, in_true[i]);
        ++it;
        ++i;
    }

    EXPECT_EQ(i, in_true.size());
}


TEST_F(OrderedSetTests, Rebalance)
{
    EXPECT_EQ(s1.size(), 10);
    EXPECT_EQ(s1.get_height(), 6);
    s1.rebalance();
    EXPECT_EQ(s1.size(), 10);
    EXPECT_EQ(s1.get_height(), 4);

    std::stringstream strue;
    strue << "Dan" << std::endl;
    strue << "   L: Bob" << std::endl;
    strue << "      L: Abe" << std::endl;
    strue << "      R: Carl" << std::endl;
    strue << "         R: Carline" << std::endl;
    strue << "   R: Erica" << std::endl;
    strue << "      L: Doug" << std::endl;
    strue << "         R: Eric" << std::endl;
    strue << "      R: Ericaz" << std::endl;
    strue << "         R: Erin" << std::endl;

    std::string s = s1.tree_string();
    EXPECT_EQ(s, strue.str());
}


TEST_F(OrderedSetTests, SetOperations)
{
    ssuds::OrderedSet<int> set1{ 1, 5, 7, 3, 2, 5, 9, 13 };
    ssuds::OrderedSet<int> set2{ 5, 2, 13, 99, 73 };

    ssuds::OrderedSet<int> union_result;
    ssuds::ArrayList<int> union_true {1, 5, 7, 3, 2, 9, 13, 99, 73};
    union_result = set1 | set2;

    EXPECT_EQ(union_result.size(), union_true.size());
    for (int v : union_true)
        EXPECT_TRUE(union_result.contains(v));

    ssuds::OrderedSet<int> intersect_result;
    ssuds::ArrayList<int> intersect_true {5, 2, 13};
    intersect_result = set1 & set2;

    EXPECT_EQ(intersect_result.size(), intersect_true.size());
    for (int v : intersect_true)
        EXPECT_TRUE(intersect_result.contains(v));

    ssuds::OrderedSet<int> diff_result;
    ssuds::ArrayList<int> diff_true {1, 7, 3, 9};
    diff_result = set1 - set2;

    EXPECT_EQ(diff_result.size(), diff_true.size());
    for (int v : diff_true)
        EXPECT_TRUE(diff_result.contains(v));

}


#endif