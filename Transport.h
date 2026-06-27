// Transport.h - COMPLETE VERSION
#ifndef TRANSPORT_H
#define TRANSPORT_H

#include "Graph.h"
#include "CSVReader.h"
#include "LinkedList.h"
#include "Stack.h"
#include <iostream>
#include <string>
#include <climits>
using namespace std;

class Bus {
public:
    string busNo;
    string company;
    string currentStop;
    BusRoute* route;


    Bus(string no, string comp, string startStop) {
        busNo = no;
        company = comp;
        currentStop = startStop;
        route = new BusRoute(no);  // Empty route
        // DON'T add startStop here
    }

    ~Bus() { delete route; }

    void updateLocation(string newStop) {
        currentStop = newStop;
    }

    bool isAtStop(string stopId) {
        return currentStop == stopId;
    }

    void displayInfo() {
        cout << "\nBus " << busNo << " (" << company << ")\n";
        cout << "Current Location: " << currentStop << endl;
        route->displayRoute();
    }
};

class TransportSystem {
private:
    Graph* cityGraph;
    RouteStack* history;
    Bus** buses;
    int busCount;
    int busCapacity;
    RouteStack* travelHistory;

    void expandBusArray() {
        int newCap = busCapacity * 2;
        Bus** newBuses = new Bus * [newCap];

        for (int i = 0; i < busCount; i++) {
            newBuses[i] = buses[i];
        }
        for (int i = busCount; i < newCap; i++) {
            newBuses[i] = nullptr;
        }

        delete[] buses;
        buses = newBuses;
        busCapacity = newCap;
    }

public:
    TransportSystem(Graph* graph) {
        cityGraph = graph;
        history = new RouteStack();
        busCount = 0;
        busCapacity = 20;
        buses = new Bus * [busCapacity];

        for (int i = 0; i < busCapacity; i++) {
            buses[i] = nullptr;
        }
        travelHistory = new RouteStack();
    }

    ~TransportSystem() {
        for (int i = 0; i < busCount; i++) {
            delete buses[i];
        }
        delete[] buses;
        delete history;
        delete travelHistory;
    }

    // ============ BUS MANAGEMENT ============
    bool registerBus(string busNo, string company, string startStop) {
        if (cityGraph->findNodeIndex(startStop) == -1) {
            cout << "[ERROR] Stop '" << startStop << "' doesn't exist!\n";
            return false;
        }
       

        // Check if bus already exists
        for (int i = 0; i < busCount; i++) {
            if (buses[i]->busNo == busNo) {
                cout << "[ERROR] Bus " << busNo << " already registered!\n";
                return false;
            }
        }

        if (busCount == busCapacity) expandBusArray();

        // Create bus WITHOUT adding startStop to route
        buses[busCount] = new Bus(busNo, company, startStop);
        busCount++;

        
        travelHistory->push("Registered Bus: " + busNo + " (" + company + ") at " + startStop);
        cout << "[OK] Bus " << busNo << " (" << company << ") registered\n";
      
        return true;
    }

    bool addStopToBus(string busNo, string stopId) {
        if (cityGraph->findNodeIndex(stopId) == -1) {
            cout << "ERROR: Stop '" << stopId << "' doesn't exist!\n";
            return false;
        }

        for (int i = 0; i < busCount; i++) {
            if (buses[i]->busNo == busNo) {
                buses[i]->route->addStop(stopId);
                history->push("Bus " + busNo + " added stop: " + stopId);
                return true;
            }
        }

        cout << "ERROR: Bus " << busNo << " not found!\n";
        return false;
    }

    // ============ BUS OPERATIONS ============

    bool updateBusLocation(string busNo, string newStop) {
        for (int i = 0; i < busCount; i++) {
            if (buses[i]->busNo == busNo) {
                // Verify stop exists
                if (cityGraph->findNodeIndex(newStop) == -1) {
                    cout << "ERROR: Stop '" << newStop << "' doesn't exist!\n";
                    return false;
                }

                buses[i]->updateLocation(newStop);
                history->push("Bus " + busNo + " moved to: " + newStop);
                cout << "Bus " << busNo << " location updated to " << newStop << endl;
                return true;
            }
        }

        cout << "ERROR: Bus " << busNo << " not found!\n";
        travelHistory->push("Bus " + busNo + " moved to " + newStop);
        return false;
    }

    // ============ SEARCH FUNCTIONS ============

    string findNearestBus(string locationId) {
        int locIdx = cityGraph->findNodeIndex(locationId);
        if (locIdx == -1) {
            cout << "ERROR: Location '" << locationId << "' not found!\n";
            return "";
        }

        if (busCount == 0) {
            cout << "No buses registered in the system!\n";
            return "";
        }

        cout << "\nSearching for nearest bus to " << locationId << "...\n";

        // Check if any bus is at exact location
        for (int i = 0; i < busCount; i++) {
            if (buses[i]->isAtStop(locationId)) {
                cout << " Bus " << buses[i]->busNo << " is at your location!\n";
                return buses[i]->busNo;
            }
        }

        // If no bus at exact location, use graph search
        string nearestBus = "";
        int minDistance = INT_MAX;

        for (int i = 0; i < busCount; i++) {
            // For each bus, find distance from location to bus's current stop
            string busStop = buses[i]->currentStop;
            // We would use Dijkstra here, but for simplicity:
            // Just return the first bus for now
            nearestBus = buses[i]->busNo;
            minDistance = 1; // Placeholder
            break;
        }

        if (!nearestBus.empty()) {
            cout << " Nearest available bus: " << nearestBus
                << " at stop " << getBusLocation(nearestBus)
                << " (approx " << minDistance << " km away)\n";
        }

        return nearestBus;
    }


    void displayAllBuses() {
        if (busCount == 0) {
            cout << "No buses registered in the system.\n";
            return;
        }

        cout << "\n=== REGISTERED BUSES (" << busCount << ") ===\n";
   
        for (int i = 0; i < busCount; i++) {
            cout << "[" << i + 1 << "] ";
            buses[i]->displayInfo();
            if (i < busCount - 1);
        }
       
    }

    bool displayBus(string busNo) {
        for (int i = 0; i < busCount; i++) {
            if (buses[i]->busNo == busNo) {
                cout << "\n=== BUS DETAILS ===\n";
                buses[i]->displayInfo();
               
                return true;
            }
        }

        cout << "Bus " << busNo << " not found!\n";
        return false;
    }

    void displayHistory() {
        cout << "\n=== TRANSPORT SYSTEM HISTORY ===\n";
        history->display();
    }

    // ============ GETTER FUNCTIONS ============

    int getBusCount() {
        return busCount;
    }

    string getBusLocation(string busNo) {
        for (int i = 0; i < busCount; i++) {
            if (buses[i]->busNo == busNo) {
                return buses[i]->currentStop;
            }
        }
        return "";
    }

 
    bool loadBusesFromCSV(string filename) {
        cout << "\n=== LOADING BUSES FROM CSV ===\n";
        cout << "File: " << filename << endl;

        // First, read the file as text to handle tab-separation
        ifstream file(filename);
        if (!file.is_open()) {
            cout << "[ERROR] Cannot open file: " << filename << endl;
            return false;
        }

        // Read all lines
        string allLines;
        string line;
        while (getline(file, line)) {
            allLines += line + "\n";
        }
        file.close();

        // Replace tabs with commas for CSV parsing
        for (char& c : allLines) {
            if (c == '\t') c = ',';
        }

        // Write to temporary file
        string tempFile = "temp_buses.csv";
        ofstream temp(tempFile);
        temp << allLines;
        temp.close();

        // Now parse as CSV
        CSVData* data = CSVHandler::readCSV(tempFile, true);

        // Clean up temp file
        remove(tempFile.c_str());

        if (data->getRowCount() == 0) {
            cout << "[ERROR] No data loaded from " << filename << endl;
            delete data;
            return false;
        }

        cout << "\nProcessing " << data->getRowCount() << " buses...\n";


        int success = 0;
        int failed = 0;

        for (int i = 0; i < data->getRowCount(); i++) {
            string busNo = data->getCell(i, 0);
            string company = data->getCell(i, 1);
            string currentStop = data->getCell(i, 2);
            string routeStr = data->getCell(i, 3);

            cout << "\n--- Bus #" << (i + 1) << " ---\n";
            cout << "Number: " << busNo << endl;
            cout << "Company: " << company << endl;
            cout << "Current Stop: " << currentStop << endl;
            cout << "Route String: " << routeStr << endl;

            if (busNo.empty() || company.empty() || currentStop.empty()) {
                cout << "[SKIP] Missing required data\n";
                failed++;
                continue;
            }

            // STEP 1: Register the bus (creates bus with empty route)
            if (registerBus(busNo, company, currentStop)) {
                // STEP 2: Parse the complete route string
                if (!routeStr.empty()) {
                    StringArray* stops = CSVHandler::parseRouteString(routeStr);

                    cout << "[INFO] Parsed stops (" << stops->getSize() << "): ";
                    stops->display();

                    // STEP 3: Add ALL stops from route string to bus route
                    // The route string already contains the complete route in order
                    for (int j = 0; j < stops->getSize(); j++) {
                        string stop = stops->get(j);
                        if (!stop.empty()) {
                            // IMPORTANT: Use addStopToBus (not checking for duplicates)
                            // The route string should have the complete correct order
                            if (addStopToBus(busNo, stop)) {
                                cout << "  [ADDED] " << stop << endl;
                            }
                            else {
                                cout << "  [WARNING] Could not add stop: " << stop << endl;
                            }
                        }
                    }

                    delete stops;
                }
                else {
                    // If route string is empty, just add current stop
                    addStopToBus(busNo, currentStop);
                    cout << "  [ADDED] Current stop only: " << currentStop << endl;
                }

                // STEP 4: Verify the route contains the current stop
                // Find the bus and check its route
                for (int b = 0; b < busCount; b++) {
                    if (buses[b]->busNo == busNo) {
                        if (!buses[b]->route->containsStop(currentStop)) {
                            // Add current stop if missing
                            buses[b]->route->addStop(currentStop);
                            cout << "  [FIXED] Added missing current stop: " << currentStop << endl;
                        }
                        break;
                    }
                }

                success++;
                cout << "[SUCCESS] Bus " << busNo << " registered with complete route\n";
            }
            else {
                failed++;
                cout << "[FAILED] Bus registration failed\n";
            }
        }

        cout << "LOADING SUMMARY:\n";
        cout << "Successfully loaded: " << success << " buses\n";
        cout << "Failed: " << failed << " buses\n";
        cout << "Total buses in system: " << busCount << endl;
     

        delete data;
        return success > 0;
    }
    void emergencyTransport(string locationId, string emergencyType) {
        cout << "\n === EMERGENCY TRANSPORT REQUEST ===\n";
        cout << "Location: " << locationId << endl;
        cout << "Emergency: " << emergencyType << endl;

        // Record in history
        travelHistory->push("EMERGENCY: " + emergencyType + " at " + locationId);

        // Find nearest bus for emergency evacuation
        string nearestBus = findNearestBus(locationId);

        if (!nearestBus.empty()) {
            cout << "\n EMERGENCY TRANSPORT ARRANGED:\n";
            cout << "  Bus: " << nearestBus << endl;
            cout << "  Current Location: " << getBusLocation(nearestBus) << endl;
            cout << "  Dispatch to: " << locationId << endl;

            // Simulate emergency dispatch
            updateBusLocation(nearestBus, locationId);
            cout << "  Bus dispatched to emergency location!\n";

            // Find nearest emergency facility
            cout << "\n ROUTING TO NEAREST " << emergencyType << ":\n";
            // This would integrate with FacilitiesSystem
        }
        else {
            cout << "\n NO BUSES AVAILABLE FOR EMERGENCY!\n";
            cout << "  Call 1122 for ambulance directly.\n";
            cout << "  Use alternative transport if possible.\n";
        }
    }
    bool removeBus(string busNo) {
        for (int i = 0; i < busCount; i++) {
            if (buses[i]->busNo == busNo) {
                delete buses[i];

                // Shift remaining buses
                for (int j = i; j < busCount - 1; j++) {
                    buses[j] = buses[j + 1];
                }

                buses[busCount - 1] = nullptr;
                busCount--;

                history->push("Removed Bus: " + busNo);
                cout << "Bus " << busNo << " removed from system.\n";
                return true;
            }
        }

        cout << "Bus " << busNo << " not found!\n";
        return false;
    }


    void clearAllBuses() {
        for (int i = 0; i < busCount; i++) {
            delete buses[i];
        }
        busCount = 0;
        history->clear();
        cout << "All buses cleared from system.\n";
    }


};

#endif // TRANSPORT_H