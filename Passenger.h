#pragma once
// Passenger.h - PASSENGER MANAGEMENT SYSTEM
#ifndef PASSENGER_H
#define PASSENGER_H

#include "Stack.h"
#include <iostream>
#include <string>
using namespace std;

class Passenger {
private:
    string passengerId;
    string name;
    string phone;
    RouteStack* travelHistory;

public:
    // Constructor
    Passenger(string id, string n, string ph = "") {
        passengerId = id;
        name = n;
        phone = ph;
        travelHistory = new RouteStack();
        travelHistory->push("Passenger registered: " + id + " - " + n);
    }

    // Destructor
    ~Passenger() {
        delete travelHistory;
    }

    // Record a journey
    void recordJourney(string from, string to, string transport = "", string purpose = "") {
        string record = "JOURNEY: " + from + " → " + to;
        if (!transport.empty()) record += " via " + transport;
        if (!purpose.empty()) record += " [" + purpose + "]";
        travelHistory->push(record);
    }

    // Record bus boarding
    void recordBusBoarding(string busNo, string stop) {
        string record = "BOARDED: Bus " + busNo + " at " + stop;
        travelHistory->push(record);
    }

    // Record facility visit
    void recordFacilityVisit(string facilityType, string facilityName) {
        string record = "VISITED: " + facilityType + " - " + facilityName;
        travelHistory->push(record);
    }

    // Display travel history
    void displayTravelHistory() {
        cout << "\n=== TRAVEL HISTORY: " << name << " (" << passengerId << ") ===\n";

        if (travelHistory->isEmpty()) {
            cout << " No travel history recorded.\n";
            // Show debug info to understand why
            cout << "\nDebug information:\n";
            travelHistory->debugInfo();
        }
        else {
            cout << "Total journeys: " << travelHistory->getSize() << endl;
            cout << "Displaying chronological order (oldest ->newest):\n";
            travelHistory->display();  // This now shows chronological order
        }
    }

    // Display passenger info
    void displayInfo() {
        cout << "\n=== PASSENGER INFO ===\n";
        cout << "ID: " << passengerId << endl;
        cout << "Name: " << name << endl;
        if (!phone.empty()) cout << "Phone: " << phone << endl;
        cout << "Total journeys: " << (travelHistory->getSize() - 1) << endl; // minus registration
        if (!travelHistory->isEmpty() && travelHistory->getSize() > 1) {
            cout << "Last activity: " << travelHistory->peek() << endl;
        }
    }

    // Getters
    string getId() { return passengerId; }
    string getName() { return name; }
    string getPhone() { return phone; }
    int getJourneyCount() { return travelHistory->getSize() - 1; } // exclude registration
};

// Passenger Management System
class PassengerSystem {
private:
    Passenger** passengers;
    int passengerCount;
    int capacity;

    void expandCapacity() {
        int newCap = capacity * 2;
        Passenger** newPassengers = new Passenger * [newCap];

        for (int i = 0; i < passengerCount; i++) {
            newPassengers[i] = passengers[i];
        }
        for (int i = passengerCount; i < newCap; i++) {
            newPassengers[i] = nullptr;
        }

        delete[] passengers;
        passengers = newPassengers;
        capacity = newCap;
    }

public:
    PassengerSystem(int initialCap = 20) {
        capacity = initialCap;
        passengerCount = 0;
        passengers = new Passenger * [capacity];

        for (int i = 0; i < capacity; i++) {
            passengers[i] = nullptr;
        }
    }

    ~PassengerSystem() {
        for (int i = 0; i < passengerCount; i++) {
            delete passengers[i];
        }
        delete[] passengers;
    }

    // Register new passenger
    bool registerPassenger(string id, string name, string phone = "") {
        // Check if already exists
        for (int i = 0; i < passengerCount; i++) {
            if (passengers[i]->getId() == id) {
                cout << " ERROR: Passenger ID " << id << " already exists!\n";
                return false;
            }
        }

        if (passengerCount == capacity) expandCapacity();

        passengers[passengerCount] = new Passenger(id, name, phone);
        passengerCount++;

        cout << " PASSENGER REGISTERED:\n";
        cout << "  ID: " << id << endl;
        cout << "  Name: " << name << endl;
        if (!phone.empty()) cout << "  Phone: " << phone << endl;
        cout << "  Total passengers now: " << passengerCount << endl;

        return true;
    }

    // Find passenger by ID
    Passenger* findPassenger(string id) {
        for (int i = 0; i < passengerCount; i++) {
            if (passengers[i]->getId() == id) {
                return passengers[i];
            }
        }
        return nullptr;
    }

    // Display all passengers
    void displayAllPassengers() {
        if (passengerCount == 0) {
            cout << "\n=== NO PASSENGERS REGISTERED ===\n";
            return;
        }

        cout << "\n=== ALL PASSENGERS (" << passengerCount << ") ===\n";
        cout << "┌─────┬────────────┬────────────────────┬────────┬────────────┐\n";
        cout << "│ No. │     ID     │        Name        │ Phone  │ Journeys   │\n";
        cout << "├─────┼────────────┼────────────────────┼────────┼────────────┤\n";

        for (int i = 0; i < passengerCount; i++) {
            cout << "│ " << (i + 1) << "   │ ";
            cout << passengers[i]->getId();
            if (passengers[i]->getId().length() < 10) cout << string(10 - passengers[i]->getId().length(), ' ');
            cout << " │ ";

            string name = passengers[i]->getName();
            if (name.length() > 18) name = name.substr(0, 15) + "...";
            cout << name;
            if (name.length() < 18) cout << string(18 - name.length(), ' ');
            cout << " │ ";

            string phone = passengers[i]->getPhone();
            if (phone.length() > 6) phone = phone.substr(0, 6);
            cout << phone;
            if (phone.length() < 6) cout << string(6 - phone.length(), ' ');
            cout << " │ ";

            cout << passengers[i]->getJourneyCount();
            if (passengers[i]->getJourneyCount() < 10) cout << " ";
            cout << "         │\n";
        }

        cout << "└─────┴────────────┴────────────────────┴────────┴────────────┘\n";
    }

    // Display passenger details
    bool displayPassenger(string id) {
        Passenger* p = findPassenger(id);
        if (p == nullptr) {
            cout << " Passenger " << id << " not found!\n";
            return false;
        }

        p->displayInfo();
        return true;
    }

    // Remove passenger
    bool removePassenger(string id) {
        for (int i = 0; i < passengerCount; i++) {
            if (passengers[i]->getId() == id) {
                cout << "Removing passenger: " << passengers[i]->getName() << " (" << id << ")\n";
                delete passengers[i];

                // Shift remaining passengers
                for (int j = i; j < passengerCount - 1; j++) {
                    passengers[j] = passengers[j + 1];
                }

                passengers[passengerCount - 1] = nullptr;
                passengerCount--;

                cout << "✓ Passenger removed successfully.\n";
                cout << "Total passengers remaining: " << passengerCount << endl;
                return true;
            }
        }

        cout << "❌ ERROR: Passenger " << id << " not found!\n";
        return false;
    }

    // In PassengerSystem::addDemoPassengers() - FIXED VERSION:
    void addDemoPassengers() {
        cout << "\n=== ADDING DEMO PASSENGERS ===\n";

        string demoData[][3] = {
            {"P001", "Ali Ahmed", "03001234567"},
            {"P002", "Sara Khan", "03111234568"},
            {"P003", "Usman Malik", "03211234569"},
            {"P004", "Fatima Noor", "03311234570"},
            {"P005", "Bilal Raza", "03411234571"},
            {"P006", "Zainab Akhtar", "03511234572"},
            {"P007", "Omar Sheikh", "03611234573"},
            {"P008", "Ayesha Siddiqui", "03711234574"},
            {"P009", "Haris Mahmood", "03811234575"},
            {"P010", "Mariam Javed", "03911234576"}
        };

        int added = 0;
        for (int i = 0; i < 10; i++) {
            if (registerPassenger(demoData[i][0], demoData[i][1], demoData[i][2])) {
                added++;

                // CRITICAL: Get the passenger AFTER registration
                Passenger* p = findPassenger(demoData[i][0]);
                if (p) {
                    cout << "\n  Adding travel history for " << p->getName() << ":\n";

                    // Record actual journeys
                    p->recordJourney("Home", "Stop1", "Walk", "Morning commute");
                    cout << "    [1] Home → Stop1 (Walk)\n";

                    p->recordBusBoarding("B101", "Stop1");
                    cout << "    [2] Boarded Bus B101 at Stop1\n";

                    p->recordJourney("Stop1", "Stop3", "Bus B101", "Work commute");
                    cout << "    [3] Stop1 → Stop3 via Bus B101\n";

                    p->recordFacilityVisit("MOSQUE", "G-10 Markaz Mosque");
                    cout << "    [4] Visited G-10 Markaz Mosque\n";

                    // IMMEDIATELY VERIFY
                    cout << "    [VERIFY] Stack size: " << p->getJourneyCount() << endl;
                    cout << "             (Should be 5: 1 registration + 4 activities)\n";
                }
                else {
                    cout << "  ERROR: Could not find passenger after registration!\n";
                }
            }
        }

        cout << "\n✓ ADDED " << added << " DEMO PASSENGERS\n";
        cout << "Each passenger should have 5 history entries.\n";
        cout << "Use 'View Passenger Travel History' to check.\n";
    }

    // Get passenger count
    int getPassengerCount() {
        return passengerCount;
    }

    // Record journey for passenger
    bool recordJourneyForPassenger(string pid, string from, string to, string transport = "", string purpose = "") {
        Passenger* p = findPassenger(pid);
        if (p == nullptr) {
            cout << " Passenger " << pid << " not found!\n";
            return false;
        }

        p->recordJourney(from, to, transport, purpose);
        cout << " Journey recorded for " << p->getName() << endl;
        return true;
    }

    // Record bus boarding for passenger
    bool recordBusBoardingForPassenger(string pid, string busNo, string stop) {
        Passenger* p = findPassenger(pid);
        if (p == nullptr) {
            cout << " Passenger " << pid << " not found!\n";
            return false;
        }

        p->recordBusBoarding(busNo, stop);
        cout << " Boarding recorded for " << p->getName() << endl;
        return true;
    }

    // Show passenger statistics
    void showStatistics() {
        cout << "\n=== PASSENGER SYSTEM STATISTICS ===\n";
        cout << "Total passengers: " << passengerCount << endl;
        cout << "System capacity: " << capacity << endl;

        if (passengerCount > 0) {
            int totalJourneys = 0;
            for (int i = 0; i < passengerCount; i++) {
                totalJourneys += passengers[i]->getJourneyCount();
            }

            cout << "Total journeys recorded: " << totalJourneys << endl;
            cout << "Average journeys per passenger: " << (double)totalJourneys / passengerCount << endl;

            // Find most traveled passenger
            int maxJourneys = -1;
            string mostTraveled = "";
            for (int i = 0; i < passengerCount; i++) {
                if (passengers[i]->getJourneyCount() > maxJourneys) {
                    maxJourneys = passengers[i]->getJourneyCount();
                    mostTraveled = passengers[i]->getName() + " (" + passengers[i]->getId() + ")";
                }
            }

            cout << "Most traveled passenger: " << mostTraveled << " (" << maxJourneys << " journeys)\n";
        }
    }

};

#endif // PASSENGER_H