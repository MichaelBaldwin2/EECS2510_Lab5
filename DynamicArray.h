/*
 * DynamicArray.h (and .cpp too I guess)
 * Name: Mike Baldwin
 * Course: EECS 2510 Non-Linear Data Structures - Fall 2019
 * Date: 12/14/2019
 * Description:
 *		This is MY implementation of a dynamicArray.
 *		It is a 100% NON-STL class capable of resizing an array when it starts to fill, similar to the std::vector.
 *		I would LOVE to be able to just use std::copy and std::move to avoid loops, but alas, they are STL themselves.
 *		And I can't use memcpy and memmove because my structures contain strings and custom objects, they would need strcpy & memcpy called on them, individually, to be copied fully.
 *		I made this templated because I use a number of different types with it, in this project.
 *
 *		I had to put the definitions in the header file (with the declarations) because of how c++ handles templates.
 *		I *could* put them in a cpp file, but the only way (that I know of) requires a hacky solution of adding this to EVERY cpp file that this class is used in:
 *
 *			<code>
 *				template DynamicArray<*MyType*>();
 *			</code>
 *
 *		Maybe there is a better way...
*/

#pragma once

template <typename T>
class DynamicArray
{
public:
	DynamicArray(); // Default constructor
	DynamicArray(const DynamicArray<T>& other); // Copy constructor
	DynamicArray(DynamicArray<T>&& other); // Move constructor
	DynamicArray(int preAllocate); // Constructor for pre-allocating array
	~DynamicArray(); // Destructor
	T& operator[] (int index); // Array access operator
	bool operator== (const DynamicArray<T>& rhs) const; // Equality operator
	bool operator!= (const DynamicArray<T>& rhs) const; // Inequality operator
	DynamicArray<T>& operator=(DynamicArray<T> other); // Copy assignment operator
	// No need for a 'Move' assignment operator (aka &&), as the copy assignment and 'move' constructor perform the same role.
	int Size();
	void Add(T item);
	void RemoveAt(int index);
	void Remove(T item);
	int Find(T item);
	bool Contains(T item);
	void SwapItems(unsigned int index1, unsigned int index2);

private:
	T* ptr;
	int size;
	int allocated;
	void Grow();
	void Swap(DynamicArray<T>& lhs, DynamicArray<T>& rhs);
};

//=================================================\\
//============= MEMBER DEFINITIONS ================\\
//=================================================\\

template <class T>
DynamicArray<T>::DynamicArray() : size(0), allocated(1), ptr(new T[1]())
{
	/*
	* Default constructor: Parameters set using initilization list
	*/
}

template <class T>
DynamicArray<T>::DynamicArray(const DynamicArray<T>& other) : size(other.size), allocated(other.size > 0 ? other.size : 1), ptr(other.size > 0 ? new T[other.size] : new T[1])
{
	/*
	* Copy constructor: Parameters set using initilization list
	*/

	for(int i = 0; i < other.size; i++)
		ptr[i] = other.ptr[i]; // Just letting the distinct objects copy constructor do the work for me

	//std::copy(other.ptr, other.ptr + other.size, ptr); // ONLY HAVE THIS IN FOR TESTING
}

template <class T>
DynamicArray<T>::DynamicArray(DynamicArray<T>&& other) : DynamicArray<T>()
{
	/*
	* Move constructor
	* I let the compiler build a default constructed object, then swap it out
	* Destruction takes place directly after.
	*/

	Swap(*this, other);
}

template <class T>
DynamicArray<T>::DynamicArray(int preAllocate) : size(0), allocated(preAllocate > 0 ? preAllocate : 1), ptr(preAllocate > 0 ? new T[preAllocate]() : new T[1]())
{
	/*
	* Constructor preallocating array size: Parameters set using initilization list
	*/
}

template <class T>
DynamicArray<T>::~DynamicArray()
{
	/*
	* Destructor: Clears mem
	*/

	delete[] ptr;
	ptr = nullptr;
	size = 0;
	allocated = 0;
}

template <class T>
T& DynamicArray<T>::operator[] (int index)
{
	/*
	* Array offset operator. Used for getting and setting items.
	*/

	return ptr[index];
}

template <class T>
bool DynamicArray<T>::operator== (const DynamicArray<T>& rhs) const
{
	/*
	* Equality operator.
	* If the sizes don't match return false.
	* If they match, then check every object.
	*/

	if (size != rhs.size)
		return false;
	for (int i = 0; i < size && i < rhs.size; i++)
		if (ptr[i] != rhs.ptr[i])
			return false;
	return true;
}

template <class T>
bool DynamicArray<T>::operator!= (const DynamicArray<T>& rhs) const
{
	/*
	* Inequality operator.
	* Checks for equality, then inverts the result.
	*/

	return !(*this == rhs);
}

template <class T>
DynamicArray<T>& DynamicArray<T>::operator=(DynamicArray<T> other)
{
	/*
	* Assignment operator. Used when assigning an object to this object.
	* I follow the copy-swap idiom when using a data wrapping class like this.
	* So I swap out this class for the right hand value object, leaving the old value in a usuable state
	* to be destructed cleanly without causing dangling pointers. Much safer.
	*/

	Swap(*this, other);
	return *this;
}

template <class T>
int DynamicArray<T>::Size()
{
	/*
	* Returns the size of the array (not the allocation!)
	*/

	return size;
}

template <class T>
void DynamicArray<T>::Add(T item)
{
	/*
	* Adds a new item onto the end of the array. If the new size equals the current allocation, the array will call grow()
	*/

	if (size == allocated)
		Grow();

	ptr[size] = item;
	size++;
}

template <class T>
void DynamicArray<T>::RemoveAt(int index)
{
	/*
	* Removes an item at the specified index, then shifts the underlying array to fill the now empty space.
	*/

	for (unsigned int i = index; i < size - 1; i++)
		ptr[i] = ptr[i + 1];
	//std::move(ptr + index + 1, ptr + size, ptr + index); // ONLY HAVE THIS IN FOR TESTING
	size--;
}

template <class T>
void DynamicArray<T>::Remove(T item)
{
	/*
	* Removes an item from the list, calls find so O(n)
	*/

	int index = Find(item);
	if (index >= 0)
		RemoveAt(index);
}

template <class T>
void DynamicArray<T>::Grow()
{
	/*
	* Grows the underlying array as needed.
	* It actually just doubles the allocated size each time this is called, nothing fancy.
	* Allocation will be trimmed to current size when the copy constructor is called.
	*/

	int newSize = allocated * 2;
	T* newPtr = new T[newSize];

	for(int i = 0; i < size; i++)
		newPtr[i] = ptr[i];

	//std::copy(ptr, ptr + allocated, newPtr); // ONLY HAVE THIS IN FOR TESTING
	allocated = newSize;
	delete[] ptr;
	ptr = newPtr;
}

template <class T>
int DynamicArray<T>::Find(T item)
{
	/*
	* Searches for an item, the only function that loops: O(n)
	* Required to loop as it has to search for an object.
	* Will return -1 if none found.
	*/

	for (int i = 0; i < size; i++)
		if (ptr[i] == item)
			return i;
	return -1;
}

template <class T>
bool DynamicArray<T>::Contains(T item)
{
	/*
	* Searches for an item and returns true if found, false otherwise.
	* Useful when you don't want the item, you just want to know if it exists.
	*/

	return Find(item) >= 0;
}

template <class T>
void DynamicArray<T>::SwapItems(unsigned int index1, unsigned int index2)
{
	/*
	* Swaps two items at the two locations.
	*/

	T temp = ptr[index1];
	ptr[index1] = ptr[index2];
	ptr[index2] = temp;
}

template <class T>
void DynamicArray<T>::Swap(DynamicArray<T>& lhs, DynamicArray<T>& rhs)
{
	/*
	* Swap function.
	* Short. And. Sweet.
	*/

	T* tempPtr = lhs.ptr;
	lhs.ptr = rhs.ptr;
	rhs.ptr = tempPtr;

	int tempSize = lhs.size;
	lhs.size = rhs.size;
	rhs.size = tempSize;
	
	int tempAllocated = lhs.allocated;
	lhs.allocated = rhs.allocated;
	rhs.allocated = tempAllocated;
}
