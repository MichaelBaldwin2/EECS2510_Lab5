/*
 * BinaryHeap.h (and .cpp too I guess)
 * Name: Mike Baldwin
 * Course: EECS 2510 Non-Linear Data Structures - Fall 2019
 * Date: 12/14/2019
 * Description:
 *		This is MY implementation of a MIN binary heap.
 *		Although the Cormen text describes a MAX-heap in psuedo-code, I was having trouble following how it resorts on removal.
 *		So I made this one from the description of it instead......and had lots of fun debugging it....
 *		Similar to the DynamicArray this class is templated as well, although technically I think I only use the Edge struct with this, so it doesn't have to be.
 *		Unlike the DynamicArray, this clas doesn't require a copy constructor, move constructor, or assignment operator overload; as it is not handling dynamic memory.
 *
 *		NOTE: Please see the description of the DynamicArray as to why I have the definitions in the header file. Thank you.
*/

#pragma once

#include <iostream>
#include "DynamicArray.h"

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

private:
	DynamicArray<T> data;
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
	 * Pushes an item onto the heap.
	 * This method places the item at the back (or bottom) of the heap
	 * and slowly moves it towards the front (or top) depending upon its equality.
	 * Very similar to MaxHeapify() (or I guess MinHeapify()) but my own version, also non-recursive.
	 * The way I have it working is that it doesn't care if it's less than its sibling.
	 * It only cares if it's less than its parent or not.
	 * This may not be the *exact* way Max(Min)Heapify() works, but it keeps items sorted correctly,
	 * in the sense that the first item is ALWAYS the lowest value, and when you add or remove an item
	 * the heap will resort itself to maintain that lowest value on the bottom.
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
	data.SwapItems(1, data.Size() - 1); // Then we swap the front and back items. NOTE: it allows an easy removal without an array shift
	data.RemoveAt(data.Size() - 1); // Remove the last node (the saved one)

	int nodeIndex = 1; // Set the index (parent index) to the first value (my heap starts at 1)
	int firstChildIndex = nodeIndex * 2; // Get the first child index (this may or may not exist)
	int secondChildIndex = nodeIndex * 2 + 1; // Get the second child index (this may or may not exist)

	// If we ever *fall* off the back of the heap (with either children), we've gone too far
	while (firstChildIndex < data.Size())
	{
		if (secondChildIndex < data.Size())
		{
			// Find which child has a lower value, Swap items (if needed) and set new index values
			if (data[firstChildIndex] < data[nodeIndex] || data[secondChildIndex] < data[nodeIndex])
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
				break; // We are as far as we can go
		}
		else // Only the first child exists
		{
			// Check if the first child has a lower value, if so, swap items and set new index values
			if (data[firstChildIndex] < data[nodeIndex])
			{
				data.SwapItems(nodeIndex, firstChildIndex);
				nodeIndex = firstChildIndex;
				firstChildIndex = nodeIndex * 2;
				secondChildIndex = nodeIndex * 2 + 1;
			}
			else
				break; // We are as far as we can go
		}
	}

	return result; // Return the previously saved item
}

template <class T>
T BinaryHeap<T>::Peek()
{
	/*
	 * Get the top (lowest) value, but don't remove it from the heap.
	 * Because the heap starts at 1, grab the element at 1.
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
