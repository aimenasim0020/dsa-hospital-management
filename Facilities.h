// Facilities.h - PUBLIC FACILITIES MODULE
#ifndef FACILITIES_H
#define FACILITIES_H

#include "Graph.h"
#include <iostream>
#include <string>
using namespace std;

// Facility types
#define FACILITY_MOSQUE "MOSQUE"
#define FACILITY_PARK "PARK"
#define FACILITY_WATER_COOLER "WATER_COOLER"
#define FACILITY_HOSPITAL "HOSPITAL"
#define FACILITY_POLICE_STATION "POLICE"
#define FACILITY_FIRE_STATION "FIRE_STATION"

class FacilitiesSystem {
private:
    Graph* cityGraph;

    // Helper method to check if a node is ANY type of facility
    bool isAnyFacility(GraphNode* node) {
        string type = node->type;
        return (type == FACILITY_MOSQUE || type == FACILITY_PARK ||
            type == FACILITY_WATER_COOLER || type == FACILITY_HOSPITAL ||
            type == FACILITY_POLICE_STATION || type == FACILITY_FIRE_STATION);
    }

public:
    // Constructor
    FacilitiesSystem(Graph* graph) {
        cityGraph = graph;
    }

    // ============ CORE FACILITY METHODS ============

    // Add a single facility (works for ALL facility types)
    bool addFacility(string id, string name, double x, double y, string facilityType) {
        // Validate facility type
        if (!isValidFacilityType(facilityType)) {
            cout << "ERROR: Invalid facility type! Must be MOSQUE, PARK, WATER_COOLER, HOSPITAL, POLICE, or FIRE_STATION\n";
            return false;
        }

        // Add to graph
        int index = cityGraph->addNode(id, name, x, y, facilityType);
        if (index != -1) {
            cout << " ✓ Added " << getFacilityTypeName(facilityType) << ": " << name << " (" << id << ")\n";
            return true;
        }

        return false;
    }

    // Find nearest facility of specific type
    string findNearestFacility(string startId, string facilityType) {
        cout << "\n=== FINDING NEAREST " << getFacilityTypeName(facilityType) << " ===\n";

        // Validate facility type
        if (!isValidFacilityType(facilityType)) {
            cout << "ERROR: Invalid facility type!\n";
            return "";
        }

        // Use graph's built-in search
        string nearestId = cityGraph->findNearestNodeOfType(startId, facilityType);

        if (!nearestId.empty()) {
            GraphNode* facility = cityGraph->getNodeById(nearestId);
            if (facility) {
                cout << "\n ✓ FOUND NEAREST " << getFacilityTypeName(facilityType) << ":\n";
                cout << "  Name: " << facility->name << endl;
                cout << "  ID: " << facility->id << endl;
                cout << "  Coordinates: (" << facility->x << ", " << facility->y << ")\n";
            }
        }
        else {
            cout << "\n ✗ No " << getFacilityTypeName(facilityType) << " found!\n";
        }

        return nearestId;
    }

    // Find nearest facility of ANY type (including emergency)
    void findNearestAnyFacility(string startId) {
        cout << "\n=== FINDING NEAREST FACILITY (Any Type) ===\n";

        int startIdx = cityGraph->findNodeIndex(startId);
        if (startIdx == -1) {
            cout << "ERROR: Start location '" << startId << "' not found!\n";
            return;
        }

        GraphNode* startNode = cityGraph->getNode(startIdx);
        cout << "From: " << startNode->name << " (" << startId << ")\n";
        cout << "Searching for nearest facility of ANY type...\n";

        // Try each type
        string nearestId = "";
        string nearestType = "";
        int minDistance = INT_MAX;

        // Check all facility types
        string types[] = { FACILITY_MOSQUE, FACILITY_PARK, FACILITY_WATER_COOLER,
                         FACILITY_HOSPITAL, FACILITY_POLICE_STATION, FACILITY_FIRE_STATION };
        string typeNames[] = { "Mosque", "Park", "Water Cooler", "Hospital", "Police Station", "Fire Station" };

        cout << "\n=== SEARCH RESULTS ===\n";
        int count = 0;

        for (int i = 0; i < 6; i++) {
            string facilityId = findNearestFacility(startId, types[i]);
            if (!facilityId.empty()) {
                GraphNode* facility = cityGraph->getNodeById(facilityId);
                if (facility) {
                    cout << "[" << ++count << "] " << typeNames[i] << ": "
                        << facility->name << " (" << facilityId << ")\n";
                }
            }
        }

        if (count == 0) {
            cout << "\n  No facilities found near " << startId << endl;
        }
        else {
            cout << "\nFound " << count << " different types of facilities nearby.\n";
            cout << "Use 'Find Nearest [Specific Type]' for exact distances.\n";
        }
    }

    // Display ALL facilities in system (including emergency)
    void displayAllFacilities() {
        cout << "\n=== ALL FACILITIES IN SYSTEM ===\n";

        int mosqueCount = 0, parkCount = 0, coolerCount = 0;
        int hospitalCount = 0, policeCount = 0, fireCount = 0;

        for (int i = 0; i < cityGraph->getNodeCount(); i++) {
            GraphNode* node = cityGraph->getNode(i);
            string type = node->type;

            if (isAnyFacility(node)) {
                if (mosqueCount + parkCount + coolerCount + hospitalCount + policeCount + fireCount == 0) {
                    cout << "ID\t\tType\t\t\tName\t\tCoordinates\n";

                }

                cout << node->id << "\t\t";
                cout << type;

                // Adjust spacing for shorter type names
                if (type.length() < 8) cout << "\t\t";
                else if (type.length() < 16) cout << "\t";

                cout << "\t" << node->name;

                // Adjust name spacing
                if (node->name.length() < 12) cout << "\t\t";
                else if (node->name.length() < 20) cout << "\t";

                cout << "(" << node->x << ", " << node->y << ")\n";

                // Count by type
                if (type == FACILITY_MOSQUE) mosqueCount++;
                else if (type == FACILITY_PARK) parkCount++;
                else if (type == FACILITY_WATER_COOLER) coolerCount++;
                else if (type == FACILITY_HOSPITAL) hospitalCount++;
                else if (type == FACILITY_POLICE_STATION) policeCount++;
                else if (type == FACILITY_FIRE_STATION) fireCount++;
            }
        }

        int totalFacilities = mosqueCount + parkCount + coolerCount + hospitalCount + policeCount + fireCount;

        if (totalFacilities == 0) {
            cout << "No facilities added yet.\n";
        }
        else {
            cout << "\n=== FACILITIES SUMMARY ===\n";
            cout << "PUBLIC FACILITIES:\n";
            cout << "  Mosques: " << mosqueCount << endl;
            cout << "  Parks: " << parkCount << endl;
            cout << "  Water Coolers: " << coolerCount << endl;
            cout << "  Total Public: " << (mosqueCount + parkCount + coolerCount) << endl;

            cout << "\nEMERGENCY FACILITIES:\n";
            cout << "  Hospitals: " << hospitalCount << endl;
            cout << "  Police Stations: " << policeCount << endl;
            cout << "  Fire Stations: " << fireCount << endl;
            cout << "  Total Emergency: " << (hospitalCount + policeCount + fireCount) << endl;

            cout << "\nGRAND TOTAL: " << totalFacilities << " facilities\n";
        }
    }

    // Connect ALL facilities to nearest bus stops (including emergency)
    void connectFacilitiesToStops() {
        cout << "\n=== CONNECTING ALL FACILITIES TO BUS STOPS ===\n";

        // First check if bus stops exist
        int busStopCount = 0;
        for (int i = 0; i < cityGraph->getNodeCount(); i++) {
            if (cityGraph->getNode(i)->type == "BUS_STOP") {
                busStopCount++;
            }
        }

        if (busStopCount == 0) {
            cout << " ERROR: No bus stops found in the graph!\n";
            cout << "Please load bus stops first (Load stops.csv in Transport Demo)\n";
            return;
        }

        cout << "Found " << busStopCount << " bus stops in the system\n";

        int connections = 0;
        int nodeCount = cityGraph->getNodeCount();

        for (int i = 0; i < nodeCount; i++) {
            GraphNode* facility = cityGraph->getNode(i);

            // Check if it's ANY type of facility
            if (isAnyFacility(facility)) {
                // Find distance to nearest bus stop
                double minDistance = 1000000; // Large number
                string nearestStopId = "";
                GraphNode* nearestStop = nullptr;

                for (int j = 0; j < nodeCount; j++) {
                    GraphNode* stop = cityGraph->getNode(j);

                    if (stop->type == "BUS_STOP") {
                        // Calculate Euclidean distance between coordinates
                        double dx = (stop->x - facility->x) * 111.0; // Convert to km
                        double dy = (stop->y - facility->y) * 111.0;
                        double distance = sqrt(dx * dx + dy * dy);

                        if (distance < minDistance) {
                            minDistance = distance;
                            nearestStopId = stop->id;
                            nearestStop = stop;
                        }
                    }
                }

                if (!nearestStopId.empty() && minDistance < 5.0) {
                    // Different weights for different facility types
                    int weight = (int)(minDistance * 10);

                    // Emergency facilities get priority (lower weight)
                    if (facility->type == FACILITY_HOSPITAL ||
                        facility->type == FACILITY_POLICE_STATION ||
                        facility->type == FACILITY_FIRE_STATION) {
                        weight = max(1, weight / 2); // Emergency routes are faster
                    }

                    if (weight < 1) weight = 1; // Minimum weight of 1

                    if (cityGraph->addEdge(facility->id, nearestStopId, weight)) {
                        connections++;
                        cout << "  " << getFacilityTypeName(facility->type) << " [" << facility->name << "]";
                        cout << " -> " << nearestStop->name << " (" << nearestStopId << ")";
                        cout << " [Weight: " << weight << "]\n";
                    }
                }
            }
        }

        cout << "\n✓ Created " << connections << " weighted connections\n";
    }

    // ============ HELPER METHODS ============
    bool isValidFacilityType(string type) {
        return (type == FACILITY_MOSQUE || type == FACILITY_PARK ||
            type == FACILITY_WATER_COOLER || type == FACILITY_HOSPITAL ||
            type == FACILITY_POLICE_STATION || type == FACILITY_FIRE_STATION);
    }

    string getFacilityTypeName(string type) {
        if (type == FACILITY_MOSQUE) return "Mosque";
        if (type == FACILITY_PARK) return "Park";
        if (type == FACILITY_WATER_COOLER) return "Water Cooler";
        if (type == FACILITY_HOSPITAL) return "Hospital";
        if (type == FACILITY_POLICE_STATION) return "Police Station";
        if (type == FACILITY_FIRE_STATION) return "Fire Station";
        return "Unknown";
    }

    // ============ EMERGENCY FACILITY SPECIAL METHODS ============

    // Add hospital with emergency details (still uses addFacility internally)
    bool addHospital(string id, string name, double x, double y,
        int emergencyBeds, string specialization) {
        if (!addFacility(id, name, x, y, FACILITY_HOSPITAL)) {
            return false;
        }

        // Add emergency metadata 
        cout << "  Emergency Beds: " << emergencyBeds << endl;
        cout << "  Specialization: " << specialization << endl;

        return true;
    }

    // Add police station
    bool addPoliceStation(string id, string name, double x, double y) {
        return addFacility(id, name, x, y, FACILITY_POLICE_STATION);
    }

    // Add fire station  
    bool addFireStation(string id, string name, double x, double y) {
        return addFacility(id, name, x, y, FACILITY_FIRE_STATION);
    }

    // Display only emergency facilities
    void displayEmergencyFacilities() {
        cout << "\n=== EMERGENCY FACILITIES ===\n";

        int hospitals = 0, police = 0, fire = 0;

        for (int i = 0; i < cityGraph->getNodeCount(); i++) {
            GraphNode* node = cityGraph->getNode(i);
            string type = node->type;

            if (type == FACILITY_HOSPITAL || type == FACILITY_POLICE_STATION ||
                type == FACILITY_FIRE_STATION) {

                if (hospitals + police + fire == 0) {
                    cout << "ID\tType\t\t\tName\t\t\tDetails\n";
                   
                }

                cout << node->id << "\t";
                cout << type;
                if (type.length() < 8) cout << "\t\t";
                else cout << "\t";

                cout << "\t" << node->name;
                if (node->name.length() < 16) cout << "\t\t";

                // Show additional info for hospitals
                if (type == FACILITY_HOSPITAL) {
                    cout << "[Emergency Facility]";
                }
                else {
                    cout << "[Emergency Service]";
                }
                cout << endl;

                if (type == FACILITY_HOSPITAL) hospitals++;
                else if (type == FACILITY_POLICE_STATION) police++;
                else if (type == FACILITY_FIRE_STATION) fire++;
            }
        }

        if (hospitals + police + fire == 0) {
            cout << "No emergency facilities added yet.\n";
        }
        else {
            cout << "\n--- TOTALS ---\n";
            cout << "Hospitals: " << hospitals << endl;
            cout << "Police Stations: " << police << endl;
            cout << "Fire Stations: " << fire << endl;
            cout << "Total Emergency Facilities: " << (hospitals + police + fire) << endl;
        }
    }

    // Add demo emergency facilities
    void addDemoEmergencyFacilities() {
        cout << "\n=== ADDING DEMO EMERGENCY FACILITIES ===\n";

        // Check if bus stops exist first
        int busStopCount = 0;
        for (int i = 0; i < cityGraph->getNodeCount(); i++) {
            if (cityGraph->getNode(i)->type == "BUS_STOP") {
                busStopCount++;
            }
        }

        if (busStopCount == 0) {
            cout << "WARNING: No bus stops found!\n";
            cout << "Emergency facilities need bus stops to connect to.\n";
            cout << "They will be added but won't connect until bus stops are loaded.\n";
        }

        // Add hospitals
        cout << "\n--- HOSPITALS ---\n";
        addHospital("H01", "PIMS Hospital", 33.706, 73.055, 12, "General, Cardiology");
        addHospital("H02", "Shifa International", 33.695, 73.045, 20, "Oncology, Surgery");
        addHospital("H03", "Maroof Hospital", 33.691, 73.019, 8, "Orthopedics");
        addHospital("H04", "Poly Clinic", 33.710, 73.030, 14, "General");
        addHospital("H05", "Islamabad Medical", 33.720, 73.040, 10, "Pediatrics");

        // Add police stations
        cout << "\n--- POLICE STATIONS ---\n";
        addPoliceStation("PS1", "G-10 Police Station", 33.685, 73.027);
        addPoliceStation("PS2", "F-8 Police Station", 33.709, 73.038);
        addPoliceStation("PS3", "Blue Area Police Station", 33.721, 73.066);

        // Add fire stations
        cout << "\n--- FIRE STATIONS ---\n";
        addFireStation("FS1", "Central Fire Station", 33.700, 73.035);
        addFireStation("FS2", "North Fire Station", 33.730, 73.040);

        cout << "\n✓ Added 10 demo emergency facilities\n";

        // Try to connect if bus stops exist
        if (busStopCount > 0) {
            cout << "\nConnecting to existing bus stops...\n";
            connectFacilitiesToStops();
        }
    }

    // Emergency routing with priority
    void emergencyRouting(string startId, string emergencyType) {
        cout << "\n === EMERGENCY ROUTING ACTIVATED ===" << endl;
        cout << "From: " << startId << endl;
        cout << "Emergency Type: " << emergencyType << endl;
        cout << "Finding nearest facility with PRIORITY routing...\n";

        string targetType;
        if (emergencyType == "HOSPITAL") {
            targetType = FACILITY_HOSPITAL;
        }
        else if (emergencyType == "POLICE") {
            targetType = FACILITY_POLICE_STATION;
        }
        else if (emergencyType == "FIRE_STATION") {
            targetType = FACILITY_FIRE_STATION;
        }
        else {
            cout << "ERROR: Invalid emergency type!\n";
            return;
        }

        string nearestFacility = cityGraph->findNearestNodeOfType(startId, targetType);

        if (!nearestFacility.empty()) {
            GraphNode* facility = cityGraph->getNodeById(nearestFacility);
            if (facility) {
                cout << "\nNEAREST " << emergencyType << " FOUND!\n";
                cout << "  Name: " << facility->name << endl;
                cout << "  ID: " << facility->id << endl;
                cout << "  Coordinates: (" << facility->x << ", " << facility->y << ")\n";

                // Get shortest path using Dijkstra
                cout << "\n EMERGENCY ROUTE INSTRUCTIONS:\n";
                cityGraph->dijkstra(startId, facility->id);

                cout << "\n EMERGENCY PROTOCOL:\n";
                if (emergencyType == "HOSPITAL") {
                    cout << "1. Call 1122 for ambulance\n";
                    cout << "2. Keep patient calm and still\n";
                    cout << "3. Provide first aid if trained\n";
                }
                else if (emergencyType == "POLICE") {
                    cout << "1. Call 15 for police\n";
                    cout << "2. Stay in safe location\n";
                    cout << "3. Provide clear location details\n";
                }
                else if (emergencyType == "FIRE_STATION") {
                    cout << "1. Call 16 for fire department\n";
                    cout << "2. Evacuate immediately\n";
                    cout << "3. Do not use elevators\n";
                }
            }
        }
        else {
            cout << "\n NO " << emergencyType << " FOUND WITHIN REACHABLE DISTANCE!\n";
            cout << "Try alternative transport or call emergency directly.\n";
        }
    }

    // Add sample public facilities for testing
    void addSampleFacilities() {
        cout << "\n=== ADDING SAMPLE PUBLIC FACILITIES ===\n";

        // Check if bus stops exist
        int busStopCount = 0;
        for (int i = 0; i < cityGraph->getNodeCount(); i++) {
            if (cityGraph->getNode(i)->type == "BUS_STOP") {
                busStopCount++;
            }
        }

        if (busStopCount == 0) {
            cout << "WARNING: No bus stops found!\n";
            cout << "Facilities need bus stops to connect to.\n";
        }

        // MOSQUES
        addFacility("M1", "Faisal Mosque", 33.730, 73.039, FACILITY_MOSQUE);
        addFacility("M2", "G-10 Markaz Mosque", 33.6845, 73.026, FACILITY_MOSQUE);
        addFacility("M3", "Blue Area Mosque", 33.721, 73.066, FACILITY_MOSQUE);
        addFacility("M4", "PIMS Mosque", 33.707, 73.056, FACILITY_MOSQUE);

        // PARKS
        addFacility("P1", "F-10 Children Park", 33.6915, 73.020, FACILITY_PARK);
        addFacility("P2", "Lake View Park", 33.720, 73.101, FACILITY_PARK);
        addFacility("P3", "F-6 Community Park", 33.728, 73.076, FACILITY_PARK);
        addFacility("P4", "G-9 Green Park", 33.701, 73.031, FACILITY_PARK);

        // WATER COOLERS
        addFacility("WC1", "G-10 Markaz Cooler", 33.6842, 73.0255, FACILITY_WATER_COOLER);
        addFacility("WC2", "PIMS Water Point", 33.7065, 73.0555, FACILITY_WATER_COOLER);
        addFacility("WC3", "Centaurus Mall Cooler", 33.7085, 73.0415, FACILITY_WATER_COOLER);
        addFacility("WC4", "Faisal Mosque Cooler", 33.7295, 73.0385, FACILITY_WATER_COOLER);
        addFacility("WC5", "Blue Area Water Station", 33.7205, 73.0655, FACILITY_WATER_COOLER);
        addFacility("WC6", "F-6 Super Cooler", 33.7275, 73.0755, FACILITY_WATER_COOLER);

        cout << "\n Added 14 sample public facilities:\n";
        cout << "  - 4 Mosques\n";
        cout << "  - 4 Parks\n";
        cout << "  - 6 Water Coolers\n";

        // Try to connect if bus stops exist
        if (busStopCount > 0) {
            cout << "\nConnecting to existing bus stops...\n";
            connectFacilitiesToStops();
        }
    }

};

#endif // FACILITIES_H