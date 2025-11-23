#ifndef PLAYER_H
#define PLAYER_H

#include <cstdlib>
#include <iostream>
#include <vector>
#include <string>

using namespace std;

struct Player {
    int moveCount;
    int moveLimit;
    vector<string> inventory;
    pair<int, int> loc;

    // add package to inventory
    void addPackage(const string& package);

    // deliver packages and empty inventory
    void deliverAllPackages();

    // count packages
    int getPackageCount();

    // check if inventory is empty
    bool hasPackages();

    // display current inventory
    void displayInv();

    // pick up packages when reach packages
    bool pickupPackageFromMap(vector<vector<string>>& map, int row, int col);

    // deliver to customer
    bool deliverToCustomer(const vector<vector<string>>& map, int row, int col);

};

#endif