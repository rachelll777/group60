#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstdio>
#include <utility> 

#include "player.h"
using namespace std;

// Saves the current game state into a file for future loading
bool saveGameProgress(Player* p, const vector<pair<int, int> >& packageLocs, 
                     const pair<int, int>& customerLoc, int moveCount, 
                     int stepLimits, int numPackages, const string& difficulty, const vector<vector<string>>& currentMap) {  

    string filename = "./game_save.txt";
    
    // Delete previously saved file
    remove(filename.c_str());

    ofstream saveFile(filename);
    if (!saveFile.is_open()) {
        cout << "Error: Cannot create save file!" << endl;
        return false;
    }
    
    cout << "DEBUG: Starting save process..." << endl;
    cout << "DEBUG: Map size: " << currentMap.size() << "x" 
         << (currentMap.empty() ? 0 : currentMap[0].size()) << endl;
    
    // Save basic information
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

    // Ensure saving of map data
    cout << "DEBUG: Saving map data now..." << endl;
    if (currentMap.empty()) {
        cout << "ERROR: Current map is empty!" << endl;
        saveFile.close();
        return false;
    }
    
    cout << "DEBUG: Map dimensions: " << currentMap.size() << "x" << currentMap[0].size() << endl;
    
    // Save map size
    saveFile << currentMap.size() << " " << currentMap[0].size() << endl;
    
    // Save map content
    for (int i = 0; i < currentMap.size(); i++) {
        for (int j = 0; j < currentMap[i].size(); j++) {
            //Dealing with blank space
            if (currentMap[i][j] == " ") {
                saveFile << "_ ";
            } else {
                saveFile << currentMap[i][j] << " ";
            }
        }
        saveFile << endl;
        
        // Try output
        if (i < 2) {
            cout << "DEBUG: Saved row " << i << endl;
        }
    }
    
    saveFile.close();
    
    // Testing the content saved
    ifstream checkFile(filename);
    if (checkFile.is_open()) {
        string line;
        int lineCount = 0;
        while (getline(checkFile, line)) {
            lineCount++;
        }
        checkFile.close();
        cout << "DEBUG: Save file has " << lineCount << " lines" << endl;
    }
    
    cout << "Game progress saved successfully!" << endl;
    return true;
}

bool loadGameProgress(Player*& p, vector<pair<int, int> >& packageLocs, 
                     pair<int, int>& customerLoc, int& moveCount, 
                     int& stepLimits, int& numPackages, string& difficulty, vector<vector<string>>& currentMap) {
    
    string filename = "./game_save.txt";
    ifstream saveFile(filename);
    if (!saveFile.is_open()) {
        cout << "No saved game found!" << endl;
        return false;
    }
    
    // Load basic information
    saveFile >> difficulty;
    
    // Load player position
    int playerX, playerY;
    saveFile >> playerX;
    saveFile >> playerY;
    p->loc = {playerX, playerY};
    
    // Load move-related information
    saveFile >> moveCount;
    saveFile >> stepLimits;
    saveFile >> numPackages;
    
    // Load customer position
    int custX, custY;
    saveFile >> custX;
    saveFile >> custY;
    customerLoc = {custX, custY};
    
    // Load player inventory
    p->inventory.clear();
    int inventorySize;
    saveFile >> inventorySize;
    saveFile.ignore(); 
    for (int i = 0; i < inventorySize; i++) {
        string item;
        getline(saveFile, item);
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
    
    // Load map data
    int rows, cols;
    saveFile >> rows >> cols;
    cout << "DEBUG: Loading map of size " << rows << "x" << cols << endl;
    
    currentMap.clear();
    saveFile.ignore(); 
    
    for (int i = 0; i < rows; i++) {
        string line;
        getline(saveFile, line);
        
        vector<string> row;
        string cell;
        
        for (size_t j = 0; j < line.length(); j++) {
            char c = line[j];
            if (c == ' ') {
                if (!cell.empty()) {
                    // Transfer to blank space
                    if (cell == "_") {
                        row.push_back(" ");
                    } else {
                        row.push_back(cell);
                    }
                    cell.clear();
                }
            } else {
                cell += c;
            }
        }
        
        if (!cell.empty()) {
            if (cell == "_") {
                row.push_back(" ");
            } else {
                row.push_back(cell);
            }
        }
        
        // Testing the correct number of rows
        if (row.size() != cols) {
            cout << "WARNING: Row " << i << " has " << row.size() 
                 << " columns, expected " << cols << endl;
        }
        
        currentMap.push_back(row);
    }

    saveFile.close();
    cout << "DEBUG: Loaded map size: " << currentMap.size() << "x" 
         << (currentMap.empty() ? 0 : currentMap[0].size()) << endl;
    cout << "Game loaded successfully!" << endl;
    return true;
}