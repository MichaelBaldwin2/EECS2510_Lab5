#include "BinaryHeap.h"
#include <iostream>
#include <string>
#include "DynamicArray.h"

BinaryHeap::BinaryHeap()
{
	data = DynamicArray<Node>();
}

void BinaryHeap::Push(Node item)
{
	data.Add(item);
	int nodePos = data.Size() - 1;
	int parentPos = nodePos / 2;

	while (parentPos > 0 && item < data[parentPos])
	{
		data.SwapItems(nodePos, parentPos);
		nodePos = parentPos;
		parentPos = nodePos / 2;
	}
}

Node BinaryHeap::Pop()
{
	if (data.Size() <= 1)
	{
		return Node();
	}

	Node result = data[1]; //Save the first node, which is the lowest cost
	data.SwapItems(1, data.Size() - 1); //Then we swap the front and back items, it allows an easy removal without an array shift (at least in my old c++ implementation)
	data.RemoveAt(data.Size() - 1); //Remove the last node (the saved one) as to not mess up the next bit here

	unsigned int nodeIndex = 1;
	unsigned int firstChildIndex = nodeIndex * 2;
	unsigned int secondChildIndex = nodeIndex * 2 + 1;

	while (firstChildIndex < data.Size())
	{
		if (secondChildIndex < data.Size()) //Both children exist
		{
			if (data[nodeIndex] > data[firstChildIndex] || data[nodeIndex] > data[secondChildIndex]) //One of the children have a lower FCost
			{
				int lowestChildIndex = secondChildIndex;
				if (data[firstChildIndex] < data[secondChildIndex])
					lowestChildIndex = firstChildIndex;
				data.SwapItems(nodeIndex, lowestChildIndex);
				nodeIndex = lowestChildIndex;
				firstChildIndex = nodeIndex * 2;
				secondChildIndex = nodeIndex * 2 + 1;
			}
			else
				break;
		}
		else //Only the first child exists
		{
			if (data[nodeIndex] > data[firstChildIndex]) //The first child has a lower FCost
			{
				data.SwapItems(nodeIndex, firstChildIndex);
				nodeIndex = firstChildIndex;
				firstChildIndex = nodeIndex * 2;
				secondChildIndex = nodeIndex * 2 + 1;
			}
			else
				break;
		}
	}

	return result; //Return the old saved lowest FCost node
}

Node BinaryHeap::Peek()
{
	return data[1];
}

void BinaryHeap::Resort(int index)
{
	int nodePos = index;
	int parentPos = nodePos / 2;

	while (parentPos > 0 && data[nodePos] < data[parentPos])
	{
		data.SwapItems(nodePos, parentPos);
		nodePos = parentPos;
		parentPos = nodePos / 2;
	}
}

Node &BinaryHeap::operator[] (int index)
{
	return data[index];
}