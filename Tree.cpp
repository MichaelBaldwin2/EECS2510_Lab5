/*
 * Tree.cpp
 * Name: Mike Baldwin
 * Course: EECS 2510 Non-Linear Data Structures - Fall 2019
 * Date: 12/14/2019
 * Description: Definition of the tree structure
*/

#include <string>
#include "DynamicArray.h"
#include "Edge.h"
#include "Node.h"
#include "Tree.h"

Tree::Tree() : nodes(), edges()
{
	/*
	* Default constructor: Parameters set using initilization list
	*/
}

Tree::Tree(DynamicArray<Node> nodes, DynamicArray<Edge> edges) : nodes(nodes), edges(edges)
{
	/*
	* Constructor: Parameters set using initilization list
	* Copies the supplied parameters
	*/
}

bool Tree::operator== (const Tree& other) const
{
	/*
	* Equality operator
	*/

	return nodes == other.nodes && edges == other.edges;
}

bool Tree::operator!= (const Tree& other) const
{
	/*
	* Inequality operator.
	* Checks for equality, then inverts the result.
	*/

	return !(*this == other);
}

std::string Tree::ToString()
{
	/*
	* Builds a string of the tree and returns it.
	* Used for writing the tree to console.
	*/

	std::string result = "";

	// First calculate the total edge weight
	double totalWeight = 0;
	for (int i = 0; i < edges.Size(); i++)
		totalWeight += edges[i].weight;

	result += std::to_string(totalWeight);
	result += "\n";

	DynamicArray<Edge> copyOfEdges = edges;

	// Then 'Sort' the edges as you pull them and append them to the string.
	while (copyOfEdges.Size() > 0)
	{
		int smallestIndex = 0;
		for (int i = 0; i < copyOfEdges.Size(); i++)
		{
			if (copyOfEdges[i].node1Name < copyOfEdges[smallestIndex].node1Name)
				smallestIndex = i;
			if (copyOfEdges[i].node1Name == copyOfEdges[smallestIndex].node1Name)
				if (copyOfEdges[i].node2Name < copyOfEdges[smallestIndex].node2Name)
					smallestIndex = i;
		}

		result += copyOfEdges[smallestIndex].node1Name + '-' + copyOfEdges[smallestIndex].node2Name + ": " + std::to_string(copyOfEdges[smallestIndex].weight);
		result += '\n';
		copyOfEdges.RemoveAt(smallestIndex);
	}
	return result;
}
