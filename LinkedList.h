#pragma once
// LinkedList.h
#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#include <iostream>
#include <string>
using namespace std;

// Node for bus route stops
class BusStopNode {
public:
    string stopId;          // "Stop1", "Stop2", etc.
    BusStopNode* next;      // Pointer to next stop

    // Constructor
    BusStopNode(string id) {
        stopId = id;
        next = nullptr;
    }
};

// Singly Linked List for bus route
class BusRoute {
private:
    BusStopNode* head;      // First stop
    BusStopNode* tail;      // Last stop
    int stopCount;          // Number of stops
    string busNumber;       // Bus identifier

public:
    // Constructor
    BusRoute(string busNo) {
        head = tail = nullptr;
        stopCount = 0;
        busNumber = busNo;
    }

    // Destructor
    ~BusRoute() {
        clearRoute();
    }

    // Add stop to end of route
    void addStop(string stopId) {
        BusStopNode* newNode = new BusStopNode(stopId);

        if (head == nullptr) {
            // First stop
            head = tail = newNode;
        }
        else {
            // Append to end
            tail->next = newNode;
            tail = newNode;
        }

        stopCount++;
        cout << "Added stop " << stopId << " to Bus " << busNumber << endl;
    }

    // Insert stop at specific position (1-based index)
    bool insertStopAt(string stopId, int position) {
        if (position < 1 || position > stopCount + 1) {
            cout << "ERROR: Invalid position " << position << endl;
            return false;
        }

        BusStopNode* newNode = new BusStopNode(stopId);

        if (position == 1) {
            // Insert at beginning
            newNode->next = head;
            head = newNode;
            if (tail == nullptr) tail = newNode;
        }
        else {
            // Find node before position
            BusStopNode* current = head;
            for (int i = 1; i < position - 1; i++) {
                current = current->next;
            }

            newNode->next = current->next;
            current->next = newNode;

            if (newNode->next == nullptr) {
                tail = newNode;  // New tail
            }
        }

        stopCount++;
        cout << "Inserted stop " << stopId << " at position " << position << endl;
        return true;
    }

    // Remove stop by ID
    bool removeStop(string stopId) {
        if (head == nullptr) {
            cout << "ERROR: Route is empty!\n";
            return false;
        }

        // Check if head needs to be removed
        if (head->stopId == stopId) {
            BusStopNode* temp = head;
            head = head->next;
            if (head == nullptr) tail = nullptr;  // List became empty
            delete temp;
            stopCount--;
            cout << "Removed stop " << stopId << endl;
            return true;
        }

        // Search for node to remove
        BusStopNode* current = head;
        while (current->next != nullptr && current->next->stopId != stopId) {
            current = current->next;
        }

        if (current->next == nullptr) {
            cout << "ERROR: Stop " << stopId << " not found in route\n";
            return false;
        }

        BusStopNode* temp = current->next;
        current->next = temp->next;

        if (temp == tail) {
            tail = current;  // Update tail if last node removed
        }

        delete temp;
        stopCount--;
        cout << "Removed stop " << stopId << endl;
        return true;
    }

    // Display entire route
    void displayRoute() {
        if (head == nullptr) {
            cout << "Bus " << busNumber << " route is empty.\n";
            return;
        }

        cout << "\n=== Bus " << busNumber << " Route ===\n";
        cout << "Total stops: " << stopCount << endl;
        cout << "Route: ";

        BusStopNode* current = head;
        int count = 1;
        while (current != nullptr) {
            cout << current->stopId;
            if (current->next != nullptr) {
                cout << " -> ";
            }

            // Formatting: 3 stops per line
            if (count % 3 == 0 && current->next != nullptr) {
                cout << "\n        ";  // Indent for next line
            }

            current = current->next;
            count++;
        }
        cout << "\n=========================\n";
    }

    // Get first stop
    string getFirstStop() {
        return (head != nullptr) ? head->stopId : "";
    }

    // Get last stop
    string getLastStop() {
        return (tail != nullptr) ? tail->stopId : "";
    }

    // Get stop count
    int getStopCount() {
        return stopCount;
    }

    // Check if route contains a stop
    bool containsStop(string stopId) {
        BusStopNode* current = head;
        while (current != nullptr) {
            if (current->stopId == stopId) {
                return true;
            }
            current = current->next;
        }
        return false;
    }

    // Clear entire route
    void clearRoute() {
        while (head != nullptr) {
            BusStopNode* temp = head;
            head = head->next;
            delete temp;
        }
        head = tail = nullptr;
        stopCount = 0;
        cout << "Bus " << busNumber << " route cleared.\n";
    }

    // Get bus number
    string getBusNumber() {
        return busNumber;
    }
};

#endif // LINKEDLIST_H