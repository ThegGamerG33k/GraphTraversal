#include <visual_graph.h>
#include <graph.h>
#include <unordered_map.h>
#include <string>
#include <map>


namespace ssuds
{
	class GraphTraversal
	{
	protected:
		
		misc::VisualGraph G;
	public:
		void breadth_first(misc::VisualGraph& graph, std::map<int, float>& traversal_map, int start_node)
		{
			ssuds::ArrayList<int> frontier = { start_node };
			traversal_map[start_node] = -1;
			while (frontier.size() > 0)
			{
				ssuds::ArrayList<int> new_frontier = {};

				for ( int i = 0; )
				{
					for (int neighbor = 0; graph.get_neighbors(neighbor) != nullptr; neighbor++)
					{
						if (neighbor not in traversal_map)
						{
							traversal_map[neighbor] = node;
							new_frontier.append(neighbor);
						}
					}
				}
				frontier = new_frontier;
			}
		}

		void depth_first(ssuds::VisualGraph& graph, std::map<X, X>& traversal_map, const N& start_node, const N& cur_node = nullptr)
		{
			const N& next_node = nullptr;
			if (cur_node == None)
			{
				traversal_map[start_node] = nullptr;
				
			}
			else
			{
				for(int neighbor = 0; )
			}
		}



	};
}