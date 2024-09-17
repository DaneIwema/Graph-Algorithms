// DisjointSet implementation using union by size and path compression
// By Mary Elaine Califf and Dane Iwema

#include "DisjointSet.h"
#include <iostream>

DisjointSet::DisjointSet(int numObjects)
{
    theArray = std::vector<int>(numObjects, -1);
    numValues = numObjects;
}

//recursive method to find the item -- does path compression on the way out of the recursion
int DisjointSet::find(int objectIndex)
{
    if (theArray[objectIndex] < 0)
        return objectIndex;
    theArray[objectIndex] = find(theArray[objectIndex]);
    return theArray[objectIndex];
}

bool DisjointSet::doUnion(int objIndex1, int objIndex2)
{
    
    int parent1 = find(objIndex1);
    int parent2 = find(objIndex2);
    if (parent1 != parent2) {
        if (theArray[parent1] > theArray[parent2]) {
            theArray[parent2] += theArray[parent1];
            theArray[parent1] = parent2;
        } else {
            theArray[parent1] += theArray[parent2];
            theArray[parent2] = parent1;
        }
        return (theArray[parent1] == -(int)theArray.size() || theArray[parent2] == -(int)theArray.size());
    }
    return false;
}

void DisjointSet::printArrayValues(std::ostream &outputStream)
{
    for (int i = 0; i < numValues; i++)
    {
        outputStream << theArray[i] << " ";
    }
    outputStream << std::endl;
}
