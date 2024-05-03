/*
HEADER COMMENT 
Full name: Tyler Wescott
NSHE: 5006959108
Goal of the program/output: 
Goal of main is to read data from NSHE.txt and populate hash maps using this data, then prompt user for
names and NSHE ids of students to search and display proper error message or correct student data

Goal of hashMap.cpp is to create constructor, destructor, copyHashMap, deallocateHashMap, operator[], operator=, and
resize functions by following steps outlined in the PDF provided. 
*/

#include <cstdlib>
#include <string>
#include "hashMap.h"

using namespace std;

template <class t1, class t2>
hashMap<t1, t2>::hashMap(const hashMap<t1, t2>& copy)
//deep copy constructor, you can call copyHashMap function
{
	copyHashMap(copy);
}

template <class t1, class t2>
const hashMap<t1, t2>& hashMap<t1, t2>::operator=(const hashMap<t1, t2>& rhs)
//deepcopy assignment operator, you can call deallocateHashMap and copyHashMap functions
{
	if (this != &rhs)
	{
		deallocateHashMap();
		copyHashMap(rhs);
	}

	return *this;
}

template <class t1, class t2>	
hashMap<t1, t2>::~hashMap()
//destructor, you can call deallocateHashMap function
{
	deallocateHashMap();
}

template <class t1, class t2>
t2& hashMap<t1, t2>::operator[](t1 key)
/* function that performs a search/insert in the following way
1. If the load factor is 20% or larger, call the resize function
2. Construct an index using the following code size_t index = hash(key) % size
3. Set a pointer node * x = table[index] and search the linked list for a node with a matching key,
if a match is found, return the node’s value field
4. If not found in table[index] determine if the linked list at this location is full
(if index is even there can be at most 2 nodes, if index is odd there can be at most 3 nodes)
5. If there is room in the table[index] linked list, insert a new node into the list and return
its value field (also increment filled entries by 1 before returning if this insert causes table[index] to max out)
6. If table[index] is maxed out, update index = (index + 1) % size and then go to step 3
*/
{
	//load factor
	size_t loadFactor = filledEntries/size;
	if (loadFactor >= .2)
		resize();

	//construct an index
	size_t index = hash(key) % size;
	
	step3:
	//Set a pointer node * x = table[index]
	node * x = table[index];
	
	int nodeCount = 0;		//count node var to see if linked list gets maxed out

	//search the linked list for a node with a matching key
    while (x != nullptr)
	{
        if (x->key == key)
            return x->value;	//key found, return value

        x = x->link;
		nodeCount++;	//counts nodes
    }
	
	//If not found in table[index] determine if the linked list at this location is full
	if (((index % 2 == 0) && (nodeCount < 2)) || ((index % 2 != 0) && (nodeCount < 3)))	//if even & < 2 nodes or if odd and < 3
	{
		//If there is room in the table[index] linked list, insert a new node into the list and return
		//its value field (also increment filled entries by 1 before returning if this insert causes table[index] to max out)
		node * newNode = new node(key, t2(), table[index]);		//make a new node with key, default value, and link->table[i]
		table[index] = newNode;		//head insert newNode

		nodeCount++;		//inc node count for linked list

		//also increment filled entries by 1 before returning if this insert causes table[index] to max out
		if (((index % 2) == 0 && nodeCount >= 2) || ((index % 2) != 0 && nodeCount >= 3))	//if the inserted node caused linked list to become full
		{
			filledEntries++;
		}
		return table[index]->value;
	}

	//If table[index] is maxed out, update index = (index + 1) % size and then go to step 3
	if (((index % 2 == 0) && (nodeCount >= 2)) || ((index % 2 != 0) && (nodeCount >= 3)))	//if linked list is maxed out
	{
		index = (index + 1) % size;
		goto step3;
	}
}

template <class t1, class t2>
void hashMap<t1, t2>::resize()
//function that doubles the size of the hash table (must rehash all of the entries non blank key entries into a larger table)
{
	size_t newSize = size * 2;			//get orginial size * 2

	node ** tmpTable = new node*[newSize];		//make a temp table to copy original table into

	//initialize new table to nullptr
	for (size_t i = 0; i < newSize; i++)
		tmpTable[i] = nullptr;

	for (size_t i = 0; i < size; i++)
	{
		node * x = table[i];	//need this to iterate through linked list
		while (x != nullptr)
		{
			size_t newIndex = hash(x->key) % newSize;	//calculate the new index for each key in each linked list
			node * xCurrentLink = x->link;		//save x's link so we can continue to iterate through the loop
			x->link = tmpTable[newIndex];			//head insert, tmp node point to current head node
			tmpTable[newIndex] = x;		//head insert complete
			x = xCurrentLink;		//next node
		}
	}

	delete[] table;		//delete original table
	table = nullptr;	//table to nullptr after delete

	table = tmpTable;	//set table as new table
	size = newSize;		//set size as the new size
}


template <class t1, class t2>
void hashMap<t1, t2>::copyHashMap(const hashMap<t1, t2>& otherMap)
//function that deep copies the otherMap object into the *this object
{
    //copy size and filledEntries from the map we are copying
    size = otherMap.size;
    filledEntries = otherMap.filledEntries;

    table = new node*[size];

	//initialize the table to nullptr
    for (size_t i = 0; i < size; i++)
        table[i] = nullptr;

    for (size_t i = 0; i < otherMap.size; i++)
	{
        node *tableIndex = otherMap.table[i];	//node* for what index of table we are at
        while (tableIndex != nullptr)
		{
            node *newNode = new node(tableIndex->key, tableIndex->value); //make a new node with info from current table index
            size_t index = hash(newNode->key) % size;	//new index
            
			//head insert
			newNode->link = table[index];	
            table[index] = newNode;

            //go next node
            tableIndex = tableIndex->link;
        }
    }
}

template <class t1, class t2>
void hashMap<t1, t2>::deallocateHashMap()
//function that deallocates table and sets size and filledEntries to 0
{
	for (size_t i = 0; i < size; i++)
	{
		node *tableIndex = table[i];

		//delete the linked lists
		while (tableIndex != nullptr)
		{
			node *tmp = tableIndex;
			tableIndex = tableIndex->link;
			delete tmp;
		}
	}

	delete[] table;		//delete table array
	table = nullptr;

    size = 0;
    filledEntries = 0;
}
