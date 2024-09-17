//Graph Class implementation using an adjacency matrix
//By Dane Iwema

#ifndef GRAPH_H
#define GRAPH_H

#include <unordered_map>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <queue>
#include "DisjointSet.h"
using namespace std;

class Graph
{
private:
    vector<vector<int>> adjMat; //adjacency matrix
    unordered_map<string, int> indexMap; //hashtable used search by string
    vector<string> vertexOrder; //vector used to search for the name by index
    int numVertices;
    int numEdges;

public:
    //constructor
    Graph();

    //reads from a text file to build the graph
    bool readGraph(const string &inFileName);

    //prints the graph to the given output
    void printGraph(ostream& outputStream);

    //Sorts the graph Topologically and prints it to the given output
    void computeTopologicalSort(ostream& outputStream);

    //Finds the shortest path to the given node and prints to the given output
    void computeShortestPaths( ostream& outputStream, const string &node);
    
    //find the minimum spanning tree of the graph and prints it to the given output
    void computeMinimumSpanningTree(ostream& outputStream);

    //helper method to add nodes to the graph
    void addNode(const string& node);

    //helper method to add edges to the graph
    void addEdge(const string& node1, const string& node2, int weight);

private:
    //helper method to clear the previous graph
    // and to set the vector sizes from the first added number
    void initialize(int size);

    //helper method to get the integer of the next line ends at the new line
    int getNumFromFile(ifstream& infile);

    //helper method to read the next node from the file ends on a new line or space
    string getNodeFromFile(ifstream& infile);

    //recursive helper method for computeSHortestPath()
    //used to print the path and the weight of the given source, destination, and pathCost vector
    void printPath(int src, int dest, const vector<pair<int, pair<int, bool>>>& pathCost);
};

#endif
