#ifndef PACKAGE_MANAGER_H
#define PACKAGE_MANAGER_H

#include "inventory.h"
#include <vector>
#include <string>

using namespace std;

class PackageManager {
private:
    Inventory& playerInventory;

public:
    PackageManager(Inventory& inventory) : playerInventory(inventory) {}
    
    bool checkAndPickupPackage(vector<vector<string>>& map, int row, int col) {
        string cellContent = map[row][col];
        
        if (isPackage(cellContent)) {
            map[row][col] = " ";
            playerInventory.addPackage("Package" + cellContent);
            return true;
        }
        return false;
    }
    
    bool checkAndDeliverToCustomer(const vector<vector<string>>& map, int row, int col) {
        if (map[row][col] == "C") {
            playerInventory.deliverAllPackages();
            return true;
        }
        return false;
    }
    
private:
    bool isPackage(const string& cellContent) const {
        return cellContent >= "1" && cellContent <= "5";
    }
};

#endif
