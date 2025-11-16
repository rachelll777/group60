#ifndef MAP_H
#define MAP_H

#include <vector>
#include <string>

using namespace std;

class Map {
public:
    vector<vector<string>> currentMap;
    vector<int> playerLoc;  // [row, col]
    int mapIndex;
    
    // 多地图系统的指针
    Map* rightMap;
    Map* leftMap;
    Map* upMap;
    Map* downMap;
    
    // 出口位置
    int UpExit;
    int LeftExit;
    int DownExit;
    int RightExit;
    
    // 构造函数
    Map() : mapIndex(0), rightMap(nullptr), leftMap(nullptr), 
            upMap(nullptr), downMap(nullptr), UpExit(-1), 
            LeftExit(-1), DownExit(-1), RightExit(-1) {}
};

#endif
