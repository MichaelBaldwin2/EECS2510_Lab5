/*
 * Edge.cpp
 * Name: Mike Baldwin
 * Course: EECS 2510 Non-Linear Data Structures - Fall 2019
 * Date: 12/14/2019
 * Description: Definition of the edge structure
*/

#include <string>
#include "Edge.h"

Edge::Edge() : node1Name(""), node2Name(""), weight(0)
{
	/*
	* Default constructor: Parameters set using initilization list
	*/
}

Edge::Edge(std::string node1Name, std::string node2Name, double weight) : node1Name(node1Name), node2Name(node2Name), weight(weight)
{
	/*
	* Constructor: Parameters set using initilization list
	*/

	// Make sure the smallest (natural equality) is the first node name
	if (this->node2Name < this->node1Name)
	{
		this->node1Name = node2Name;
		this->node2Name = node1Name;
	}
}

bool Edge::operator< (const Edge& other) const
{
	/*
	* Less than operator
	*/

	return weight < other.weight;
}

bool Edge::operator> (const Edge& other) const
{
	/*
	* Greater than operator
	*/

	return weight > other.weight;
}

bool Edge::operator<= (const Edge& other) const
{
	/*
	* Less than or equal than operator
	*/

	return (*this < other) || (*this == other);
}

bool Edge::operator>= (const Edge& other) const
{
	/*
	* Greater than or equal operator
	*/

	return (*this > other) || (*this == other);
}

bool Edge::operator== (const Edge& other) const
{
	/*
	* Equality operator
	*/

	return weight == other.weight && node1Name == other.node1Name && node2Name == other.node2Name;
}

bool Edge::operator!= (const Edge& other) const
{
	/*
	* Inequality operator.
	* Checks for equality, then inverts the result.
	*/

	return !(*this == other);
}
