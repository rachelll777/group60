#ifndef PLAYER_H
#define PLAYER_H

#include <cstdlib> 
#include <iostream>
#include <vector>
#include <map>

#include <string.h>
#include <vector>

using namespace std;

struct Player {
    int moveCount;
    int moveLimit;
    vector<string> inventory;
    pair<int, int> loc;
};

#endif
