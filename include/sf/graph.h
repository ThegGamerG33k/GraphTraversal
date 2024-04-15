#pragma once
#include <unordered_map.h>
#include <sstream>

namespace ssuds
{
	/// <summary>
	/// A simple Graph class
	/// </summary>
	/// <typeparam name="N">The type of Nodes/Vertices</typeparam>
	/// <typeparam name="E">The type of Edges</typeparam>
	template <class N, class E>
	class Graph
	{
	protected:
		/// <summary>
		/// The internal data structure used to implement all of our methods
		/// </summary>
		typename UnorderedMap<N, UnorderedMap<N, E>> mData;

	public:
		/// <summary>
		/// Adds a node to the graph (no effect if this Node already exists)
		/// </summary>
		/// <param name="val">The value of the node</param>
		void add_node(const N& val)
		{
			// This seems like it's too simple, but by doing this we 
			// do create the node.  If it's already there, this does nothing
			mData[val];
		}


		/// <summary>
		/// Tests for existence of a node
		/// </summary>
		/// <param name="val">the Node in question</param>
		/// <returns>true if it exists in this graph</returns>
		bool contains_node(const N& val) const
		{
			typename UnorderedMap<N, UnorderedMap<N, E>>::UnorderedMapIterator it = mData.find(val);
			return it != mData.end();
		}


		/// <summary>
		/// Tests for existence of an edge between two nodes
		/// </summary>
		/// <param name="start_node">The starting node</param>
		/// <param name="destination_node">The destination node</param>
		/// <returns>true if there is an edge</returns>
		bool contains_edge(const N& start_node, const N& destination_node) const
		{
			typename UnorderedMap<N, UnorderedMap<N, E>>::UnorderedMapIterator it = mData.find(start_node);
			if (it != mData.end())
			{
				typename UnorderedMap<N, E>::UnorderedMapIterator inner_it = (*it).second.find(destination_node);
				return inner_it != (*it).second.end();
			}
			return false;
		}


		/// <summary>
		/// Gets an edge (raises a std::exception if the start or destination node are not in this graph)
		/// </summary>
		/// <param name="start_node">start node</param>
		/// <param name="destination_node">end node</param>
		/// <returns>Edge value reference</returns>
		E& get_edge(const N& start_node, const N& destination_node) const
		{
			typename UnorderedMap<N, UnorderedMap<N, E>>::UnorderedMapIterator it = mData.find(start_node);
			if (it != mData.end())
			{
				typename UnorderedMap<N, E>::UnorderedMapIterator inner_it = (*it).second.find(destination_node);
				if (inner_it != (*it).second.end())
					return (*inner_it).second;
			}
			throw std::exception("invalid start or destination node");
		}


		/// <summary>
		/// Creates / update an edge between two nodes.  A std::out_of_range exception is thrown
		/// if either node does not exist
		/// </summary>
		/// <param name="start">start node</param>
		/// <param name="destination">destination node</param>
		/// <param name="edge_value">new edge value</param>
		void set_edge(const N& start, const N& destination, const E& edge_value)
		{
			typename UnorderedMap<N, UnorderedMap<N, E>>::UnorderedMapIterator it = mData.find(destination);
			if (it == end())
				throw std::out_of_range("Invalid destination node -- add this node before adding edges to it");
			it = mData.find(start);
			if (it == end())
				throw std::out_of_range("Invalid start node -- add this node before adding edges from it");

			(*it).second[destination] = edge_value;
		}


		/// <summary>
		/// Removes the node and all edges to / from it in the graph
		/// </summary>
		/// <param name="node">node to remove</param>
		/// <returns>true if that node was removed (false if it didn't exist)</returns>
		bool remove_node(const N& node)
		{
			// The sneaky part: remove nodes going TO this one
			typename UnorderedMap<N, UnorderedMap<N, E>>::UnorderedMapIterator it = mData.begin();
			while (it != mData.end())
			{
				typename UnorderedMap<N, E>::UnorderedMapIterator inner_it = (*it).second.find(node);
				if (inner_it != (*it).second.end())
					(*it).second.remove(node);
				++it;
			}

			// This is the easy part: removing edges going from that node (and the node itself)
			if (mData.find(node) != mData.end())
			{
				mData.remove(node);
				return true;
			}
			return false;
		}


		/// <summary>
		/// Removes an edge between two nodes.  
		/// </summary>
		/// <param name="start_node">start node</param>
		/// <param name="destination_node">destination node</param>
		/// <returns>true if there was an edge between them (false if there was not, or if either node
		/// did not exist in the graph)</returns>
		bool remove_edge(const N& start_node, const N& destination_node)
		{
			typename UnorderedMap<N, UnorderedMap<N, E>>::UnorderedMapIterator it = mData.find(start_node);
			if (it != mData.end())
			{
				typename UnorderedMap<N, E>::UnorderedMapIterator inner_it = (*it).second.find(destination_node);
				if (inner_it != (*it).second.end())
				{
					(*it).second.remove(destination_node);
					return true;
				}
			}
			return false;
		}


		/// <summary>
		/// Returns the number of nodes in the graph
		/// </summary>
		/// <returns>number of nodes</returns>
		unsigned int num_nodes() const
		{
			return mData.size();
		}


		/// <summary>
		/// Returns the number of outgoing edges from this node
		/// </summary>
		/// <param name="start_node">the node in question</param>
		/// <returns>the number of neighbors</returns>
		unsigned int num_neighbors(const N& start_node) const
		{
			typename UnorderedMap<N, E>::UnorderedMapIterator it = mData.find(start_node);
			if (it == end())
				throw std::out_of_range("Invalid node");
			return it.second.size();
		}


		/// <summary>
		/// Returns an iterator referring to the given node (or end iterator if that is not found)
		/// </summary>
		/// <param name="node">The node we're searching for</param>
		/// <returns>an iterator</returns>
		typename UnorderedMap<N, UnorderedMap<N,E>>::UnorderedMapIterator find(const N& node) const
		{
			return mData.find(node);
		}

		/// <summary>
		/// Returns an iterator that is capable of visiting all nodes in the graph
		/// </summary>
		/// <returns>an iterator</returns>
		typename UnorderedMap<N, UnorderedMap<N, E>>::UnorderedMapIterator begin() const
		{
			return mData.begin();
		}

		/// <summary>
		/// Returns a special iterator value that indicates if we couldn't find the node requested by
		/// find or have reached the end of iteration
		/// </summary>
		/// <returns>an iterator</returns>
		typename UnorderedMap<N, UnorderedMap<N, E>>::UnorderedMapIterator end() const
		{
			return mData.end();
		}

		/// <summary>
		/// Removes all nodes and edges in the graph
		/// </summary>
		void clear()
		{
			mData.clear();
		}


		/// <summary>
		/// Outputs the graph to the given output stream in adjacency list style
		/// </summary>
		/// <param name="os">the output stream (cout, file, etc.)</param>
		/// <param name="G">the graph</param>
		/// <returns>the (unchanged) output stream</returns>
		friend std::ostream& operator<<(std::ostream& os, const Graph& G)
		{
			// Figure out the maximum string size of all nodes
			std::stringstream ss;
			size_t max_len = 0;
			typename UnorderedMap<N, UnorderedMap<N, E>>::UnorderedMapIterator it = G.begin();
			while (it != G.end())
			{
				ss.str("");
				ss << (*it).first;
				size_t new_len = ss.str().length();
				if (new_len > max_len)
					max_len = new_len;
				++it;
			}

			// Now actually write out the lines
			it = G.begin();
			while (it != G.end())
			{
				ss.str("");
				ss << (*it).first;
				std::string temps = ss.str();
				
				os << temps;
				for (int i = 0; i < max_len - temps.size(); i++)
					os << " ";
				os << " | ";
				typename UnorderedMap<N, E>::UnorderedMapIterator inner_it = (*it).second.begin();
				while (inner_it != (*it).second.end())
				{
					os << "(" << (*inner_it).first << ":" << (*inner_it).second << ") ";
					++inner_it;
				}
				os << "\n";
				++it;
			}

			return os;
		}
	};
}