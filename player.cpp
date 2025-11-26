#include <cstdlib> 
#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <vector>

using namespace std;

struct Player {
    int moveCount;
    int moveLimit;
    vector<string> inventory;
    pair<int, int> loc;

    // add package to inventory
    void addPackage(const string& package){
        inventory.push_back(package);
        cout << "pick-up package" << package << endl;
        displayInv();
    }

    // deliver packages and empty inventory
    void deliverAllPackages(){
        if (!inventory.empty()){
            cout << "Successfully delivered " << inventory.size() << "package(s) to customer!" << endl;
            inventory.clear();
            cout << "Inventory is empty!" << endl;
        }
        else{
            cout << "No package can be delivered!" << endl;}
    }

    // count packages
    int getPackageCount() {
        return inventory.size();
    }

    // check if inventory is empty
    bool hasPackages(){
        return !inventory.empty();
    }

    // display current inventory
    void displayInv(){
        cout << "📦 Inventory: ";
        if (inventory.empty()){
            cout << "empty";}
        else{
            for (const auto& pkg : inventory) cout << pkg << " ";}
        cout << endl;
    }

    // pick up packages when reach packages
    bool pickupPackageFromMap(vector<vector<string>>& map, int row, int col) {
        string cellContent = map[row][col];
        
        // check if pkg
        if (cellContent == "1" || cellContent == "2" || cellContent == "3" || 
            cellContent == "4" || cellContent == "5") {
            
            // remove pkg from map
            map[row][col] = " ";
            
            // add to inventory
            string packageName = "Package" + cellContent;
            addPackage(packageName);
            return true;
        }
        return false;
    }

    // deliver to customer
    void deliverToCustomer(vector<vector<string>>& map, pair<int, int> loc) {
        
        // check customer's location
        if (map[loc.first][loc.second] == "C") {
            if (hasPackages()) deliverAllPackages();
        }
    }
};
