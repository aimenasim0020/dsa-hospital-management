// CSVHandler.h
#ifndef CSVHANDLER_H
#define CSVHANDLER_H

#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
using namespace std;

// Custom String Array class (replaces vector<string>)
class StringArray {
private:
    string* data;
    int capacity;
    int size;

    void resize() {
        int newCapacity = (capacity == 0) ? 2 : capacity * 2;
        string* newData = new string[newCapacity];

        for (int i = 0; i < size; i++) {
            newData[i] = data[i];
        }

        delete[] data;
        data = newData;
        capacity = newCapacity;
    }

public:
    StringArray() {
        data = nullptr;
        capacity = 0;
        size = 0;
    }

    ~StringArray() {
        delete[] data;
    }

    void add(string value) {
        if (size >= capacity) resize();
        data[size++] = value;
    }

    string get(int index) {
        if (index < 0 || index >= size) return "";
        return data[index];
    }

    int getSize() { return size; }

    void clear() {
        delete[] data;
        data = nullptr;
        capacity = 0;
        size = 0;
    }

    // For displaying
    void display() {
        for (int i = 0; i < size; i++) {
            cout << "[" << data[i] << "] ";
        }
        cout << endl;
    }
};

// CSV Data container (replaces vector<vector<string>>)
class CSVData {
private:
    StringArray* rows;
    int rowCapacity;
    int rowCount;
    int colCount;

    void resizeRows() {
        int newCapacity = (rowCapacity == 0) ? 2 : rowCapacity * 2;
        StringArray* newRows = new StringArray[newCapacity];

        for (int i = 0; i < rowCount; i++) {
            for (int j = 0; j < rows[i].getSize(); j++) {
                newRows[i].add(rows[i].get(j));
            }
        }

        delete[] rows;
        rows = newRows;
        rowCapacity = newCapacity;
    }

public:
    CSVData() {
        rows = nullptr;
        rowCapacity = 0;
        rowCount = 0;
        colCount = 0;
    }

    ~CSVData() {
        delete[] rows;
    }

    void addRow(StringArray& row) {
        if (rowCount >= rowCapacity) resizeRows();

        for (int i = 0; i < row.getSize(); i++) {
            rows[rowCount].add(row.get(i));
        }

        if (row.getSize() > colCount) {
            colCount = row.getSize();
        }

        rowCount++;
    }

    string getCell(int row, int col) {
        if (row < 0 || row >= rowCount || col < 0 || col >= colCount) return "";
        return rows[row].get(col);
    }

    int getRowCount() { return rowCount; }
    int getColCount() { return colCount; }

    void display() {
        cout << " CSV Data : " << rowCount << " rows, " << colCount << " columns\n";
        cout << "════════════════════════════════════════\n";
        for (int i = 0; i < rowCount; i++) {
            cout << "Row " << i << ": ";
            for (int j = 0; j < colCount; j++) {
                string cell = getCell(i, j);
                if (!cell.empty()) {
                    cout << "[" << cell << "] ";
                }
            }
            cout << endl;
        }
        cout << "════════════════════════════════════════\n";
    }
};

// Main CSV Handler class
class CSVHandler {
private:
    // Helper: Trim whitespace
    static string trim(const string& str) {
        int start = 0, end = str.length() - 1;
        while (start <= end && str[start] == ' ') start++;
        while (end >= start && str[end] == ' ') end--;

        if (start > end) return "";
        return str.substr(start, end - start + 1);
    }

    // Helper: Parse a CSV line
    static void parseCSVLine(const string& line, StringArray& row, char delimiter = ',') {
        string field;
        bool inQuotes = false;

        for (size_t i = 0; i < line.length(); i++) {
            char c = line[i];

            if (c == '"') {
                inQuotes = !inQuotes;
            }
            else if (c == delimiter && !inQuotes) {
                row.add(trim(field));
                field = "";
            }
            else if (c == '\\' && i + 1 < line.length()) {
                // Handle escape sequences
                field += line[++i];
            }
            else {
                field += c;
            }
        }

        // Add the last field
        if (!field.empty()) {
            row.add(trim(field));
        }
    }

    // Helper: Check if file exists
    static bool fileExists(const string& filename) {
        ifstream file(filename);
        bool exists = file.good();
        file.close();
        return exists;
    }

public:
    // Read CSV file and return data
    static CSVData* readCSV(const string& filename, bool hasHeader = true) {
        CSVData* data = new CSVData();

        if (!fileExists(filename)) {
            cout << " ERROR: File not found: " << filename << endl;
            return data;
        }

        ifstream file(filename);
        if (!file.is_open()) {
            cout << " ERROR: Cannot open file: " << filename << endl;
            return data;
        }

        string line;
        int lineNumber = 0;

        while (getline(file, line)) {
            lineNumber++;

            // Skip empty lines
            if (line.empty() || trim(line).empty()) continue;

            // Skip header if requested
            if (hasHeader && lineNumber == 1) continue;

            // Parse the line
            StringArray row;
            parseCSVLine(line, row);

            // Add to data
            data->addRow(row);
        }

        file.close();

        cout << " Loaded " << filename << ": " << data->getRowCount() << " records\n";
        return data;
    }

    // Special parser for bus routes (handles ">" separator)
    static StringArray* parseRouteString(const string& routeStr) {
        StringArray* stops = new StringArray();
        string currentStop;

        for (size_t i = 0; i < routeStr.length(); i++) {
            char c = routeStr[i];

            if (c == '>' || c == '-' || c == ',') {
                string trimmed = trim(currentStop);
                if (!trimmed.empty()) {
                    stops->add(trimmed);
                }
                currentStop = "";
            }
            else {
                currentStop += c;
            }
        }

        // Last stop
        string trimmed = trim(currentStop);
        if (!trimmed.empty()) {
            stops->add(trimmed);
        }

        return stops;
    }

    // Count lines in file (for debugging)
    static int countLines(const string& filename) {
        ifstream file(filename);
        if (!file.is_open()) return 0;

        int count = 0;
        string line;
        while (getline(file, line)) {
            if (!line.empty() && !trim(line).empty()) count++;
        }

        file.close();
        return count;
    }
};

#endif // CSVHANDLER_H