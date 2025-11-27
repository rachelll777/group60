#include "player_new.h"
#include <iostream>

// add package to inventory
void Player::addPackage(const std::string& package) {
    inventory.push_back(package);
    std::cout << "pick-up package " << package << std::endl;
    displayInv();
}

// deliver packages and empty inventory
void Player::deliverAllPackages() {
    if (!inventory.empty()) {
        std::cout << "Successfully delivered " << inventory.size()
                  << " package(s) to customer!" << std::endl;
        inventory.clear();
        std::cout << "Inventory is empty!" << std::endl;
    } else {
        std::cout << "No package can be delivered!" << std::endl;
    }
}

// count packages
int Player::getPackageCount() {
    return static_cast<int>(inventory.size());
}

// check if inventory is empty
bool Player::hasPackages() {
    return !inventory.empty();
}

// display current inventory
void Player::displayInv() {
    std::cout << "📦 Inventory: ";
    if (inventory.empty()) {
        std::cout << "empty";
    } else {
        for (const auto& pkg : inventory) {
            std::cout << pkg << " ";
        }
    }
    std::cout << std::endl;
}

// pick up packages when reach packages
bool Player::pickupPackageFromMap(std::vector<std::vector<std::string>>& map,
                                  int row, int col) {
    std::string cellContent = map[row][col];

    // check if pkg
    if (cellContent == "1" || cellContent == "2" || cellContent == "3" ||
        cellContent == "4" || cellContent == "5") {

        // remove pkg from map
        map[row][col] = " ";

        // add to inventory
        std::string packageName = "Package" + cellContent;
        addPackage(packageName);
        return true;
    }
    return false;
}

// deliver to customer
void Player::deliverToCustomer(std::vector<std::vector<std::string>>& map,
                               std::pair<int, int> loc) {
    // check customer's location
    if (map[loc.first][loc.second] == "C") {
        if (hasPackages()) {
            deliverAllPackages();
        }
    }
}
