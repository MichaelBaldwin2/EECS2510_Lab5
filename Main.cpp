/*
 * Main.cpp
 * Name: Mike Baldwin
 * Course: EECS 2510 Non-Linear Data Structures - Fall 2019
 * Date: 12/14/2019
 * Description: Main file, does the cmd line parsing and contains the algorithms for the MSTs
*/

#include <string>
#include <iostream>
#include <fstream>
#include "BinaryHeap.h"
#include "DynamicArray.h"
#include "Edge.h"
#include "Node.h"
#include "Tree.h"

Tree RunMST_Kruskal(DynamicArray<Node> nodes, DynamicArray<Edge> edges);
Tree RunMST_Prim(DynamicArray<Node> nodes, DynamicArray<Edge> edges);
Tree FindSet(DynamicArray<Tree> forest, Node node);
Tree Merge(Tree tree1, Tree tree2, Edge edge);

int main(int argc, char* argv[])
{
	/*
	 * Main function. Parses the cmd args and passes that into the required functions
	*/

	// Check for more or less than 2 arguments (including SPAN), throw warning message and return if so.
	if (argc < 2)
	{
		std::cout << "Too few arguments!" << std::endl;
		std::cout << "The only argument accepted is an file path to a spanning tree file." << std::endl;
		return 0;
	}
	if (argc > 2)
	{
		std::cout << "Too many arguments!" << std::endl;
		std::cout << "The only argument accepted is an file path to a spanning tree file." << std::endl;
		return 0;
	}

	// Declare variables
	int nodeCount = 0;
	DynamicArray<Node> nodes;
	DynamicArray<Edge> edges;
	DynamicArray<double> adjMat = DynamicArray<double>();

	// Open the input file and check if it opened correctly
	std::fstream inputStream;
	inputStream.open(argv[1]);
	if (!inputStream.is_open())
	{
		std::cout << "Failed to open the file" << std::endl;
		return 0;
	}

	/*
	 * The next block of code reads all 'n' node names.
	 * Followed by reading the entire matrix into a single dimensional array
	*/
	std::string word;
	inputStream >> word;
	nodeCount = std::stoi(word);
	for (int i = 0; i < nodeCount; i++)
	{
		inputStream >> word;
		nodes.Add(word);
	}
	for (int i = 0; i < nodeCount; i++)
	{
		for (int j = 0; j < nodeCount; j++)
		{
			inputStream >> word;
			adjMat.Add(std::stod(word));
		}
	}

	// Build a list of nodes and connected edges (with weights) this is what we will pass into each function to build the mst
	for (int i = 0; i < nodeCount; i++)
	{
		for (int j = i; j < nodeCount; j++) // We start j at i, to avoid duplications across the matrix diagonal
		{
			if (i == j) // Skip the center diagonal
				continue;

			std::string node1Name = nodes[i].name;
			std::string node2Name = nodes[j].name;
			double weight = adjMat[j * nodeCount + i];

			if (weight == 0) // A weight of 0 (zero) means no edge
				continue;

			// Build the edge and add it to the edges list
			Edge edge = Edge(node1Name, node2Name, weight);
			edges.Add(edge);

			// Add this edge to the node's edge list as well
			nodes[i].edges.Add(edge);
			nodes[j].edges.Add(edge);
		}
	}

	Tree kMST = RunMST_Kruskal(nodes, edges);
	std::cout << kMST.ToString() << std::endl;

	Tree pMST = RunMST_Prim(nodes, edges);
	std::cout << pMST.ToString() << std::endl;

	inputStream.close();
	return 0;
}

Tree RunMST_Kruskal(DynamicArray<Node> nodes, DynamicArray<Edge> edges)
{
	/*
	* Kruskal's minimum spanning tree algorithm.
	*/

	// Forest of trees
	DynamicArray<Tree> forest = DynamicArray<Tree>(nodes.Size());
	for (int i = 0; i < nodes.Size(); i++)
	{
		forest.Add(Tree());
		forest[i].nodes.Add(nodes[i]);
	}

	// Sort the edges by weight, by inserting them into a BinaryHeap
	BinaryHeap<Edge> sortedEdges = BinaryHeap<Edge>();
	for (int i = 0; i < edges.Size(); i++)
		sortedEdges.Push(edges[i]);

	while (sortedEdges.Size() > 0)
	{
		// Get the smallest (weight) edge
		Edge edge = sortedEdges.Pop();
		Tree tree1 = FindSet(forest, Node(edge.node1Name));
		Tree tree2 = FindSet(forest, Node(edge.node2Name));

		// Check two trees to see if this edge will make loop.
		if (tree1 == tree2)
			continue;

		Tree mergedTree = Merge(tree1, tree2, edge);
		forest.Remove(tree1);
		forest.Remove(tree2);
		forest.Add(mergedTree);
	}

	return forest[0];
}

Tree RunMST_Prim(DynamicArray<Node> nodes, DynamicArray<Edge> edges)
{
	/*
	* Prim's minimum spanning tree algorithm.
	*/

	Tree tree = Tree();
	BinaryHeap<Edge> sortedEdges = BinaryHeap<Edge>();
	tree.nodes.Add(nodes[0]);


	// Add the first nodes edges to the queue
	for(int i = 0; i < nodes[0].edges.Size(); i++)
		sortedEdges.Push(nodes[0].edges[i]);

	while (tree.nodes.Size() < nodes.Size()) // We loop until we've added every node to the tree
	{
		Edge edge = sortedEdges.Pop();
		bool node1InTree = tree.nodes.Contains(Node(edge.node1Name));
		bool node2InTree = tree.nodes.Contains(Node(edge.node2Name));

		if(node1InTree && node2InTree) // This edge will form a loop
			continue;
		else if(!node1InTree) // Node 1 isn't part of the tree yet; add it and add its edges to the heap
		{
			int indexOfNode = nodes.Find(Node(edge.node1Name));
			Node node = nodes[indexOfNode]; // Get the actual node (not just the name)
			
			tree.nodes.Add(node); // Add this node to the tree
			tree.edges.Add(edge); // Add this edge to the tree (used for printing at the end)

			// Now add this new nodes edges to the tree, if they are not already included
			for(int i = 0; i < node.edges.Size(); i++)
				if(!sortedEdges.Contains(node.edges[i]) && !tree.edges.Contains(node.edges[i]))
					sortedEdges.Push(node.edges[i]);
		}
		else if(!node2InTree) // Node 2 isn't part of the tree yet; add it and add its edges to the heap
		{
			int indexOfNode = nodes.Find(Node(edge.node2Name));
			Node node = nodes[indexOfNode]; // Get the actual node (not just the name)
			
			tree.nodes.Add(node); // Add this node to the tree
			tree.edges.Add(edge); // Add this edge to the tree (used for printing at the end)

			// Now add this new nodes edges to the tree, if they are not already included
			for(int i = 0; i < node.edges.Size(); i++)
				if(!sortedEdges.Contains(node.edges[i]) && !tree.edges.Contains(node.edges[i]))
					sortedEdges.Push(node.edges[i]);
		}
	}

	return tree;
}

Tree FindSet(DynamicArray<Tree> forest, Node node)
{
	/*
	 * Searches through all the tree's within the forest to see if any of them contain the specified node.
	 * If found, returns the tree. If not found, returns and empty tree.
	*/

	for (int i = 0; i < forest.Size(); i++)
		if (forest[i].nodes.Contains(Node(node.name)))
			return forest[i];
	return Tree(); // Never gets here
}

Tree Merge(Tree tree1, Tree tree2, Edge edge)
{
	/*
	 * Merges two trees together to make a combined thrid tree. Includes the specified edge within the third (new) tree.
	 * The added edge id (usually) the edge that connects these trees.
	*/

	DynamicArray<Node> nodes = DynamicArray<Node>(tree1.nodes.Size() + tree2.nodes.Size());
	DynamicArray<Edge> edges = DynamicArray<Edge>(tree1.edges.Size() + tree2.edges.Size());

	// Combine nodes and edges from both trees
	for (int i = 0; i < tree1.nodes.Size(); i++)
		nodes.Add(tree1.nodes[i]);
	for (int i = 0; i < tree2.nodes.Size(); i++)
		nodes.Add(tree2.nodes[i]);
	for (int i = 0; i < tree1.edges.Size(); i++)
		edges.Add(tree1.edges[i]);
	for (int i = 0; i < tree2.edges.Size(); i++)
		edges.Add(tree2.edges[i]);

	// Add the new edge
	edges.Add(edge);

	return Tree(nodes, edges); // Return the tree with the combined nodes and edges (plus the new edge)
}
