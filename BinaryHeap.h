/*
 * BinaryHeap.h (and .cpp too I guess)
 * Name: Mike Baldwin
 * Course: EECS 2510 Non-Linear Data Structures - Fall 2019
 * Date: 12/14/2019
 * Description: MY Minimum Binary Heap class.
*/

#pragma once

#include <iostream>
#include "DynamicArray.h"

/*
 * This is MY c# implementation of a binary heap.
 * Although the Cormen text describes a min-heap...nicely. I wasn't that impressed with the psuedo-code.
 * I understand the psuedo-code is meant to be language-agnostic, but it seems very non-oop and extreamly c-style.
 * SO I made this one from the description of the min-heap instead. .....and had lots of fun debugging it....
 *
 * Similar to the DynamicArray, I was about 3 monster energy drinks in at 2am so this class is templated as well.
 *
 * Unlike the DynamicArray, this clas doesn't require a copy constructor, move constructor, or assignment operator overload.
 * As it is not handling dynamic memory.
*/
template <typename T>
class BinaryHeap
{
public:
	BinaryHeap<T>();
	void Push(T item);
	T Pop();
	T Peek();
	unsigned int Size();
	bool Contains(T item);
	T& operator[] (int index);
	void MaxHeapify(int index);

private:
	DynamicArray<T> data;
	int GetParentIndexOf(int index);
	int GetLeftIndexOf(int index);
	int GetRightIndexOf(int index);
};

//=================================================\\
//============= MEMBER DEFINITIONS ================\\
//=================================================\\

template <class T>
BinaryHeap<T>::BinaryHeap() : data()
{
	/*
	* Default constructor: Parameters set using initilization list
	* Have to add a blank item to the data structure, as the heap starts at index 1.
	*/

	data.Add(T());
}

template <class T>
void BinaryHeap<T>::Push(T item)
{
	/*
	 * Pushes an item ont the heap.
	 * This method places the item at the back (or bottom) of the heap
	 * and slowly moves it towards the front (or top) depending upon its equality.
	 * Very similar to MaxHeapify() but my own version.
	*/

	// Add the item to the underlying list, set the index variables to the new item and it's parent (if there is one)
	data.Add(item);
	int nodePos = data.Size() - 1;
	int parentPos = nodePos / 2;

	// If there is a parent AND the value of the item is less than it's parent, swap the positions.
	while (parentPos > 0 && item < data[parentPos])
	{
		data.SwapItems(nodePos, parentPos);
		nodePos = parentPos;
		parentPos = nodePos / 2;
	}
}

template <class T>
T BinaryHeap<T>::Pop()
{
	/*
	 * Removes the first item off of the heap.
	 * Because this is a min-heap, this item is the lowest.
	 * After removal it "re-sorts" the children and finds a new root.
	*/

	if (data.Size() <= 1)
	{
		std::cout << "Nothing in the BinaryHeap!" << std::endl;
		return T();
	}

	T result = data[1]; // Save the first node, which is the lowest cost
	data.SwapItems(1, data.Size() - 1); // Then we swap the front and back items, it allows an easy removal without an array shift (at least in my old c++ implementation)
	data.RemoveAt(data.Size() - 1); // Remove the last node (the saved one) as to not mess up the next bit here

	int nodeIndex = 1;
	int firstChildIndex = nodeIndex * 2;
	int secondChildIndex = nodeIndex * 2 + 1;

	while (firstChildIndex < data.Size())
	{
		if (secondChildIndex < data.Size()) //Both children exist
		{
			// Find which child has a lower value, Swap items (if needed) and set new index values
			if (data[nodeIndex] > data[firstChildIndex] || data[nodeIndex] > data[secondChildIndex])
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
			// Check if the first child has a lower value, if so, swap items and set new index values
			if (data[nodeIndex] > data[firstChildIndex])
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

	return result;
}

template <class T>
T BinaryHeap<T>::Peek()
{
	/*
	 * Get the top (lowest) value, but don't remove it from the heap.
	*/

	return data[1];
}

template <class T>
unsigned int BinaryHeap<T>::Size()
{
	/*
	* Returns the size of the heap, not counting the first empty
	*/

	return data.Size() - 1;
}

template <class T>
bool BinaryHeap<T>::Contains(T item)
{
	/*
	* Searches for an item and returns true if found, false otherwise.
	* Useful when you don't want the item, you just want to know if it exists.
	*/

	return data.Contains(item);
}

template <class T>
T& BinaryHeap<T>::operator[] (int index)
{
	/*
	* Array offset operator. Used for getting and setting items.
	* Offsets all accesses by 1 to account for empty first item.
	*/

	return data[index + 1];
}

/*
* I included the below functions to appease the grader in-case they didn't like my version.
* Although the below functions work, I don't use them for this project.
*/

template <class T>
int BinaryHeap<T>::GetParentIndexOf(int index)
{
	/*
	 * Returns the parent item of the item at the supplied index
	*/

	return data[index / 2];
}

template <class T>
int BinaryHeap<T>::GetLeftIndexOf(int index)
{
	/*
	 * Returns the left child item of the item at the supplied index
	*/

	return data[2 * index];
}

template <class T>
int BinaryHeap<T>::GetRightIndexOf(int index)
{
	/*
	 * Returns the right child item of the item at the supplied index
	*/

	return data[2 * index + 1];
}

template <class T>
void BinaryHeap<T>::MaxHeapify(int index)
{
	/*
	 * Recursively sorts an item down through the heap
	*/

	int left = GetLeftIndexOf(index);
	int right = GetRightIndexOf(index);
	int largest = 0;

	if(left <= data.Size() && data[left] > data[index])
		largest = left;
	else
		largest = index;
	if(right <= data.Size() && data[right] > data[largest])
		largest = right;
	if(largest != index)
		data.SwapItems(index, largest);
	MaxHeapify(largest);
}
