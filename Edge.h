/*
 * Edge.h
 * Name: Mike Baldwin
 * Course: EECS 2510 Non-Linear Data Structures - Fall 2019
 * Date: 12/14/2019
 * Description: Declaration of the node structure
*/

#pragma once

#include <string>

struct Edge
{
	std::string node1Name;
	std::string node2Name;
	double weight;

	Edge();
	Edge(std::string node1Name, std::string node2Name, double weight);
	bool operator< (const Edge& other) const;
	bool operator> (const Edge& other) const;
	bool operator<= (const Edge& other) const;
	bool operator>= (const Edge& other) const;
	bool operator== (const Edge& other) const;
	bool operator!= (const Edge& other) const;
};
