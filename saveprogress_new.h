#ifndef PROGRESS_SAVING_H
#define PROGRESS_SAVING_H

#include "player.h"
#include <vector>
#include <string>
#include <utility>

bool saveGameProgress(Player* p, const std::vector<std::pair<int, int>>& packageLocs, 
                     const std::pair<int, int>& customerLoc, int moveCount, 
                     int stepLimits, int numPackages, const std::string& difficulty);

bool loadGameProgress(Player*& p, std::vector<std::pair<int, int>>& packageLocs, 
                     std::pair<int, int>& customerLoc, int& moveCount, 
                     int& stepLimits, int& numPackages, std::string& difficulty);

#endif
