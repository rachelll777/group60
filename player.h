#ifndef PLAYER_H
#define PLAYER_H

#include <cstdlib>
#include <iostream>
#include <vector>
#include <string>

using namespace std;
// Represents the player’s status including position, inventory, and move limits.
struct Player {
    int moveCount;
    int moveLimit;
    vector<string> inventory;
    pair<int, int> loc;

    void addPackage(const string& package);
    void deliverAllPackages();
    int getPackageCount();
    bool hasPackages();
    void displayInv();
    bool pickupPackageFromMap(vector<vector<string>>& map, int row, int col);
    void deliverToCustomer(vector<vector<string>>& map, pair<int, int> loc);
};

#endif
