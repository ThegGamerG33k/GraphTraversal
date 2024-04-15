#define DO_GRAPH_TESTS 1
#if DO_GRAPH_TESTS

#include <gtest/gtest.h>
#include <graph.h>
#include <map>
#include <vector>

class GraphTests : public testing::Test
{
protected:
    void SetUp() override 
    {
        int_graph.add_node('a');
        int_graph.add_node('b');
        int_graph.add_node('c');
        int_graph.add_node('d');
        int_graph.add_node('e');
        int_graph.add_node('f');
        int_graph.set_edge('a', 'b', 15);
        int_graph.set_edge('d', 'e', 18);
        int_graph.set_edge('a', 'c', 32);
        int_graph.set_edge('c', 'a', 33);
    }

    ssuds::Graph<char, int> int_graph;
};

TEST_F(GraphTests, BasicTests)
{
    // See that we have all nodes
    for (char c : {'a', 'b', 'c', 'd', 'e', 'f'})
        ASSERT_TRUE(int_graph.contains_node(c));

    // See that we don't have a few other nodes
    for (char c : {'x', 'y', '?'})
        ASSERT_FALSE(int_graph.contains_node(c));

    // See that we have the edges we set
    ASSERT_TRUE(int_graph.contains_edge('a', 'b'));
    ASSERT_TRUE(int_graph.contains_edge('d', 'e'));
    ASSERT_TRUE(int_graph.contains_edge('a', 'c'));
    ASSERT_TRUE(int_graph.contains_edge('c', 'a'));

    // See that we have some edges that we shouldn't
    for (char c : {'d', 'e', 'a', 'f'})
        ASSERT_FALSE(int_graph.contains_edge('a', c));
    for (char c : {'b', 'c', 'd', 'e', 'f'})
        ASSERT_FALSE(int_graph.contains_edge('c', c));
    for (char c : {'a', 'b', 'c', 'd', 'f'})
        ASSERT_FALSE(int_graph.contains_edge('d', c));
    for (char c : {'a', 'b', 'c', 'd', 'e', 'f'})
    {
        ASSERT_FALSE(int_graph.contains_edge('b', c));
        ASSERT_FALSE(int_graph.contains_edge('e', c));
        ASSERT_FALSE(int_graph.contains_edge('e', c));
        ASSERT_FALSE(int_graph.contains_edge('f', c));
    } 
}

TEST_F(GraphTests, IteratorTests)
{
    std::map<char, std::map<char, bool>> required_neighbors = { 
        { 'a', {{'a', false}, {'b', true}, {'c', true}, {'d', false}, {'e', false}, {'f', false}}},
        {'b', {{'a', false}, {'b', false}, {'c', false}, {'d', false}, {'e', false}, {'f', false}}},
        {'c', {{'a', true}, {'b', false}, {'c', false}, {'d', false}, {'e', false}, {'f', false}}},
        {'d', {{'a', false}, {'b', false}, {'c', false}, {'d', false}, {'e', true}, {'f', false}}} ,
        {'e', {{'a', false}, {'b', false}, {'c', false}, {'d', false}, {'e', false}, {'f', false}}} ,
        {'f', {{'a', false}, {'b', false}, {'c', false}, {'d', false}, {'e', false}, {'f', false}}} 
    };
    
    std::map<char, bool> visited;
    for (char c : {'a', 'b', 'c', 'd', 'e', 'f'})
    {
        visited[c] = false;
        typename ssuds::UnorderedMap<char, ssuds::UnorderedMap<char, int>>::UnorderedMapIterator it = int_graph.find(c);
        EXPECT_NE(it, int_graph.end());
    }


    // Iterate through all nodes
    int count = 0;
    typename ssuds::UnorderedMap<char, ssuds::UnorderedMap<char, int>>::UnorderedMapIterator it = int_graph.begin();
    while (it != int_graph.end())
    {
        // Mark that we visited this node
        char start_node = (*it).first;
        visited[start_node] = true;

        // Iterate through all neighbors of that node
        typename ssuds::UnorderedMap<char, int>::UnorderedMapIterator inner_it = (*it).second.begin();
        while (inner_it != (*it).second.end())
        {
            char neighbor = (*inner_it).first;
            int edge_val = (*inner_it).second;
            required_neighbors[start_node][neighbor] = false;
            count++;
            ++inner_it;
        }
        ++it;
    }

    for (char i : {'a', 'b', 'c', 'd', 'e', 'f'})
    {
        EXPECT_TRUE(visited[i]);
        for (char j : {'a', 'b', 'c', 'd', 'e', 'f'})
            EXPECT_FALSE(required_neighbors[i][j]);
    }
}


TEST_F(GraphTests, OStream)
{
    std::map<char, std::vector<std::string>> possibilities;
    possibilities['a'] = { "a | (b:15) (c:32) \n", "a | (c:32) (b:15) \n" };
    possibilities['b'] = { "b | \n" };
    possibilities['c'] = { "c | (a:33) \n" };
    possibilities['d'] = { "d | (e:18) \n" };
    possibilities['e'] = { "e | \n" };
    possibilities['f'] = { "f | \n" };

    std::stringstream ss;
    ss << int_graph;
    std::string s = ss.str();
    int i = 0;
    int start_pos = 0;
    while (i < s.length())
    {
        if (s[i] == '\n')
        {
            std::string line = s.substr(start_pos, i - start_pos + 1);
            if (line.length() >= 3)
            {
                char c = line[0];
                bool found = false;
                for (std::string p : possibilities[c])
                {
                    if (p == line)
                        found = true;
                }
                EXPECT_TRUE(found);
            }
            start_pos = i+1;
        }
        i++;
    }
}

#endif