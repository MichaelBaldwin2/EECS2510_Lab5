/*
 * Tree.h
 * Name: Mike Baldwin
 * Course: EECS 2510 Non-Linear Data Structures - Fall 2019
 * Date: 12/14/2019
 * Description: Declaration of the tree structure
*/

#pragma once

#include <string>
#include "DynamicArray.h"
#include "Edge.h"
#include "Node.h"

struct Tree
{
	DynamicArray<Node> nodes;
	DynamicArray<Edge> edges;

	Tree();
	Tree(DynamicArray<Node> nodes, DynamicArray<Edge> edges);
	bool operator== (const Tree& other) const;
	bool operator!= (const Tree& other) const;
	std::string ToString();
};
