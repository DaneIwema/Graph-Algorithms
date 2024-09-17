//Implementation file for the Graph class
//by Dane Iwema

#include "Graph.h"
Graph::Graph() {}

bool Graph::readGraph(const string &inFileName) {
    numVertices = 0;
    numEdges = 0;
    ifstream infile(inFileName);
    if (!infile.is_open())
        return false;
    int count = getNumFromFile(infile);
    initialize(count);
    for (int i = 0; i < count; i++)
        addNode(getNodeFromFile(infile));
    count = getNumFromFile(infile);
    for (int i = 0; i < count; i ++) {
        string node1 = getNodeFromFile(infile);
        string node2 = getNodeFromFile(infile);
        addEdge(node1, node2, getNumFromFile(infile));
    }
    infile.close();
    return true;
}

void Graph::printGraph(ostream& outputStream) {
    cout << numVertices << endl;
    for (int i = 0; i < numVertices; i++)
        cout << vertexOrder[i] << endl;
    cout << numEdges << endl;
    for (int i = 0; i < numVertices; i++) {
        for (int j = 0; j < numVertices; j++) {
            if(adjMat[i][j] != 0)
                cout << vertexOrder[i] << " " << vertexOrder[j] << " " << adjMat[i][j] << endl;
        }
    }
}

void Graph::computeTopologicalSort(ostream& outputStream) {
    vector<int> indegree(numVertices, 0);
    for (int i = 0; i < numVertices; ++i)
        for (int j = 0; j < numVertices; ++j)
            if (adjMat[i][j] != 0)
                indegree[j] ++;
    queue<int> queue;
    for (int i = 0; i < numVertices; ++i)
        if (indegree[i] == 0)
            queue.push(i);
    if (queue.size() == 0) {
        cout << "This graph cannot be topologically sorted." << endl;
        return;
    }
    cout << "Topological Sort:" << endl;
    int source;
    int count = 0;
    while(!queue.empty()) {
        source = queue.front();
        queue.pop();
        cout << vertexOrder[source];
        count++;
        for (int destination = 0; destination < numVertices; destination++) {
            if (adjMat[source][destination] != 0) {
                indegree[destination]--;
                if (indegree[destination] == 0)
                    queue.push(destination);
            }
        }
        if (count < numVertices)
            cout << " --> ";
    }
    cout << endl;
}

void Graph::computeShortestPaths( ostream& outputStream, const string &source) {
    int src = indexMap[source];
    cout << "Shortest paths from " << source << ":" << endl;
    struct compare {
        bool operator()(pair<int, pair<int, int>>& source, pair<int, pair<int, int>>& destination) {
            return source.second.second > destination.second.second;
        }
    };
    priority_queue<pair<int, pair<int, int>>, vector<pair<int, pair<int, int>>>, compare> pqueue;
    vector<pair<int, pair<int, bool>>> pathCost(numVertices, {0, {0, false}});
    pqueue.push({0, {src, 0}});
    pathCost[src].second.second = true;
    int checked = 0;
    while(!pqueue.empty()) {
        pair<int, pair<int, int>> node = pqueue.top();
        pqueue.pop();
        pathCost[node.second.first] = {node.first ,{node.second.second ,true}};
        checked++;
        for (int adj = 0; adj < numVertices; adj++) {
            if (adjMat[node.second.first][adj] != 0 && !pathCost[adj].second.second) {
                pqueue.push({node.second.first ,{adj ,node.second.second + adjMat[node.second.first][adj]}});
            }
        }
        if (checked == numVertices)
            break;
    }
    for (int i = 0; i < numVertices; i++) {
        if (!pathCost[i].second.second)
            cout << "No path from " << source << " to "  << vertexOrder[i] << " found." << endl;
        else if (src != i) {
            printPath(src, i, pathCost);
            cout << " || Weight: " << pathCost[i].second.first << endl;
        }
    }
}

void Graph::printPath(int src, int dest, const vector<pair<int, pair<int, bool>>>& pathCost) {
    if (src != dest) {
        printPath(src, pathCost[dest].first, pathCost);
        cout << " --> ";
    }
    cout << vertexOrder[dest];
}

void Graph::computeMinimumSpanningTree(ostream& outputStream) {
    cout << "Minimum Spanning Tree:" << endl;
    struct compare {
        bool operator()(pair<int, pair<int, int>>& source, pair<int, pair<int, int>>& destination) {
            if (source.second.second != destination.second.second)
                return source.second.second > destination.second.second;
            else
                return source.first > destination.first;
        }
    };
    priority_queue<pair<int, pair<int, int>>, vector<pair<int, pair<int, int>>>, compare> edges;
    for (int i = 0; i < numVertices; i++)
        for (int j = 0; j < numVertices; j++)
            if (adjMat[i][j] != 0)
                //cout << vertexOrder[i] << " -- " << vertexOrder[j] << " || Weight: " << adjMat[i][j] << endl;
                edges.push({i ,{j ,adjMat[i][j]}});
    DisjointSet spanningTree(numVertices);
    int cost = 0;
    int count = 0;
    while (!edges.empty()) {
        
        pair<int, pair<int, int>> edge = edges.top();
        edges.pop();
        //cout << vertexOrder[edge.first] << " -- " << vertexOrder[edge.second.first] << " || Weight: " << edge.second.second << endl;

        if (spanningTree.find(edge.first) != spanningTree.find(edge.second.first)) {
            spanningTree.doUnion(edge.first, edge.second.first);
            cout << vertexOrder[edge.first] << " -- " << vertexOrder[edge.second.first] << " || Weight: " << edge.second.second << endl;
            cost +=edge.second.second;
            count++;
        }
        if (count == numVertices)
            break;
    }
    cout << "Total Cost: " << cost << endl;
}

void Graph::addNode(const string& node) {
    indexMap[node] = numVertices;
    vertexOrder.push_back(node);
    numVertices++;
}

void Graph::addEdge(const string& node1, const string& node2, int weight) {
    adjMat[indexMap[node1]][indexMap[node2]] = weight;
    numEdges++;
}

void Graph::initialize(int size) {
    adjMat.clear();
    indexMap.clear();
    vertexOrder.clear();
    for (int i = 0; i < size; i++)
        adjMat.push_back(vector<int>(size, 0));
    indexMap.reserve(size);
}

int Graph::getNumFromFile(ifstream& infile) {
    char token;
    int theNum = 0;
    while(infile.get(token) && token != '\n') {
        theNum *= 10;
        theNum += token - '0';
    }
    return theNum;
}

string Graph::getNodeFromFile(ifstream& infile) {
    char token;
    string node = "";
    while(infile.get(token) && token != '\n' && token != ' ')
        node += token;
    return node;
}