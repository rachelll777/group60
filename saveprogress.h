#ifndef SAVEPROGRESS_H
#define SAVEPROGRESS_H

#include "player.h"
#include <vector>
#include <string>
#include <utility>
#include <fstream>

using namespace std;


bool saveGameProgress(Player* p, const vector<pair<int, int>>& packageLocs, 
                     const pair<int, int>& customerLoc, int moveCount, 
                     int stepLimits, int numPackages, const string& difficulty,
                     const vector<vector<string>>& currentMap);

bool loadGameProgress(Player*& p, vector<pair<int, int>>& packageLocs, 
                     pair<int, int>& customerLoc, int& moveCount, 
                     int& stepLimits, int& numPackages, string& difficulty,
                     vector<vector<string>>& currentMap);

#endif