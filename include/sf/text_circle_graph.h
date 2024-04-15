#pragma once
#include <graph.h>
#include <text_circle.h>

namespace sf
{
	class TextCircleGraph
	{
	protected:
		ssuds::Graph<sf::TextCircle, float> mData;
	public:
		void addNode(std::string text, float radius)
		{

		}
	};
}
