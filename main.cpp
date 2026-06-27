#include <iostream>
#include "Graph.h"
#include "Transport.h"
#include "Stack.h"
#include "LinkedList.h"
#include "CSVReader.h"
#include "Facilities.h"
#include "queuecircular.h"
#include "Passenger.h"
using namespace std;

// Function prototypes
void testCSVHandler();
void demoWithCSV();
void interactiveCSVDemo();
void runtimeModificationDemo();
void testAllDataStructures();

// ========================================
// TEST CSVHANDLER
// ========================================
void testCSVHandler() {
    cout << "\n=== TESTING CSVHANDLER ===\n";

    // Test 1: StringArray
    cout << "\n1. Testing StringArray:\n";
    StringArray arr;
    arr.add("Hello");
    arr.add("World");
    arr.add("CSV");
    arr.add("Parser");
    cout << "Array contents: ";
    arr.display();
    cout << "Size: " << arr.getSize() << endl;
    cout << "Element 2: " << arr.get(2) << endl;

    // Test 2: CSVData
    cout << "\n2. Testing CSVData:\n";
    CSVData* csvData = new CSVData();

    StringArray row1, row2, row3;
    row1.add("ID1"); row1.add("Name1"); row1.add("Value1");
    row2.add("ID2"); row2.add("Name2"); row2.add("Value2");
    row3.add("ID3"); row3.add("Name3"); row3.add("Value3");

    csvData->addRow(row1);
    csvData->addRow(row2);
    csvData->addRow(row3);

    csvData->display();
    cout << "Cell (1,2): " << csvData->getCell(1, 2) << endl;

    delete csvData;

    // Test 3: Route parsing
    cout << "\n3. Testing Route Parsing:\n";
    string testRoute = "Stop1 > Stop2 > Stop3 > Stop4";
    StringArray* stops = CSVHandler::parseRouteString(testRoute);
    cout << "Route: " << testRoute << endl;
    cout << "Parsed stops: ";
    stops->display();
    delete stops;
}

// ========================================
// DEMO WITH CSV LOADING
// ========================================
void demoWithCSV() {
    cout << "\n=== SMART CITY WITH CSV LOADING ===\n";

    // Create systems
    Graph city;
    TransportSystem transport(&city);
    RouteStack history;

    // Display initial state
    cout << "\n[1] Initial State:\n";
    cout << "City nodes: " << city.getNodeCount() << endl;
    cout << "Buses: " << transport.getBusCount() << endl;

    // Load data from CSV
    cout << "\n[2] Loading Data from CSV files...\n";

    // Try to load stops
    bool stopsLoaded = city.loadStopsFromCSV("stops.csv");

    if (!stopsLoaded) {
        cout << "\n[WARNING] CSV loading failed. Creating sample data...\n";

        // Create sample stops
        city.addNode("Stop1", "G-10 Markaz", 33.684, 73.025, "BUS_STOP");
        city.addNode("Stop2", "F-10 Park", 33.691, 73.019, "BUS_STOP");
        city.addNode("Stop3", "PIMS Hospital", 33.706, 73.055, "BUS_STOP");
        city.addNode("Stop4", "F-8 Kacheri", 33.709, 73.037, "BUS_STOP");
        city.addNode("Stop5", "G-9 Bus Adda", 33.700, 73.030, "BUS_STOP");
        city.addNode("Stop6", "Blue Area", 33.720, 73.065, "BUS_STOP");

        cout << "[OK] Created 6 sample stops\n";
    }

    // Create connections
    cout << "\n[3] Creating Road Connections...\n";
    city.createAutomaticConnections();

    // Try to load buses
    bool busesLoaded = transport.loadBusesFromCSV("buses.csv");

    if (!busesLoaded) {
        cout << "\n[WARNING] Bus CSV loading failed. Creating sample buses...\n";

        // Create sample buses
        transport.registerBus("B101", "MetroBus", "Stop3");
        transport.addStopToBus("B101", "Stop1");
        transport.addStopToBus("B101", "Stop2");
        transport.addStopToBus("B101", "Stop4");

        transport.registerBus("B102", "CapitalTrans", "Stop1");
        transport.addStopToBus("B102", "Stop5");
        transport.addStopToBus("B102", "Stop8");

        cout << "[OK] Created 2 sample buses\n";
    }

    // Update some bus locations
    cout << "\n[4] Updating Bus Locations...\n";
    transport.updateBusLocation("B101", "Stop3");
    transport.updateBusLocation("B102", "Stop1");

    // Display final state
    cout << "\n[5] Final State:\n";
    cout << "========================================\n";
    cout << "City Graph Summary:\n";
    city.displayGraph();

    cout << "\nTransport System Summary:\n";
    transport.displayAllBuses();

    // Test Dijkstra
    cout << "\n[6] Testing Dijkstra Algorithm:\n";
    cout << "========================================\n";

    cout << "\nTest 1: Short path\n";
    city.dijkstra("Stop1", "Stop2");

    cout << "\nTest 2: Medium path\n";
    city.dijkstra("Stop1", "Stop4");

    // Test nearest bus
    cout << "\n[7] Testing Nearest Bus Search:\n";
    cout << "========================================\n";
    transport.findNearestBus("Stop2");

    // Test stack
    cout << "\n[8] Testing Stack Operations:\n";
    cout << "========================================\n";
    history.push("System started");
    history.push("CSV data loaded");
    history.push("Buses initialized");
    history.display();

    // Summary
    cout << "\n========================================\n";
    cout << "[SYSTEM SUMMARY]\n";
    cout << "========================================\n";
    cout << "[OK] Graph: " << city.getNodeCount() << " nodes\n";
    cout << "[OK] Buses: " << transport.getBusCount() << " buses\n";
    cout << "[OK] Dijkstra: Working\n";
    cout << "[OK] Stack: " << history.getSize() << " entries\n";
    cout << "[OK] CSV Loading: " << (stopsLoaded ? "Successful" : "Fallback used") << endl;
    cout << "========================================\n";
}

// ========================================
// INTERACTIVE CSV DEMO
// ========================================
void interactiveCSVDemo(Graph* cityGraph, TransportSystem* transport, FacilitiesSystem* facilities) {
    cout << "\n=== INTERACTIVE CSV DEMO ===\n";

    // REMOVE these local creations:
    // Graph city;  ❌ DELETE THIS
    // TransportSystem transport(&city);  ❌ DELETE THIS

    // USE the passed objects instead:
    // cityGraph, transport, facilities are already provided

    int choice;
    do {
        cout << "\n=== CSV DEMO MENU ===\n";
        cout << "1. Load stops.csv\n";
        cout << "2. Load buses.csv\n";
        cout << "3. Display Graph\n";
        cout << "4. Display Buses\n";
        cout << "5. Test Dijkstra\n";
        cout << "6. Find Nearest Bus\n";
        cout << "7. Test Stack\n";
        cout << "8. Test Linked List\n";
        cout << "9. Test CSVHandler Directly\n";
        cout << "10. View Travel History\n";  // ADD THIS
        cout << "11. Return to Main Menu\n";
        cout << "Choice: ";
        cin >> choice;

        switch (choice) {
        case 1:
            cityGraph->loadStopsFromCSV("stops.csv");  // Use cityGraph
            cityGraph->createAutomaticConnections();
            break;
        case 2:
            transport->loadBusesFromCSV("buses.csv");  // Use transport
            break;
        case 3:
            cityGraph->displayGraph();  // Use cityGraph
            break;
        case 4:
            transport->displayAllBuses();  // Use transport
            break;
        case 5: {
            string start, end;
            cout << "Enter start stop: ";
            cin >> start;
            cout << "Enter end stop: ";
            cin >> end;
            cityGraph->dijkstra(start, end);  // Use cityGraph
            break;
        }
        case 6: {
            string location;
            cout << "Enter your location stop ID: ";
            cin >> location;
            transport->findNearestBus(location);  // Use transport
            break;
        }
        case 7: {
            cout << "\n=== TESTING STACK ===\n";
            RouteStack testStack;
            testStack.push("Location A");
            testStack.push("Location B");
            testStack.push("Location C");
            testStack.display();
            cout << "Popping: " << testStack.pop() << endl;
            testStack.display();
            break;
        }
        case 8: {
            cout << "\n=== TESTING LINKED LIST ===\n";
            BusRoute testRoute("TEST001");
            testRoute.addStop("StopA");
            testRoute.addStop("StopB");
            testRoute.addStop("StopC");
            testRoute.displayRoute();
            testRoute.insertStopAt("StopX", 2);
            testRoute.displayRoute();
            testRoute.removeStop("StopB");
            testRoute.displayRoute();
            break;
        }
        case 9:
            testCSVHandler();
            break;
        case 10:  // NEW: View travel history
            cout << "\n=== TRANSPORT SYSTEM TRAVEL HISTORY ===\n";
            // You need to add this method to TransportSystem:
            // transport->displayTravelHistory();
            // For now, use existing history:
            transport->displayHistory();
            break;
        case 11:
            cout << "Returning to main menu...\n";
            // Objects PERSIST because they came from main()
            break;
        default:
            cout << "Invalid choice!\n";
        }
    } while (choice != 11);
}

// ========================================
// RUNTIME MODIFICATION DEMO
// ========================================
void runtimeModificationDemo() {
    cout << "\n=== RUNTIME MODIFICATION DEMO ===\n";
    cout << "Add/Modify data at runtime (For Viva)\n";

    Graph city;
    TransportSystem transport(&city);

    // Create initial system
    cout << "\nCreating initial system...\n";
    city.addNode("Stop1", "G-10 Markaz", 33.684, 73.025, "BUS_STOP");
    city.addNode("Stop2", "F-10 Park", 33.691, 73.019, "BUS_STOP");
    city.addNode("Stop3", "PIMS Hospital", 33.706, 73.055, "BUS_STOP");

    city.addEdge("Stop1", "Stop2", 5);
    city.addEdge("Stop2", "Stop3", 8);

    transport.registerBus("B101", "MetroBus", "Stop1");
    transport.addStopToBus("B101", "Stop2");
    transport.addStopToBus("B101", "Stop3");

    cout << "\n[INITIAL STATE]\n";
    cout << "Nodes: " << city.getNodeCount() << endl;
    cout << "Buses: " << transport.getBusCount() << endl;

    // Runtime modifications
    cout << "\n=== RUNTIME MODIFICATIONS ===\n";

    // 1. Add new bus stop at runtime
    cout << "\n1. Adding new bus stop 'Stop4' at runtime...\n";
    city.addNode("Stop4", "F-8 Kacheri", 33.709, 73.037, "BUS_STOP");
    city.addEdge("Stop3", "Stop4", 3);

    // 2. Add new bus at runtime
    cout << "\n2. Adding new bus 'B102' at runtime...\n";
    transport.registerBus("B102", "CapitalTrans", "Stop2");
    transport.addStopToBus("B102", "Stop4");

    // 3. Update bus location at runtime
    cout << "\n3. Updating bus B101 location at runtime...\n";
    transport.updateBusLocation("B101", "Stop3");

    // 4. Test Dijkstra with new nodes
    cout << "\n4. Testing Dijkstra with new stop...\n";
    city.dijkstra("Stop1", "Stop4");

    // 5. Test nearest bus with new system
    cout << "\n5. Testing nearest bus search...\n";
    transport.findNearestBus("Stop2");

    // Final state
    cout << "\n[FINAL STATE]\n";
    cout << "Nodes: " << city.getNodeCount() << endl;
    cout << "Buses: " << transport.getBusCount() << endl;
    cout << "\n=== RUNTIME MODIFICATION COMPLETE ===\n";
}

// ========================================
// TEST ALL DATA STRUCTURES
// ========================================
void testAllDataStructures() {
    cout << "\n=== TESTING ALL DATA STRUCTURES ===\n";

    cout << "\n1. TESTING GRAPH:\n";
    Graph g;
    g.addNode("A", "Location A", 0, 0);
    g.addNode("B", "Location B", 0, 0);
    g.addNode("C", "Location C", 0, 0);
    g.addEdge("A", "B", 10);
    g.addEdge("B", "C", 20);
    g.displayGraph();
    cout << "\nDijkstra Test (A to C):\n";
    g.dijkstra("A", "C");

    cout << "\n2. TESTING STACK:\n";
    RouteStack s;
    s.push("Home");
    s.push("Stop1");
    s.push("Stop2");
    s.display();
    cout << "Popped: " << s.pop() << endl;
    s.display();

    cout << "\n3. TESTING LINKED LIST (Bus Route):\n";
    BusRoute route("TEST");
    route.addStop("S1");
    route.addStop("S2");
    route.addStop("S3");
    route.displayRoute();
    route.insertStopAt("SX", 2);
    route.displayRoute();
    route.removeStop("S2");
    route.displayRoute();

    cout << "\n4. TESTING TRANSPORT SYSTEM:\n";
    Graph city2;
    TransportSystem ts(&city2);
    city2.addNode("X", "Loc X", 0, 0);
    city2.addNode("Y", "Loc Y", 0, 0);
    ts.registerBus("BUS1", "TestCo", "X");
    ts.addStopToBus("BUS1", "Y");
    ts.displayAllBuses();

    cout << "\n=== ALL DATA STRUCTURES WORKING ===\n";
}
void queueSimulationMenu(CircularQueue* passengerQueue) {
    int choice;

    do {
        cout << "\n=== PASSENGER QUEUE SIMULATION ===\n";
        cout << "1. Add Passenger to Queue\n";
        cout << "2. Remove Next Passenger (Dequeue)\n";
        cout << "3. View Next Passenger\n";
        cout << "4. Display Entire Queue\n";
        cout << "5. Update Waiting Time (+1 min)\n";
        cout << "6. Search Passenger in Queue\n";
        cout << "7. Simulate Bus Arrival\n";
        cout << "8. Get Queue Statistics\n";
        cout << "9. Clear Queue\n";
        cout << "10. Return to Main Menu\n";
        cout << "Choice: ";
        cin >> choice;

        string pid, name, destination, busNo;
        int position;

        switch (choice) {
        case 1: // Add Passenger
            cout << "Enter Passenger ID: ";
            cin >> pid;
            cout << "Enter Name: ";
            cin.ignore();
            getline(cin, name);
            cout << "Enter Destination Stop: ";
            cin >> destination;
            passengerQueue->enqueue(pid, name, destination);
            break;

        case 2: // Remove Passenger
            passengerQueue->dequeue();
            break;

        case 3: // View Next
        {
            QueuePassenger next = passengerQueue->peek();
            if (!next.passengerId.empty()) {
                cout << "\n=== NEXT PASSENGER ===\n";
                cout << "ID: " << next.passengerId << endl;
                cout << "Name: " << next.name << endl;
                cout << "Destination: " << next.destinationStop << endl;
                cout << "Waiting: " << next.waitingTime << " minutes\n";
            }
        }
        break;

        case 4: // Display Queue
            passengerQueue->displayQueue();
            break;

        case 5: // Update Waiting Time
            passengerQueue->updateWaitingTime();
            break;

        case 6: // Search
            cout << "Enter Passenger ID to search: ";
            cin >> pid;
            position = passengerQueue->searchPassenger(pid);
            if (position > 0) {
                cout << " Found! Position in queue: " << position << endl;
            }
            else {
                cout << " Passenger not found in queue.\n";
            }
            break;

        case 7: // Simulate Bus
            cout << "Enter Bus Number: ";
            cin >> busNo;
            passengerQueue->simulateBusArrival(busNo);
            break;

        case 8: // Statistics
            cout << "\n=== QUEUE STATISTICS ===\n";
            cout << "Current size: " << passengerQueue->getSize() << endl;
            cout << "Capacity: " << passengerQueue->getCapacity() << endl;
            cout << "Average waiting time: " << passengerQueue->getAverageWaitingTime() << " minutes\n";
            cout << "Queue is " << (passengerQueue->isEmpty() ? "EMPTY" : "NOT EMPTY") << endl;
            cout << "Queue is " << (passengerQueue->isFull() ? "FULL" : "NOT FULL") << endl;
            break;

        case 9: // Clear
            passengerQueue->clearQueue();
            break;

        case 10:
            cout << "Returning to Main Menu...\n";
            break;

        default:
            cout << "Invalid choice!\n";
        }
    } while (choice != 10);
}
void passengerMenu(PassengerSystem* passengers, TransportSystem* transport, FacilitiesSystem* facilities) {
    int choice;

    do {
        cout << "\n=== PASSENGER MANAGEMENT SYSTEM ===\n";
        cout << "1. Register New Passenger\n";
        cout << "2. Display All Passengers\n";
        cout << "3. Find Passenger Details\n";
        cout << "4. View Passenger Travel History\n";
        cout << "5. Remove Passenger\n";
        cout << "6. Add Demo Passengers\n";
        cout << "7. Record Journey for Passenger\n";
        cout << "8. Record Bus Boarding\n";
        cout << "9. Show Statistics\n";
        cout << "10. Return to Main Menu\n";
        cout << "Choice: ";
        cin >> choice;

        string pid, name, phone, from, to, transportName, purpose, busNo, stop;

        switch (choice) {
        case 1: // Register Passenger
            cout << "Enter Passenger ID: ";
            cin >> pid;
            cout << "Enter Name: ";
            cin.ignore();
            getline(cin, name);
            cout << "Enter Phone (optional): ";
            getline(cin, phone);
            passengers->registerPassenger(pid, name, phone);
            break;

        case 2: // Display All
            passengers->displayAllPassengers();
            break;

        case 3: // Find Passenger
            cout << "Enter Passenger ID: ";
            cin >> pid;
            passengers->displayPassenger(pid);
            break;

        case 4: // View Travel History
            cout << "Enter Passenger ID: ";
            cin >> pid;
            {
                Passenger* p = passengers->findPassenger(pid);
                if (p) {
                    p->displayTravelHistory();
                }
                else {
                    cout << "❌ Passenger not found!\n";
                }
            }
            break;

        case 5: // Remove Passenger
            cout << "Enter Passenger ID to remove: ";
            cin >> pid;
            passengers->removePassenger(pid);
            break;

        case 6: // Add Demo Passengers
            passengers->addDemoPassengers();
            break;

        case 7: // Record Journey
            cout << "Enter Passenger ID: ";
            cin >> pid;
            cout << "Enter From: ";
            cin >> from;
            cout << "Enter To: ";
            cin >> to;
            cout << "Enter Transport (optional): ";
            cin.ignore();
            getline(cin, transportName);
            cout << "Enter Purpose (optional): ";
            getline(cin, purpose);
            passengers->recordJourneyForPassenger(pid, from, to, transportName, purpose);
            break;

        case 8: // Record Bus Boarding
            cout << "Enter Passenger ID: ";
            cin >> pid;
            cout << "Enter Bus Number: ";
            cin >> busNo;
            cout << "Enter Stop: ";
            cin >> stop;
            passengers->recordBusBoardingForPassenger(pid, busNo, stop);
            break;

        case 9: // Statistics
            passengers->showStatistics();
            break;

        case 10:
            cout << "Returning to Main Menu...\n";
            break;

        default:
            cout << "Invalid choice!\n";
        }
    } while (choice != 10);
}

// Add to your menu:
void facilitiesMenu(FacilitiesSystem* facilities) {
    int choice;

    do {
        cout << "\n=== PUBLIC FACILITIES MENU ===\n";
        cout << "1. Add New Facility\n";
        cout << "2. Find Nearest Mosque\n";
        cout << "3. Find Nearest Park\n";
        cout << "4. Find Nearest Water Cooler\n";
        cout << "5. Find Nearest Any Facility\n";
        cout << "6. Display All Facilities\n";
        cout << "7. Connect Facilities to Bus Stops\n";
        cout << "8. Add Sample Facilities (Demo)\n";
        cout << "9. EMERGENCY: Add Demo Emergency Facilities\n";      // NEW
        cout << "10. EMERGENCY: Display Emergency Facilities\n";      // NEW
        cout << "11. EMERGENCY: Find Nearest Hospital\n";             // NEW
        cout << "12. EMERGENCY: Find Nearest Police Station\n";       // NEW
        cout << "13. EMERGENCY: Find Nearest Fire Station\n";         // NEW
        cout << "14. Return to Main Menu\n";
        cout << "Choice: ";
        cin >> choice;

        string id, name, typeInput;
        double x, y;
        string startId;

        switch (choice) {
        case 1: // Add New Facility
            cout << "Enter Facility ID: ";
            cin >> id;
            cout << "Enter Facility Name: ";
            cin.ignore();
            getline(cin, name);
            cout << "Enter Facility Type (MOSQUE/PARK/WATER_COOLER): ";
            cin >> typeInput;
            cout << "Enter Latitude: ";
            cin >> x;
            cout << "Enter Longitude: ";
            cin >> y;

            facilities->addFacility(id, name, x, y, typeInput);
            break;

        case 2: // Find Nearest Mosque
            cout << "Enter your location stop ID: ";
            cin >> startId;
            facilities->findNearestFacility(startId, FACILITY_MOSQUE);
            break;

        case 3: // Find Nearest Park
            cout << "Enter your location stop ID: ";
            cin >> startId;
            facilities->findNearestFacility(startId, FACILITY_PARK);
            break;

        case 4: // Find Nearest Water Cooler
            cout << "Enter your location stop ID: ";
            cin >> startId;
            facilities->findNearestFacility(startId, FACILITY_WATER_COOLER);
            break;

        case 5: // Find Nearest Any Facility
            cout << "Enter your location stop ID: ";
            cin >> startId;
            facilities->findNearestAnyFacility(startId);
            break;

        case 6: // Display All Facilities
            facilities->displayAllFacilities();
            break;

        case 7: // Connect Facilities to Bus Stops
            facilities->connectFacilitiesToStops(); // 1 km max distance
            break;

        case 8: // Add Sample Facilities
            facilities->addSampleFacilities();
            break;
        case 9:  // Add demo emergency facilities
            facilities->addDemoEmergencyFacilities();
            break;

        case 10: // Display emergency facilities
            facilities->displayEmergencyFacilities();
            break;

        case 11: // Find nearest hospital
            cout << "Enter your location stop ID: ";
            cin >> startId;
            facilities->emergencyRouting(startId, "HOSPITAL");
            break;

        case 12: // Find nearest police
            cout << "Enter your location stop ID: ";
            cin >> startId;
            facilities->emergencyRouting(startId, "POLICE");
            break;

        case 13: // Find nearest fire station
            cout << "Enter your location stop ID: ";
            cin >> startId;
            facilities->emergencyRouting(startId, "FIRE_STATION");
            break;

        case 14:
            cout << "Returning to Main Menu...\n";
            break;

        default:
            cout << "Invalid choice!\n";
        }
    } while (choice != 14);
}


// ========================================
// MAIN FUNCTION
// ========================================
int main() {
    cout << "========================================\n";
    cout << "   SMART CITY TRANSPORT & FACILITIES SYSTEM\n";
    cout << "========================================\n\n";

    // ============ CREATE SINGLE SHARED GRAPH ============
    Graph* cityGraph = new Graph(100);  // ONE graph for everything

    // Create all systems with the SAME graph
    TransportSystem* transport = new TransportSystem(cityGraph);
    FacilitiesSystem* facilities = new FacilitiesSystem(cityGraph);

    // Create Passenger System and Queue
    PassengerSystem* passengers = new PassengerSystem();
    CircularQueue* passengerQueue = new CircularQueue();
    // ===================================================

    int choice;
    do {
        cout << "\n=== MAIN MENU ===\n";
        cout << "1. Passenger Queue Simulation (Circular Queue)\n";
        cout << "2. Passenger Management System\n";
        cout << "3. Interactive Transport Demo (Load CSV)\n";
        cout << "4. Public Facilities Module\n";
        cout << "5. Test All Components\n";
        cout << "6. Exit\n";
        cout << "Choice: ";
        cin >> choice;

        switch (choice) {
        case 1:
            queueSimulationMenu(passengerQueue);
            break;
        case 2:
            passengerMenu(passengers, transport, facilities);
            break;
        case 3:
            interactiveCSVDemo(cityGraph, transport, facilities);
            break;
        case 4:
            facilitiesMenu(facilities);
            break;
        case 5:
            testAllDataStructures();
            break;
        case 6:
            cout << "Exiting...\n";
            break;

        default:
            cout << "Invalid choice!\n";
        }
    } while (choice != 6);

    // ============ CLEANUP ============
    delete passengerQueue;
    delete passengers;
    delete facilities;
    delete transport;
    delete cityGraph;
    // =================================

    return 0;
}