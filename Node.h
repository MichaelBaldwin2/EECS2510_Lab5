/*
 * Node.h
 * Name: Mike Baldwin
 * Course: EECS 2510 Non-Linear Data Structures - Fall 2019
 * Date: 12/14/2019
 * Description: Declaration of the node structure
*/

#pragma once

#include <string>
#include "DynamicArray.h"
#include "Edge.h"

struct Node
{
	std::string name;
	DynamicArray<Edge> edges;

	Node();
	Node(std::string name);
	bool operator< (const Node& other) const;
	bool operator> (const Node& other) const;
	bool operator<= (const Node& other) const;
	bool operator>= (const Node& other) const;
	bool operator== (const Node& other) const;
	bool operator!= (const Node& other) const;
};