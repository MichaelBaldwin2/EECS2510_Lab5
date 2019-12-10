/*
 * Node.cpp
 * Name: Mike Baldwin
 * Course: EECS 2510 Non-Linear Data Structures - Fall 2019
 * Date: 12/14/2019
 * Description: Definition of the node structure
*/

#include <string>
#include "DynamicArray.h"
#include "Edge.h"
#include "Node.h"


Node::Node() : name(""), edges()
{
	/*
	* Default constructor: Parameters set using initilization list
	*/
}

Node::Node(std::string name) : name(name), edges()
{
	/*
	* Constructor: Parameters set using initilization list
	*/
}

bool Node::operator< (const Node& other) const
{
	/*
	* Less than operator
	*/

	return name < other.name;
}

bool Node::operator> (const Node& other) const
{
	/*
	* Greater than operator
	*/

	return name > other.name;
}

bool Node::operator<= (const Node& other) const
{
	/*
	* Less than or equal than operator
	*/
	
	return (*this < other) || (*this == other);
}

bool Node::operator>= (const Node& other) const
{
	/*
	* Greater than or equal operator
	*/

	return (*this > other) || (*this == other);
}

bool Node::operator== (const Node& other) const
{
	/*
	* Equality operator
	*/

	return name == other.name;
}

bool Node::operator!= (const Node& other) const
{
	/*
	* Inequality operator.
	* Checks for equality, then inverts the result.
	*/

	return !(*this == other);
}
