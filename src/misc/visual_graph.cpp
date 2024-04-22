#include <visual_graph.h>


misc::VisualGraph::VisualGraph(const sf::Font& circle_font, const std::string& fname) : mCircleFont(circle_font)
{
	load(fname);
}






void misc::VisualGraph::draw(sf::RenderWindow& rw)
{
	// Draw edges
	sf::VertexArray lines(sf::LinesStrip, 2);
	ssuds::UnorderedMap<int, ssuds::UnorderedMap<int, float>>::UnorderedMapIterator outer_it = mInternalGraph.begin();
	while (outer_it != mInternalGraph.end())
	{
		ssuds::UnorderedMap<int, float>::UnorderedMapIterator inner_it = (*outer_it).second.begin();
		while (inner_it != (*outer_it).second.end())
		{
			std::pair<int, float> p = *inner_it;

			// See if this is a bi-directional edge or not
			bool bidirectional = false;
			ssuds::UnorderedMap<int, ssuds::UnorderedMap<int, float>>::UnorderedMapIterator test_it = mInternalGraph.find(p.first);
			if (test_it != mInternalGraph.end())
			{
				ssuds::UnorderedMap<int, float>::UnorderedMapIterator test_it2 = (*test_it).second.find((*outer_it).first);
				if (test_it2 != (*test_it).second.end())
					bidirectional = true;
			}
			if (bidirectional)
			{
				lines[0].color = lines[1].color = sf::Color::White;

			}
			else
			{
				lines[0].color = sf::Color(0u, 0u, 255u, 255u);
				lines[1].color = sf::Color::White;
			}
			lines[0].position = mCircleData[(*outer_it).first].get_position();
			lines[1].position = mCircleData[p.first].get_position();
			rw.draw(lines);
			++inner_it;
		}
		++outer_it;
	}

	// Draw nodes
	ssuds::UnorderedMap<int, sf::TextCircle>::UnorderedMapIterator it = mCircleData.begin();
	while (it != mCircleData.end())
	{
		std::pair<int, sf::TextCircle> p = *it;
		rw.draw(p.second);
		++it;
	}
}



void misc::VisualGraph::load(const std::string& fname)
{
	// Clear out any existing data
	mInternalGraph.clear();
	mCircleData.clear();

	std::ifstream fp(fname);
	if (fp.is_open())
	{
		while (true)
		{
			std::string datatype;
			std::getline(fp, datatype, ':');
			if (fp.eof())
				break;
			if (datatype == "n")
			{
				// Format=>     n:id:name:red:green:blue:x:y
				int red, green, blue;
				float x, y;
				int id;
				std::string name;
				fp >> id;
				std::getline(fp, name, ':');		// Consumes the ':'
				std::getline(fp, name, ':');
				fp >> red;
				fp.ignore(1);
				fp >> green;
				fp.ignore(1);
				fp >> blue;
				fp.ignore(1);
				fp >> x;
				fp.ignore(1);
				fp >> y;
				fp.ignore(1);
				mCircleData[id] = sf::TextCircle(x, y, mCircleFont, name);
				mCircleData[id].set_circle_color(sf::Color((sf::Uint8)red, (sf::Uint8)green, (sf::Uint8)blue));
				mInternalGraph.add_node(id);
			}
			else if (datatype == "e")
			{
				// Format=>     e:id_start:id_end:edge_value
				float d;
				int id_start, id_end;
				fp >> id_start;
				fp.ignore(1);
				fp >> id_end;
				fp.ignore(1);
				fp >> d;
				fp.ignore(1);
				if (mInternalGraph.contains_node(id_start) && mInternalGraph.contains_node(id_end))
					mInternalGraph.set_edge(id_start, id_end, d);
			}
		}
	}
}

void misc::VisualGraph::print_trav(std::map<int, float>& traversal_map)
{
	std::map<int, float>::iterator mit = traversal_map.begin();
	while (mit != traversal_map.end())
	{
		std::cout << (*mit).first << ": " << (*mit).second << std::endl;
		++mit;
	}
}


/// <summary>
/// This function will perform breadth first traversal for the given set of nodes and edges. It will start from the 
/// starting node and move one pass of edges each time until all branches are reached
/// </summary>
/// <param name="traversal_map">A temporary map to hold the travel history of the function</param>
/// <param name="start_node">Tells the function what node to start at</param>
void misc::VisualGraph::breadth_first(std::map<int, float>& traversal_map, int start_node)
{
	ssuds::ArrayList<int> frontier = { start_node };
	traversal_map[start_node] = -1;
	while (frontier.size() > 0)
	{
		ssuds::ArrayList<int> new_frontier = {};

		for (unsigned int i = 0; i < frontier.size(); i++)
		{
			ssuds::UnorderedMap<int, ssuds::UnorderedMap<int, float>>::UnorderedMapIterator it = mInternalGraph.find(frontier[i]);
			if (it != mInternalGraph.end())
			{
				int cur_node = frontier[i];
				for (std::pair<int, float> p : (*it).second)
				{
					int id = p.first;
					if (traversal_map.find(id) == traversal_map.end())
					{
						traversal_map[id] = cur_node;
						std::cout << cur_node;
						new_frontier.append(id);
					}
				}
			}
		}
		frontier = new_frontier;
	}
	print_trav(traversal_map);
}

/// <summary>
/// This function performs a depth first movement through the given set of nodes and edges. This function
/// will move through each node recursively until it reaches a node with no edges to move through. Then it
/// will step back and explore any branches that were not explored through the recursive calls.
/// </summary>
/// <param name="traversal_map">A temporary map to hold the travel history of the function</param>
/// <param name="start_node">tells the function where to begin</param>
/// <param name="cur_node">a dummy value that tells the function where it currently is</param>
void misc::VisualGraph::depth_first(std::map<int, float>& traversal_map, int start_node, int cur_node = -1)
{
	if (cur_node == -1)
	{
		traversal_map[start_node] = -1;
		depth_first(traversal_map, start_node, start_node);
	}
	else
	{
		ssuds::UnorderedMap<int, ssuds::UnorderedMap<int, float>>::UnorderedMapIterator it = mInternalGraph.find(cur_node);
		if (it != mInternalGraph.end())
		{
			for (std::pair<int, float> p : (*it).second)
			{
				int id = p.first;
				if (traversal_map.find(id) == traversal_map.end())
				{
					traversal_map[id] = cur_node;
					depth_first(traversal_map, start_node, id);
				}
			}
		}
	}
	print_trav(traversal_map);
}

/// <summary>
/// This function checks to see where the mouse is positioned on the screen and checks to see if it is located within one of the nodes
/// on click using the point_inside function
/// </summary>
/// <param name="mouse_pos">the mouse position as passed through sfml</param>
/// <returns>bool of whether the mouse was inside a node or not, should later return the node id for the node that was pressed
/// or return nothing if it didn't click a node</returns>
bool misc::VisualGraph::mouse_check(sf::Vector2f mouse_pos)
{
	ssuds::UnorderedMap<int, sf::TextCircle>::UnorderedMapIterator it = mCircleData.begin();
	if (it != mCircleData.end())
	{
		if ((*it).second.point_inside(mouse_pos))
		{
			return true;
		}
		else
		{
			return false;
		}
	}
}