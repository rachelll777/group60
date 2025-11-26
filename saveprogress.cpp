#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include "player.h"
using namespace std;

// Saves the current game state into a file for future loading
bool saveGameProgress(Player* p, const vector<pair<int, int>>& packageLocs, 
                     const pair<int, int>& customerLoc, int moveCount, 
                     int stepLimits, int numPackages, const string& difficulty) {
    
    ofstream saveFile("game_save.txt");
    if (!saveFile.is_open()) {
        cout << "Error: Cannot create save file!" << endl;
        return false;
    }
    
    // Save basic information - variable names are consistent with main.cpp
    saveFile << difficulty << endl;
    saveFile << p->loc.first << " " << p->loc.second << endl;
    saveFile << moveCount << endl;
    saveFile << stepLimits << endl;
    saveFile << numPackages << endl;
    
    // Save customer position
    saveFile << customerLoc.first << " " << customerLoc.second << endl;
    
    // Save player inventory
    saveFile << p->inventory.size() << endl;
    for (const auto& item : p->inventory) {
        saveFile << item << endl;
    }
    
    // Save remaining package positions
    saveFile << packageLocs.size() << endl;
    for (const auto& loc : packageLocs) {
        saveFile << loc.first << " " << loc.second << endl;
    }
    
    saveFile.close();
    cout << "Game progress saved successfully!" << endl;
    return true;
}

bool loadGameProgress(Player*& p, vector<pair<int, int>>& packageLocs, 
                     pair<int, int>& customerLoc, int& moveCount, 
                     int& stepLimits, int& numPackages, string& difficulty) {
    
    ifstream saveFile("game_save.txt");
    if (!saveFile.is_open()) {
        cout << "No saved game found!" << endl;
        return false;
    }
    
    // Load basic information - variable names are consistent with main.cpp
    saveFile >> difficulty;
    
    // Load player position
    int playerX, playerY;
    saveFile >> playerX >> playerY;
    p->loc = {playerX, playerY};
    
    // Load move-related information
    saveFile >> moveCount;
    saveFile >> stepLimits;
    saveFile >> numPackages;
    
    // Load customer position
    int custX, custY;
    saveFile >> custX >> custY;
    customerLoc = {custX, custY};
    
   // Load player inventory
    p->inventory.clear();
    int inventorySize;
    saveFile >> inventorySize;
    for (int i = 0; i < inventorySize; i++) {
        string item;
        saveFile >> item;
        p->inventory.push_back(item);
    }
    
    // Load remaining package positions
    packageLocs.clear();
    int packageCount;
    saveFile >> packageCount;
    for (int i = 0; i < packageCount; i++) {
        int x, y;
        saveFile >> x >> y;
        packageLocs.push_back({x, y});
    }
    
    saveFile.close();
    cout << "Game loaded successfully!" << endl;
    return true;
}
