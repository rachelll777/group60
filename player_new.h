#ifndef PLAYER_H
#define PLAYER_H

#include <vector>
#include <string>
#include <utility>  // for std::pair

// Represents the player’s status including position, inventory, and move limits.
struct Player {
    int moveCount;
    int moveLimit;
    std::vector<std::string> inventory;
    std::pair<int, int> loc;

    // add package to inventory
    void addPackage(const std::string& package);

    // deliver packages and empty inventory
    void deliverAllPackages();

    // count packages
    int getPackageCount();

    // check if inventory is empty
    bool hasPackages();

    // display current inventory
    void displayInv();

    // pick up packages when reach packages
    bool pickupPackageFromMap(std::vector<std::vector<std::string>>& map, int row, int col);

    // deliver to customer
    void deliverToCustomer(std::vector<std::vector<std::string>>& map,
                           std::pair<int, int> loc);
};

#endif
