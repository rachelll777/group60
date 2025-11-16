#include <algorithm>
#include <cmath>
#include <cstring>
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <stdlib.h>

#include <unistd.h>

// 跨平台键盘输入处理
#ifdef _WIN32
    #include <conio.h>  // Windows 使用 conio.h
#else
    // macOS/Linux 使用 termios 实现 21getch()
    #include <termios.h>
    #include <stdio.h>
    
    int getch() {
        struct termios oldt, newt;
        int ch;
        tcgetattr(STDIN_FILENO, &oldt);
        newt = oldt;
        newt.c_lflag &= ~(ICANON | ECHO);
        tcsetattr(STDIN_FILENO, TCSANOW, &newt);
        ch = getchar();
        tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
        return ch;
    }
#endif

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

// Easy模式 10x10
vector<vector<string>> easyBaseMapTemplate =
    {
        {"#", "#", "#", "#", "#", "#", "#", "#", "#", "#"},
        {"#", " ", " ", " ", " ", " ", " ", " ", " ", "#"},
        {"#", " ", "#", "#", " ", "#", "#", "#", " ", "#"},
        {"#", " ", " ", "#", " ", " ", " ", " ", " ", "#"},
        {"#", "#", " ", "#", "#", "#", " ", "#", " ", "#"},
        {"#", " ", " ", " ", " ", " ", " ", "#", " ", "#"},
        {"#", " ", "#", "#", "#", "#", " ", "#", " ", "#"},
        {"#", " ", " ", " ", " ", " ", " ", " ", " ", "#"},
        {"#", " ", "#", "#", "#", "#", "#", "#", " ", "#"},
        {"#", "#", "#", "#", "#", "#", "#", "#", "#", "#"}
    };

// Hard模式 17x17
vector<vector<string>> hardBaseMapTemplate =
    {
        {"#", "#", "#", "#", "#", "#", "#", "#", "#", "#", "#", "#", "#", "#", "#", "#", "#"},
        {"#", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", "#"},
        {"#", " ", "#", "#", "#", " ", "#", "#", "#", " ", "#", "#", "#", " ", "#", " ", "#"},
        {"#", " ", " ", " ", "#", " ", " ", " ", "#", " ", "#", " ", " ", " ", "#", " ", "#"},
        {"#", "#", "#", " ", "#", "#", "#", " ", "#", " ", "#", " ", "#", "#", "#", " ", "#"},
        {"#", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", "#"},
        {"#", " ", "#", "#", "#", "#", "#", "#", "#", "#", "#", "#", "#", " ", "#", " ", "#"},
        {"#", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", "#", " ", "#"},
        {"#", "#", "#", "#", "#", " ", "#", "#", "#", "#", "#", " ", "#", "#", "#", " ", "#"},
        {"#", " ", " ", " ", " ", " ", "#", " ", " ", " ", "#", " ", " ", " ", " ", " ", "#"},
        {"#", " ", "#", "#", "#", "#", "#", " ", "#", " ", "#", "#", "#", "#", "#", " ", "#"},
        {"#", " ", " ", " ", " ", " ", " ", " ", "#", " ", " ", " ", " ", " ", " ", " ", "#"},
        {"#", "#", "#", "#", "#", " ", "#", "#", "#", "#", "#", " ", "#", "#", "#", " ", "#"},
        {"#", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", "#", " ", " ", " ", "#"},
        {"#", " ", "#", "#", "#", "#", "#", "#", "#", "#", "#", "#", "#", " ", "#", " ", "#"},
        {"#", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", " ", "#"},
        {"#", "#", "#", "#", "#", "#", "#", "#", "#", "#", "#", "#", "#", "#", "#", "#", "#"}
    };

// 在地图上随机放置元素
void placeItemsRandomly(vector<vector<string>>& map, int itemCount, int customerCount = 1) {
    int size = map.size();
    
    // 收集所有可用的空位置
    vector<pair<int, int>> availablePositions;
    
    for(int i = 0; i < size; i++) {
        for(int j = 0; j < size; j++) {
            if(map[i][j] == " ") {
                availablePositions.push_back({i, j});
            }
        }
    }
    
    // 随机打乱可用位置
    srand(time(0));
    for(int i = 0; i < availablePositions.size(); i++) {
        int swapWith = rand() % availablePositions.size();
        swap(availablePositions[i], availablePositions[swapWith]);
    }
    
    // 放置物品
    int totalToPlace = min(itemCount + customerCount, (int)availablePositions.size());
    
    // 放置顾客
    for(int i = 0; i < customerCount && i < totalToPlace; i++) {
        int x = availablePositions[i].first;
        int y = availablePositions[i].second;
        map[x][y] = "C";
        cout << "Customer placed at position: (" << x << ", " << y << ")" << endl;
    }
    
    // 放置物品
    for(int i = customerCount; i < totalToPlace; i++) {
        int x = availablePositions[i].first;
        int y = availablePositions[i].second;
        int itemNumber = (i - customerCount) % itemCount + 1;
        map[x][y] = to_string(itemNumber);
        cout << "Item " << itemNumber << " placed at position: (" << x << ", " << y << ")" << endl;
    }
}

// 生成指定模式的地图
vector<vector<string>> generateMap(string mode) {
    vector<vector<string>> map;
    
    if (mode == "easy") {
        map = easyBaseMapTemplate;
        placeItemsRandomly(map, 2, 1); // 2个物品，1个顾客
    } else if (mode == "hard") {
        map = hardBaseMapTemplate;
        placeItemsRandomly(map, 5, 1); // 5个物品，1个顾客
    }
    
    return map;
}

// 显示地图和物品信息
void displayMapInfo(vector<vector<string>>& map, string mode) {
    cout << "=== " << (mode == "easy" ? "EASY" : "HARD") << " MODE MAP ===" << endl;
    cout << "Map Size: " << map.size() << "x" << map[0].size() << endl;
    
    // 显示物品位置
    vector<pair<int, int>> items;
    vector<pair<int, int>> customers;
    
    for(int i = 0; i < map.size(); i++) {
        for(int j = 0; j < map[i].size(); j++) {
            if(map[i][j] == "1" || map[i][j] == "2" || map[i][j] == "3" || 
               map[i][j] == "4" || map[i][j] == "5") {
                items.push_back({i, j});
            } else if(map[i][j] == "C") {
                customers.push_back({i, j});
            }
        }
    }
    
    cout << "Items positions:" << endl;
    for(auto& item : items) {
        cout << "Item " << map[item.first][item.second] << " at (" << item.first << ", " << item.second << ")" << endl;
    }
    
    cout << "Customer position:" << endl;
    for(auto& customer : customers) {
        cout << "Customer at (" << customer.first << ", " << customer.second << ")" << endl;
    }
    cout << "=============================" << endl;
}

// 显示地图
void printMap(vector<vector<string>>& map) {
    for(int i = 0; i < map.size(); i++) {
        for(int j = 0; j < map[i].size(); j++) {
            cout << map[i][j] << " ";
        }
        cout << endl;
    }
}

// 游戏主函数
void playGame(string mode) {
    // 生成地图
    vector<vector<string>> gameMap = generateMap(mode);
    
    // 显示地图信息
    displayMapInfo(gameMap, mode);
    
    cout << "Starting game in " << mode << " mode..." << endl;
    cout << "Current Map:" << endl;
    printMap(gameMap);
    cout << endl;
    
    // 在这里添加player的移动内容
 
    cout << "Game completed! Returning to main menu..." << endl << endl;
}

// 主菜单
void showMainMenu() {
    while(true) {
        cout << "=== MAZE EXPLORATION GAME ===" << endl;
        cout << "1. Easy Mode (10x10 map)" << endl;
        cout << "   - 2 Items to collect" << endl;
        cout << "   - 1 Customer to find" << endl;
        cout << "2. Hard Mode (17x17 map)" << endl;
        cout << "   - 5 Items to collect" << endl;
        cout << "   - 1 Customer to find" << endl;
        cout << "3. Exit Game" << endl;
        cout << "=============================" << endl;
        cout << "Please select an option (1-3): ";
        
        int choice;
        cin >> choice;
        
        switch(choice) {
            case 1:
                playGame("easy");
                break;
            case 2:
                playGame("hard");
                break;
            case 3:
                cout << "Thank you for playing! Goodbye!" << endl;
                return;
            default:
                cout << "Invalid choice! Please select 1, 2, or 3." << endl;
                break;
        }
        
        // 清空输入缓冲区
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

int main() {
    showMainMenu();
    return 0;
}
