#ifndef INVENTORY_H
#define INVENTORY_H

#include <vector>
#include <string>
#include <iostream>

using namespace std;

class Inventory {
private:
    vector<string> packages;

public:
    Inventory() {}
    
    void addPackage(const string& package) {
        packages.push_back(package);
        cout << "Picked up: " << package << endl;
        displayInventory();
    }
    
    void deliverAllPackages() {
        if (!packages.empty()) {
            cout << "🎉 Delivered " << packages.size() << " packages to customer!" << endl;
            packages.clear();
        }
    }
    
    int getPackageCount() const { return packages.size(); }
    bool isEmpty() const { return packages.empty(); }
    
    void displayInventory() const {
        cout << "📦 Inventory (" << packages.size() << "): ";
        for (const auto& pkg : packages) cout << pkg << " ";
        cout << endl;
    }
    
    const vector<string>& getPackages() const { return packages; }
};

#endif
