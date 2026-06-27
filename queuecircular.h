#pragma once
// Queue.h - CIRCULAR QUEUE FOR PASSENGER SIMULATION
#ifndef QUEUE_H
#define QUEUE_H

#include <iostream>
#include <string>
using namespace std;

#define QUEUE_SIZE 50  // Maximum passengers in queue

// Passenger structure for queue
struct QueuePassenger {
    string passengerId;
    string name;
    string destinationStop;
    int waitingTime;  // in minutes
};

class CircularQueue {
private:
    QueuePassenger queue[QUEUE_SIZE];  // Fixed-size circular array
    int front;      // Points to first element
    int rear;       // Points to last element
    int count;      // Current number of passengers

public:
    // Constructor
    CircularQueue() {
        front = 0;
        rear = -1;
        count = 0;

        // Initialize all slots as empty
        for (int i = 0; i < QUEUE_SIZE; i++) {
            queue[i].passengerId = "";
            queue[i].name = "";
            queue[i].destinationStop = "";
            queue[i].waitingTime = 0;
        }
    }

    // ============ QUEUE OPERATIONS ============

    // Check if queue is empty
    bool isEmpty() {
        return (count == 0);
    }

    // Check if queue is full
    bool isFull() {
        return (count == QUEUE_SIZE);
    }

    // Get current queue size
    int getSize() {
        return count;
    }

    // Get queue capacity
    int getCapacity() {
        return QUEUE_SIZE;
    }

    // ============ ENQUEUE - Add passenger to queue ============
    bool enqueue(string pid, string name, string destination) {
        if (isFull()) {
            cout << " Queue is FULL! Cannot add passenger.\n";
            return false;
        }

        // Circular increment of rear
        rear = (rear + 1) % QUEUE_SIZE;

        // Add passenger at rear
        queue[rear].passengerId = pid;
        queue[rear].name = name;
        queue[rear].destinationStop = destination;
        queue[rear].waitingTime = 0;  // Just joined

        count++;

        cout << " Passenger " << name << " (" << pid << ") joined queue\n";
        cout << "  Destination: " << destination << endl;
        cout << "  Position in queue: " << count << endl;

        return true;
    }

    // ============ DEQUEUE - Remove passenger from queue ============
    QueuePassenger dequeue() {
        QueuePassenger emptyPassenger = { "", "", "", 0 };

        if (isEmpty()) {
            cout << " Queue is EMPTY! No passenger to remove.\n";
            return emptyPassenger;
        }

        // Get passenger at front
        QueuePassenger passenger = queue[front];

        // Clear the slot
        queue[front].passengerId = "";
        queue[front].name = "";
        queue[front].destinationStop = "";
        queue[front].waitingTime = 0;

        // Circular increment of front
        front = (front + 1) % QUEUE_SIZE;
        count--;

        cout << " Passenger " << passenger.name << " (" << passenger.passengerId << ") removed\n";
        cout << "  They were waiting for " << passenger.waitingTime << " minutes\n";

        return passenger;
    }

    // ============ PEEK - View passenger at front ============
    QueuePassenger peek() {
        QueuePassenger emptyPassenger = { "", "", "", 0 };

        if (isEmpty()) {
            cout << "Queue is empty!\n";
            return emptyPassenger;
        }

        return queue[front];
    }

    // ============ UPDATE WAITING TIME ============
    void updateWaitingTime(int minutes = 1) {
        if (isEmpty()) return;

        // Update waiting time for ALL passengers in queue
        int index = front;
        for (int i = 0; i < count; i++) {
            queue[index].waitingTime += minutes;
            index = (index + 1) % QUEUE_SIZE;
        }

        cout << " Updated waiting time: +" << minutes << " minute(s)\n";
    }

    // ============ DISPLAY QUEUE ============
    void displayQueue() {
        if (isEmpty()) {
            cout << "\n=== PASSENGER QUEUE ===\n";
            cout << "Queue is EMPTY. No passengers waiting.\n";
            return;
        }

        cout << "\n=== PASSENGER QUEUE ===\n";
        cout << "Total passengers waiting: " << count << "/" << QUEUE_SIZE << endl;
        cout << "Front -> [" << front << "], Rear -> [" << rear << "]\n";
        cout << "----------------------------------------------------------\n";
        cout << "Pos\tPassenger ID\tName\t\tDestination\tWait Time\n";
        cout << "----------------------------------------------------------\n";

        int index = front;
        for (int i = 0; i < count; i++) {
            cout << (i + 1) << "\t";
            cout << queue[index].passengerId << "\t\t";

            // Truncate long names
            if (queue[index].name.length() > 8)
                cout << queue[index].name.substr(0, 6) << "..\t";
            else
                cout << queue[index].name << "\t\t";

            cout << queue[index].destinationStop << "\t\t";
            cout << queue[index].waitingTime << " min\n";

            index = (index + 1) % QUEUE_SIZE;
        }

        cout << "----------------------------------------------------------\n";

        // Show next passenger to be served
        cout << "\nNext passenger to board: ";
        cout << queue[front].name << " (" << queue[front].passengerId << ")\n";
        cout << "Waiting for: " << queue[front].waitingTime << " minutes\n";
    }

    // ============ SEARCH IN QUEUE ============
    int searchPassenger(string pid) {
        if (isEmpty()) return -1;

        int index = front;
        for (int i = 0; i < count; i++) {
            if (queue[index].passengerId == pid) {
                return i + 1;  // Position in queue (1-based)
            }
            index = (index + 1) % QUEUE_SIZE;
        }

        return -1;  // Not found
    }

    // ============ CLEAR QUEUE ============
    void clearQueue() {
        for (int i = 0; i < QUEUE_SIZE; i++) {
            queue[i].passengerId = "";
            queue[i].name = "";
            queue[i].destinationStop = "";
            queue[i].waitingTime = 0;
        }

        front = 0;
        rear = -1;
        count = 0;

        cout << "Queue cleared. All passengers removed.\n";
    }

    // ============ GET AVERAGE WAITING TIME ============
    double getAverageWaitingTime() {
        if (isEmpty()) return 0.0;

        int total = 0;
        int index = front;

        for (int i = 0; i < count; i++) {
            total += queue[index].waitingTime;
            index = (index + 1) % QUEUE_SIZE;
        }

        return (double)total / count;
    }

    // ============ SIMULATE BUS ARRIVAL ============
    void simulateBusArrival(string busNo, int capacity = 20) {
        cout << "\n=== BUS " << busNo << " ARRIVAL SIMULATION ===\n";
        cout << "Bus capacity: " << capacity << " passengers\n";

        if (isEmpty()) {
            cout << "No passengers waiting. Bus leaves empty.\n";
            return;
        }

        int boarded = 0;

        // Board passengers until bus is full or queue is empty
        while (boarded < capacity && !isEmpty()) {
            QueuePassenger p = dequeue();
            cout << "  " << (boarded + 1) << ". " << p.name << " boarded\n";
            boarded++;
        }

        cout << "\n Bus " << busNo << " departed with " << boarded << " passengers\n";
        cout << "  Passengers remaining in queue: " << count << endl;
    }
};

#endif // QUEUE_H