#pragma once
// Stack.h
#ifndef STACK_H
#define STACK_H

#include <iostream>
#include <string>
using namespace std;

#define STACK_SIZE 100

class RouteStack {
private:
    string stack[STACK_SIZE];  // Array-based stack
    int top;

public:
    // Constructor
    RouteStack() {
        top = -1;  // Empty stack
        // Initialize all to empty strings
        for (int i = 0; i < STACK_SIZE; i++) {
            stack[i] = "";
        }
    }

    // Push operation
    bool push(string location) {
        if (top >= STACK_SIZE - 1) {
            cout << "ERROR: Stack overflow! Cannot push " << location << endl;
            return false;
        }

        stack[++top] = location;
        cout << "[STACK] Pushed: " << location << " (Top=" << top << ")\n";
        return true;
    }

    // Pop operation
    string pop() {
        if (top < 0) {
            cout << "ERROR: Stack underflow! Stack is empty.\n";
            return "";
        }

        string location = stack[top--];
        cout << "[STACK] Popped: " << location << endl;
        return location;
    }

    // Peek operation
    string peek() {
        if (top < 0) {
            return "";
        }
        return stack[top];
    }

    // Check if stack is empty
    bool isEmpty() {
        return (top < 0);
    }

    // Check if stack is full
    bool isFull() {
        return (top >= STACK_SIZE - 1);
    }

    // Get stack size
    int getSize() {
        return top + 1;
    }

    // Display stack contents (FIXED: Show chronological order)
    void display() {
        if (isEmpty()) {
            cout << "Route history is empty.\n";
            return;
        }

        cout << "\n=== ROUTE HISTORY (" << getSize() << " entries) ===\n";
        cout << "Format: Oldest ? Newest (Top=" << top << ")\n";
        cout << "----------------------------------------\n";

        // Show in CHRONOLOGICAL order (oldest first)
        for (int i = 0; i <= top; i++) {
            cout << i << ": " << stack[i] << endl;
        }
        cout << "========================================\n";
    }

    // Alternative: Display most recent first (your original)
    void displayRecentFirst() {
        if (isEmpty()) {
            cout << "Route history is empty.\n";
            return;
        }

        cout << "\n=== ROUTE HISTORY (Most recent first) ===\n";
        cout << "Total entries: " << getSize() << endl;
        cout << "Most recent is at top\n";
        cout << "----------------------------\n";

        for (int i = top; i >= 0; i--) {
            cout << i << ": " << stack[i] << endl;
        }
        cout << "============================\n";
    }

    // Clear stack
    void clear() {
        top = -1;
        // Clear the array
        for (int i = 0; i < STACK_SIZE; i++) {
            stack[i] = "";
        }
        cout << "Route history cleared.\n";
    }

    // DEBUG METHOD: Show stack internals
    void debugInfo() {
        cout << "\n=== STACK DEBUG INFO ===\n";
        cout << "Top index: " << top << endl;
        cout << "Stack size: " << getSize() << endl;
        cout << "Is empty: " << (isEmpty() ? "YES" : "NO") << endl;
        cout << "Is full: " << (isFull() ? "YES" : "NO") << endl;

        if (!isEmpty()) {
            cout << "\nStack contents (array index: value):\n";
            for (int i = 0; i <= top; i++) {
                cout << "  [" << i << "]: \"" << stack[i] << "\"\n";
            }
            cout << "\nTop item (peek): \"" << peek() << "\"\n";
        }
        cout << "===========================\n";
    }

    // Get item at specific index (0 = oldest, top = newest)
    string getAt(int index) {
        if (index < 0 || index > top) {
            return "";
        }
        return stack[index];
    }
};

#endif // STACK_H