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
};

#endif
