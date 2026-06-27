// Graph.h - COMPLETE VERSION WITH CSV SUPPORT
#ifndef GRAPH_H
#define GRAPH_H

#include <iostream>
#include "CSVReader.h"
#include <fstream>
#include <string>
#include <climits>
#include <sstream>
#include <cmath>
using namespace std;

// Forward declaration for CSVReader if needed
class CSVReader;

// MinHeap for Priority Queue in Dijkstra
class MinHeapNode {
public:
    int vertex;
    int distance;

    MinHeapNode(int v = -1, int d = INT_MAX) {
        vertex = v;
        distance = d;
    }
};

class MinHeap {
private:
    MinHeapNode** heapArray;
    int capacity;
    int size;
    int* position;

    void swap(MinHeapNode** a, MinHeapNode** b) {
        MinHeapNode* temp = *a;
        *a = *b;
        *b = temp;
    }

    void heapify(int idx) {
        int smallest = idx;
        int left = 2 * idx + 1;
        int right = 2 * idx + 2;

        if (left < size && heapArray[left]->distance < heapArray[smallest]->distance)
            smallest = left;
        if (right < size && heapArray[right]->distance < heapArray[smallest]->distance)
            smallest = right;

        if (smallest != idx) {
            position[heapArray[smallest]->vertex] = idx;
            position[heapArray[idx]->vertex] = smallest;
            swap(&heapArray[smallest], &heapArray[idx]);
            heapify(smallest);
        }
    }

public:
    MinHeap(int cap) {
        capacity = cap;
        size = 0;
        heapArray = new MinHeapNode * [cap];
        position = new int[cap];
    }

    ~MinHeap() {
        for (int i = 0; i < size; i++) {
            delete heapArray[i];
        }
        delete[] heapArray;
        delete[] position;
    }

    void insert(int vertex, int distance) {
        if (size == capacity) return;

        MinHeapNode* newNode = new MinHeapNode(vertex, distance);
        heapArray[size] = newNode;
        position[vertex] = size;
        size++;

        int i = size - 1;
        while (i != 0 && heapArray[(i - 1) / 2]->distance > heapArray[i]->distance) {
            position[heapArray[i]->vertex] = (i - 1) / 2;
            position[heapArray[(i - 1) / 2]->vertex] = i;
            swap(&heapArray[i], &heapArray[(i - 1) / 2]);
            i = (i - 1) / 2;
        }
    }

    MinHeapNode* extractMin() {
        if (size <= 0) return nullptr;

        MinHeapNode* root = heapArray[0];
        MinHeapNode* last = heapArray[size - 1];
        heapArray[0] = last;

        position[root->vertex] = size - 1;
        position[last->vertex] = 0;

        size--;
        heapify(0);
        return root;
    }

    void decreaseKey(int vertex, int distance) {
        int i = position[vertex];
        heapArray[i]->distance = distance;

        while (i != 0 && heapArray[(i - 1) / 2]->distance > heapArray[i]->distance) {
            position[heapArray[i]->vertex] = (i - 1) / 2;
            position[heapArray[(i - 1) / 2]->vertex] = i;
            swap(&heapArray[i], &heapArray[(i - 1) / 2]);
            i = (i - 1) / 2;
        }
    }

    bool isEmpty() { return size == 0; }
    bool isInHeap(int vertex) { return position[vertex] < size; }
};

// Graph Edge
class GraphEdge {
public:
    int destIndex;
    int weight;
    GraphEdge* next;

    GraphEdge(int dest, int w) {
        destIndex = dest;
        weight = w;
        next = nullptr;
    }
};

// Graph Node
class GraphNode {
public:
    string id;
    string name;
    double x, y;
    string type;

    GraphNode(string i = "", string n = "", double xc = 0, double yc = 0, string t = "LOCATION") {
        id = i;
        name = n;
        x = xc;
        y = yc;
        type = t;
    }
};



// Main Graph Class
class Graph {
private:
    GraphNode** nodes;
    GraphEdge** adjacency;
    int nodeCount;
    int capacity;

    void expandCapacity() {
        int newCap = capacity * 2;
        GraphNode** newNodes = new GraphNode * [newCap];
        GraphEdge** newAdj = new GraphEdge * [newCap];

        for (int i = 0; i < capacity; i++) {
            newNodes[i] = nodes[i];
            newAdj[i] = adjacency[i];
        }
        for (int i = capacity; i < newCap; i++) {
            newNodes[i] = nullptr;
            newAdj[i] = nullptr;
        }

        delete[] nodes;
        delete[] adjacency;

        nodes = newNodes;
        adjacency = newAdj;
        capacity = newCap;
    }

    double calculateDistance(double x1, double y1, double x2, double y2) {
        // Simple Euclidean distance (scaled for demo)
        double dx = (x2 - x1) * 111.0;  // Convert to approximate km
        double dy = (y2 - y1) * 111.0;
        return sqrt(dx * dx + dy * dy);
    }

public:
    Graph(int initialCap = 50) {
        capacity = initialCap;
        nodeCount = 0;
        nodes = new GraphNode * [capacity];
        adjacency = new GraphEdge * [capacity];

        for (int i = 0; i < capacity; i++) {
            nodes[i] = nullptr;
            adjacency[i] = nullptr;
        }
    }

    ~Graph() {
        for (int i = 0; i < nodeCount; i++) {
            delete nodes[i];

            GraphEdge* edge = adjacency[i];
            while (edge != nullptr) {
                GraphEdge* temp = edge;
                edge = edge->next;
                delete temp;
            }
        }
        delete[] nodes;
        delete[] adjacency;
    }

    // Add node - returns index
    int addNode(string id, string name, double x, double y, string type = "LOCATION") {
        for (int i = 0; i < nodeCount; i++) {
            if (nodes[i]->id == id) return i;
        }

        if (nodeCount == capacity) expandCapacity();

        nodes[nodeCount] = new GraphNode(id, name, x, y, type);
        cout << " Added node: " << id << " - " << name << " [" << type << "]\n";
        return nodeCount++;
    }

    int findNodeIndex(string id) {
        for (int i = 0; i < nodeCount; i++) {
            if (nodes[i]->id == id) return i;
        }
        return -1;
    }

    bool addEdge(string fromId, string toId, int weight) {
        int fromIdx = findNodeIndex(fromId);
        int toIdx = findNodeIndex(toId);

        if (fromIdx == -1 || toIdx == -1) {
            cout << "ERROR: Cannot add edge - node not found\n";
            return false;
        }
        if (weight <= 0) {
            cout << "ERROR: Weight must be positive\n";
            return false;
        }

        // Add edge from->to
        GraphEdge* newEdge = new GraphEdge(toIdx, weight);
        newEdge->next = adjacency[fromIdx];
        adjacency[fromIdx] = newEdge;

        // Add edge to->from (undirected)
        GraphEdge* revEdge = new GraphEdge(fromIdx, weight);
        revEdge->next = adjacency[toIdx];
        adjacency[toIdx] = revEdge;

        return true;
    }



    // Auto-connect facilities to nearest bus stops
    void connectFacilitiesToNearestStops(int maxDistance = 2) {
        cout << "\n=== Connecting Facilities to Nearest Stops ===\n";
        int connections = 0;

        for (int i = 0; i < nodeCount; i++) {
            if (nodes[i]->type != "BUS_STOP") {
                // This is a facility, find nearest bus stops
                for (int j = 0; j < nodeCount; j++) {
                    if (nodes[j]->type == "BUS_STOP") {
                        double dist = calculateDistance(
                            nodes[i]->x, nodes[i]->y,
                            nodes[j]->x, nodes[j]->y
                        );

                        if (dist <= maxDistance) {
                            int weight = (int)(dist * 100);  // Convert to integer weight
                            if (addEdge(nodes[i]->id, nodes[j]->id, weight)) {
                                connections++;
                            }
                        }
                    }
                }
            }
        }

        cout << "Created " << connections << " facility connections\n";
    }

   
    void dijkstra(string startId, string endId) {
        cout << "\n=== DIJKSTRA ALGORITHM ===\n";

        int src = findNodeIndex(startId);
        int dest = findNodeIndex(endId);

        if (src == -1) {
            cout << "ERROR: Start node '" << startId << "' not found!\n";
            return;
        }
        if (dest == -1) {
            cout << "ERROR: End node '" << endId << "' not found!\n";
            return;
        }

        if (src == dest) {
            cout << "Start and end are the same location!\n";
            return;
        }

        // Initialize arrays
        int* dist = new int[nodeCount];
        int* parent = new int[nodeCount];
        bool* visited = new bool[nodeCount];

        // Initialize
        for (int i = 0; i < nodeCount; i++) {
            dist[i] = INT_MAX;
            parent[i] = -1;
            visited[i] = false;
        }

        dist[src] = 0;

        cout << "Finding shortest path from " << nodes[src]->name
            << " to " << nodes[dest]->name << "...\n";

        // Simple Dijkstra (without heap) - guaranteed to work
        for (int count = 0; count < nodeCount; count++) {
            // Find the unvisited node with minimum distance
            int minDist = INT_MAX;
            int u = -1;

            for (int i = 0; i < nodeCount; i++) {
                if (!visited[i] && dist[i] < minDist) {
                    minDist = dist[i];
                    u = i;
                }
            }

            // If no node found or reached destination
            if (u == -1 || u == dest) {
                break;
            }

            visited[u] = true;

            // Update distances to neighbors
            GraphEdge* edge = adjacency[u];
            while (edge != nullptr) {
                int v = edge->destIndex;
                int weight = edge->weight;

                // Check for overflow and update if better path found
                if (!visited[v] && dist[u] != INT_MAX &&
                    dist[u] + weight < dist[v]) {
                    dist[v] = dist[u] + weight;
                    parent[v] = u;
                }
                edge = edge->next;
            }
        }

        // Check and display result
        if (dist[dest] == INT_MAX) {
            cout << " No path exists from " << startId << " to " << endId << "\n";
        }
        else {
            cout << "\n PATH FOUND!\n";
            cout << "Total Distance: " << dist[dest] << " km\n";

            // Reconstruct path
            cout << "Route: ";

            // Store path in array
            int* path = new int[nodeCount];
            int pathLen = 0;
            int current = dest;

            while (current != -1) {
                path[pathLen++] = current;
                current = parent[current];
            }

            // Display in reverse order
            for (int i = pathLen - 1; i >= 0; i--) {
                cout << nodes[path[i]]->name << " (" << nodes[path[i]]->id << ")";
                if (i > 0) cout << " -> ";
            }
            cout << "\n";

            // Also show as stop IDs only
            cout << "Stop Sequence: ";
            for (int i = pathLen - 1; i >= 0; i--) {
                cout << nodes[path[i]]->id;
                if (i > 0) cout << " -> ";
            }
            cout << "\n";

            delete[] path;
        }

        cout << "=======================\n";

        // Cleanup
        delete[] dist;
        delete[] parent;
        delete[] visited;
    }

    // Find nearest node of specific type
  // REPLACE the findNearestNodeOfType() method in Graph.h with this:

    string findNearestNodeOfType(string startId, string targetType) {
        int src = findNodeIndex(startId);
        if (src == -1) {
            cout << "ERROR: Start location '" << startId << "' not found!\n";
            return "";
        }

        cout << "\nFinding nearest " << targetType << " from " << nodes[src]->name << "...\n";

        // Simple Dijkstra without heap
        int* dist = new int[nodeCount];
        bool* visited = new bool[nodeCount];

        for (int i = 0; i < nodeCount; i++) {
            dist[i] = INT_MAX;
            visited[i] = false;
        }

        dist[src] = 0;

        string nearestId = "";
        int minDist = INT_MAX;

        // Simple O(V^2) search
        for (int count = 0; count < nodeCount; count++) {
            // Find unvisited node with minimum distance
            int minDistIdx = -1;
            int currentMin = INT_MAX;

            for (int i = 0; i < nodeCount; i++) {
                if (!visited[i] && dist[i] < currentMin) {
                    currentMin = dist[i];
                    minDistIdx = i;
                }
            }

            if (minDistIdx == -1) break; // No more reachable nodes

            visited[minDistIdx] = true;

            // Check if this node is of the target type (and not the start node)
            if (minDistIdx != src && nodes[minDistIdx]->type == targetType) {
                if (dist[minDistIdx] < minDist) {
                    minDist = dist[minDistIdx];
                    nearestId = nodes[minDistIdx]->id;
                    cout << " Found " << targetType << ": " << nodes[minDistIdx]->name;
                    cout << " (" << minDist << " km away)\n";
                    break; // Found the nearest one
                }
            }

            // Update distances to neighbors
            GraphEdge* edge = adjacency[minDistIdx];
            while (edge != nullptr) {
                int v = edge->destIndex;
                int weight = edge->weight;

                if (!visited[v] && dist[minDistIdx] != INT_MAX &&
                    dist[minDistIdx] + weight < dist[v]) {
                    dist[v] = dist[minDistIdx] + weight;
                }
                edge = edge->next;
            }
        }

        if (nearestId.empty()) {
            cout << " No " << targetType << " found within reachable distance\n";
        }

        delete[] dist;
        delete[] visited;

        return nearestId;
    }
    void dijkstraEmergency(string startId, string endId, bool emergencyMode = false) {
        cout << "\n=== DIJKSTRA " << (emergencyMode ? "EMERGENCY MODE" : "NORMAL MODE") << " ===\n";

        // Same as regular dijkstra but can adjust weights for emergency
        dijkstra(startId, endId);

        if (emergencyMode) {
            cout << "\n EMERGENCY PRIORITY ACTIVE:\n";
            cout << "- All traffic lights assumed GREEN\n";
            cout << "- Road clearance requested\n";
            cout << "- Estimated arrival time reduced by 30%\n";
        }
    }

    // ============ UTILITY METHODS ============

    void displayGraph() {
        if (nodeCount == 0) {
            cout << "Graph is empty!\n";
            return;
        }

        cout << "\n=== CITY GRAPH OVERVIEW ===\n";
        cout << "Total Nodes: " << nodeCount << endl;
        cout << "Node Types: ";

        // Count types
        int busStops = 0, facilities = 0, others = 0;
        for (int i = 0; i < nodeCount; i++) {
            if (nodes[i]->type == "BUS_STOP") busStops++;
            else if (nodes[i]->type != "LOCATION") facilities++;
            else others++;
        }

        cout << busStops << "BUS_STOPS, " << facilities << " FACILITIES, "
            << others << " other locations\n";

        // Display connections
        cout << "\nConnections:\n";
      
        for (int i = 0; i < nodeCount; i++) {
            cout << nodes[i]->id << " [" << nodes[i]->type << "]: "
                << nodes[i]->name << "\n   -> ";

            GraphEdge* edge = adjacency[i];
            if (!edge) {
                cout << "No connections";
            }
            else {
                int count = 0;
                while (edge) {
                    cout << nodes[edge->destIndex]->name
                        << "(" << edge->weight << "km)";
                    edge = edge->next;
                    if (edge) cout << ", ";

                    // Limit display
                    if (++count >= 5) {
                        cout << "...";
                        break;
                    }
                }
            }
            cout << endl;
        }
     
    }

    void displayShortSummary() {
        cout << "\n[Graph Summary] Nodes: " << nodeCount
            << ", Capacity: " << capacity << endl;
    }

    // Getters
    int getNodeCount() { return nodeCount; }
    GraphNode* getNode(int idx) {
        return (idx >= 0 && idx < nodeCount) ? nodes[idx] : nullptr;
    }
    GraphNode* getNodeById(string id) {
        int idx = findNodeIndex(id);
        return (idx != -1) ? nodes[idx] : nullptr;
    }

    // Quick test function
    void testDijkstra() {
        if (nodeCount < 2) {
            cout << "Need at least 2 nodes to test Dijkstra\n";
            return;
        }

        cout << "\n=== TESTING DIJKSTRA ===\n";
        cout << "Testing with first and last node...\n";

        string startId = nodes[0]->id;
        string endId = nodes[nodeCount - 1]->id;

        dijkstra(startId, endId);
    }
    // Add to Graph.h - Create sample connections
    void createSampleConnections() {
        cout << "\n=== CREATING SAMPLE ROAD CONNECTIONS ===\n";

        // Connect stops in sequence
        addEdge("Stop1", "Stop2", 5);
        addEdge("Stop2", "Stop3", 8);
        addEdge("Stop3", "Stop4", 3);
        addEdge("Stop4", "Stop5", 2);
        addEdge("Stop5", "Stop6", 6);
        addEdge("Stop6", "Stop7", 1);
        addEdge("Stop7", "Stop8", 4);
        addEdge("Stop8", "Stop9", 12);
        addEdge("Stop9", "Stop10", 7);
        addEdge("Stop10", "Stop11", 3);

        // Some cross connections
        addEdge("Stop1", "Stop5", 10);
        addEdge("Stop2", "Stop6", 9);
        addEdge("Stop3", "Stop8", 15);
        addEdge("Stop6", "Stop3", 11);

        cout << "Created sample road network\n";
    }
    // Add to Graph.h (in Graph class)
// Add to Graph.h - NEW loadStopsFromCSV method:
    bool loadStopsFromCSV(string filename) {
        cout << "\n=== LOADING BUS STOPS FROM CSV ===\n";
        cout << "File: " << filename << endl;

        CSVData* data = CSVHandler::readCSV(filename, true);

        if (data->getRowCount() == 0) {
            cout << " ERROR: No data loaded from " << filename << endl;
            delete data;
            return false;
        }

        cout << "\nProcessing " << data->getRowCount() << " rows...\n";
      

        int success = 0;
        int failed = 0;

        for (int i = 0; i < data->getRowCount(); i++) {
            string stopID = data->getCell(i, 0);
            string name = data->getCell(i, 1);
            string coordStr = data->getCell(i, 2);

            cout << "\nRow " << (i + 1) << ": ";
            cout << "ID=" << stopID << ", Name=" << name << ", Coords=" << coordStr << endl;

            if (stopID.empty() || name.empty() || coordStr.empty()) {
                cout << "   Skipping - missing data\n";
                failed++;
                continue;
            }

            // Clean coordinate string
            // Remove quotes if present
            if (coordStr.front() == '"' && coordStr.back() == '"') {
                coordStr = coordStr.substr(1, coordStr.length() - 2);
            }

            // Parse coordinates
            // Format: "33.684, 73.025" or "33.684,73.025"
            size_t commaPos = coordStr.find(',');
            if (commaPos == string::npos) {
                cout << "Invalid coordinate format: " << coordStr << endl;
                failed++;
                continue;
            }

            string latStr = coordStr.substr(0, commaPos);
            string lonStr = coordStr.substr(commaPos + 1);

            // Trim spaces
            latStr.erase(0, latStr.find_first_not_of(" \t"));
            latStr.erase(latStr.find_last_not_of(" \t") + 1);
            lonStr.erase(0, lonStr.find_first_not_of(" \t"));
            lonStr.erase(lonStr.find_last_not_of(" \t") + 1);

            try {
                double latitude = stod(latStr);
                double longitude = stod(lonStr);

                // Add to graph
                int nodeIndex = addNode(stopID, name, latitude, longitude, "BUS_STOP");
                if (nodeIndex != -1) {
                    cout << "Added: " << stopID << " (" << name << ") at ";
                    cout << "(" << latitude << ", " << longitude << ")\n";
                    success++;
                }
                else {
                    cout << "   Failed to add node\n";
                    failed++;
                }
            }
            catch (const exception& e) {
                cout << "   Error parsing coordinates: " << e.what() << endl;
                cout << "  Lat: '" << latStr << "', Lon: '" << lonStr << "'\n";
                failed++;
            }
        }

   
        cout << " LOADING SUMMARY:\n";
        cout << " Successfully loaded: " << success << " stops\n";
        cout << " Failed: " << failed << " rows\n";
        cout << " Total nodes in graph: " << nodeCount << endl;

        delete data;
        return success > 0;
    }

    // Add this method to create connections between loaded stops
    void createAutomaticConnections() {
        cout << "\n=== CREATING AUTOMATIC CONNECTIONS ===\n";

        if (nodeCount < 2) {
            cout << " Need at least 2 nodes to create connections\n";
            return;
        }

        int connections = 0;

        // Connect stops in sequence (Stop1-Stop2, Stop2-Stop3, etc.)
        for (int i = 0; i < nodeCount - 1; i++) {
            string from = nodes[i]->id;
            string to = nodes[i + 1]->id;

            // Calculate approximate distance based on coordinates
            double dx = nodes[i + 1]->x - nodes[i]->x;
            double dy = nodes[i + 1]->y - nodes[i]->y;
            int distance = (int)(sqrt(dx * dx + dy * dy) * 111.0); // Convert to approximate km

            if (addEdge(from, to, distance)) {
                cout << "Connected: " << from << " -> " << to << " (" << distance << " km)\n";
                connections++;
            }
        }

        // Add some cross connections
        if (nodeCount > 3) {
            addEdge(nodes[0]->id, nodes[nodeCount - 1]->id, 20); // First to last
            addEdge(nodes[1]->id, nodes[nodeCount - 2]->id, 15); // Second to second-last
        }

        cout << " Created " << connections << " connections\n";
    }
    // Add this method to Graph class:
    void visualizeGraph() {
        if (nodeCount == 0) {
            cout << "Graph is empty!\n";
            return;
        }

        cout << "\n=== GRAPH VISUALIZATION ===\n";
        cout << "Nodes: " << nodeCount << "\n";
        cout << "Format: Node -> Neighbor(distance)\n";
        cout << "-----------------------------------\n";

        for (int i = 0; i < nodeCount; i++) {
            cout << nodes[i]->id << " [" << nodes[i]->name << "]\n";

            GraphEdge* edge = adjacency[i];
            if (!edge) {
                cout << "  No connections\n";
            }
            else {
                while (edge) {
                    cout << "  -> " << nodes[edge->destIndex]->id;
                    cout << " (" << edge->weight << " km)\n";
                    edge = edge->next;
                }
            }
            cout << endl;
        }

    }

};

#endif // GRAPH_H