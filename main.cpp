#include <algorithm>
#include <cmath>
#include <cstring>
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <iomanip>
#include <cstdlib>
#include <ctime>
#include <cstdio>

#include <unistd.h>

#include <map>
#include <queue>
#include <stack>

#include "player.h"
// #include "intro.h"
#include "end1.h"
#include "saveprogress.h"

// Cross-platform keyboard input handling
#ifdef _WIN32
    #include <conio.h>  // Windows uses conio.h
#else
    // macOS/Linux uses termios to implement getch()
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

// Color definitions
string bGreen = "\033[1;32m";
string bRed = "\033[1;31m";
string bYellow = "\033[1;33m";
string bBlue = "\033[1;34m";
string bMagenta = "\033[1;35m";
string bCyan = "\033[1;36m";
string white = "\033[0m";
string brown = "\033[38;5;94m";

bool winGame = false;
int moveCount =0;
int stepLimits =0;
int selectedDifficulty = 0; //0 for easy, 1 for hard



string input;


// Easy mode 10x10 map
vector<vector<string>> easyBaseMapTemplate = {
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

// Hard mode 17x17 map
vector<vector<string>> hardBaseMapTemplate = {
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

vector<vector<string>> currentMap;
pair<int, int> customerLoc = {};
int numPackages;
vector<pair<int, int>> packageLocs = {};
bool deliveringPackage = false;
int numDeliveredPackages = -1;
map<vector<int>, int> allSolutions = {};

// Randomly place items on the map
void placeItemsRandomly(Player *&p) {
    pair<int, int> loc = {rand()%currentMap.size(), rand()%currentMap[0].size()};
    vector<pair<int, int>> usedLocs = {};
    // player location
    while(currentMap[loc.first][loc.second] != " " || find(usedLocs.begin(), usedLocs.end(), loc) != usedLocs.end()) 
        loc = {rand()%currentMap.size(), rand()%currentMap[0].size()};
    p->loc = loc;
    usedLocs.push_back(loc);
    
    // customer location
    while(currentMap[loc.first][loc.second] != " " || find(usedLocs.begin(), usedLocs.end(), loc) != usedLocs.end()) 
        loc = {rand()%currentMap.size(), rand()%currentMap[0].size()};
    customerLoc = loc;
    usedLocs.push_back(loc);

    for(int i = 0; i < numPackages; i++) {
        while(currentMap[loc.first][loc.second] != " " || find(usedLocs.begin(), usedLocs.end(), loc) != usedLocs.end()) 
            loc = {rand()%currentMap.size(), rand()%currentMap[0].size()};
        packageLocs.push_back(loc);
        usedLocs.push_back(loc);
    }
}

void displayMapInfo(Player *&p) {
    cout << "\033[2J\033[1;1H";
    cout << bCyan << "Controls: " << bYellow << "W/A/S/D" << white << " to move, " 
         << bBlue << "V" << white << " to save, " << bRed << "Q" << white << " to quit" << endl;
    
    for(int i = 0; i < currentMap.size(); i++) {
        for(int j = 0; j < currentMap[i].size(); j++) {
            pair<int, int> loc = {i, j};
            bool isPackage = false;
            for(size_t k = 0; k < packageLocs.size(); k++) {
                if(packageLocs[k] == loc) {
                    isPackage = true;
                    break;
                }
            }
            if(loc == p->loc) 
                cout << bGreen << "P " << white;
            else if(loc == customerLoc) 
                cout << bCyan << "C " << white;
            else if(isPackage) 
                cout << bYellow << "1 " << white;
            else if(currentMap[i][j] == "#")
                cout << brown << "# " << white;
            else 
                cout << currentMap[i][j] << " ";
        }
        cout << endl;
    }
}

bool checkValidKey(string choice) {
    return choice == "W" || choice =="w" || choice =="A" || choice =="a" || 
           choice == "S" || choice =="s" || choice =="D" || choice =="d" || 
           choice == "Q" || choice == "q" || choice == "V" || choice == "v" ||
           choice == " ";
}

bool checkValidMovement(Player *&p, pair<int ,int> dir) {
    int newX = p->loc.first + dir.first;
    int newY = p->loc.second + dir.second;
    return (newX >= 0 && newX < currentMap.size() && 
            newY >= 0 && newY < currentMap[0].size() && 
            currentMap[newX][newY] != "#");
}

vector<vector<int>> allPaths = {};

void permute(vector<int> path, int start, int end) {
    if(start == end) {
        allPaths.push_back(path);
        return;
    } else {
        for(int i = start; i <= end; i++) {
            swap(path[start], path[i]);
            permute(path, start+1, end);
            swap(path[start], path[i]);
        }
    }
}

// Compute shortest path between 2 locations
int pathFinding(pair<int, int> loc1, pair<int, int> loc2) {
    vector<vector<bool>> visited(currentMap.size(), vector<bool>(currentMap[0].size(), false));
    visited[loc1.first][loc1.second] = true;
    queue<vector<int>> q;
    q.push({loc1.first, loc1.second, 0});
    
    vector<vector<int>> directions = {{-1, 0}, {1, 0}, {0, -1}, {0, 1}};

    while(!q.empty()) {
        int x = q.front()[0], y = q.front()[1], distance = q.front()[2];
        q.pop();
        
        if(x == loc2.first && y == loc2.second) return distance;
        for(vector<int> dir : directions) {
            int nextX = x+dir[0], nextY = y+dir[1];
            if(nextX >= 0 && nextX < currentMap.size() && 
               nextY >= 0 && nextY < currentMap[0].size() &&
               currentMap[nextX][nextY] != "#" && !visited[nextX][nextY]) {
                visited[nextX][nextY] = true;
                q.push({nextX, nextY, distance+1});
            }
        }
    }
    return -1;
}

// Calculate the shortest delivery path 
int shortestDelivery(Player *&p) {
    vector<bool> visited(numPackages+1, false);
    vector<int> defaultPath = {};
    for(int i = 0; i < numPackages; i++) defaultPath.push_back(i);
    
    allPaths.clear();
    permute(defaultPath, 0, numPackages-1);
    
    int distance = 99999;
    for(vector<int> path : allPaths) {
        int currDist = pathFinding(p->loc, packageLocs[path[0]]);
        for(int i = 0; i < path.size()-1; i++) 
            currDist += pathFinding(packageLocs[path[i]], packageLocs[path[i+1]]);
        currDist += pathFinding(packageLocs[path[numPackages-1]], customerLoc);
        if(currDist < distance) distance = currDist;
    }
    return distance;
}

// Player functions
void displayInv(Player *&p) {
    cout << "Packages Inventory: ";
    if (p->inventory.empty()){
        cout << "empty";
    } else {
        for (const auto& pkg : p->inventory) cout << pkg << " ";
    }
    cout << endl;
}

// deliver to customer
void deliverToCustomer(Player *&p, vector<vector<string>>& map, pair<int, int> loc) {
    if (p->loc == customerLoc) {
        if (p->inventory.size() != 0) {
            deliveringPackage = true;
            numDeliveredPackages = p->inventory.size();
            p->inventory.clear();
        }
    }
}

// pick up packages when reaching packages
void pickupPackageFromMap(Player *&p) {
    for(size_t i = 0; i < packageLocs.size(); i++) {
        pair<int, int> packLoc = packageLocs[i];
        if(packLoc == p->loc) {
            p->inventory.push_back(to_string(i+1));
            packageLocs.erase(packageLocs.begin()+i);
            cout << bGreen << "Picked up package " << (i+1) << "!" << white << endl;
            break;
        }
    }
}

void printMainMenu(int selection) {
    string top =    bBlue + "┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓\n" + white;
    string empty =  bBlue + "┃" + white + "                                                        " + bBlue + "┃\n" + white;
    string bottom = bBlue + "┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛\n" + white;
    
    vector<string> title = {
        bBlue + "┃" + white + "                     " + bYellow + "Package Panic!" + white + "                     " + bBlue + "┃\n" + white,
        bBlue + "┃" + white + "    An excellent courier should deliver packages        " + bBlue + "┃\n" + white,
        bBlue + "┃" + white + "    quickly and safely! The customer is getting         " + bBlue + "┃\n" + white,
        bBlue + "┃" + white + "    impatient, find shortcuts to save steps!            " + bBlue + "┃\n" + white,
        bBlue + "┃" + white + "                      " + bGreen + "Good Luck!" + white + "                        " + bBlue + "┃\n" + white
    };
    
    string butt1 = bBlue + "┃" + white + "               " + bYellow + "[1] Game Instructions" + white + "                    " + bBlue + "┃\n" + white;
    string butt1high = bBlue + "┃" + white + "               " + bCyan + "[1] Game Instructions" + white + "                    " + bBlue + "┃\n" + white;
    
    string butt2 = bBlue + "┃" + white + "               " + bYellow + "[2]  New Easy Game" + white + "                       " + bBlue + "┃\n" + white;
    string butt2high = bBlue + "┃" + white + "               " + bCyan + "[2]  New Easy Game" + white + "                       " + bBlue + "┃\n" + white;

    string butt3 = bBlue + "┃" + white + "               " + bYellow + "[3]  New Hard Game" + white + "                       " + bBlue + "┃\n" + white;
    string butt3high = bBlue + "┃" + white + "               " + bCyan + "[3]  New Hard Game" + white + "                       " + bBlue + "┃\n" + white;

    string butt4 = bBlue + "┃" + white + "               " + bYellow + "[4]  Continue Game" + white + "                       " + bBlue + "┃\n" + white;
    string butt4high = bBlue + "┃" + white + "               " + bCyan + "[4]  Continue Game" + white + "                       " + bBlue + "┃\n" + white;
    
    string controls = bBlue + "┃" + white + "    " + bGreen + "W/S:" + white + " Navigate    " + bYellow + "[ E ]:" + white + " Confirm    " + bRed + "Q:" + white + " Back          " + bBlue + "┃\n" + white;

    cout << "\033[2J\033[1;1H";
    cout << top << empty;
    for(string line : title) cout << line;
    cout << empty;
    
    // Display menu options
    if(selection == 0) {
        cout << butt1high;
        cout << butt2;
        cout << butt3;
        cout << butt4;
    } else if(selection == 1) {
        cout << butt1;
        cout << butt2high;
        cout << butt3;
        cout << butt4;
    } else if(selection == 2) {
        cout << butt1;
        cout << butt2;
        cout << butt3high;
        cout << butt4;
    } else if(selection == 3) {
        cout << butt1;
        cout << butt2;
        cout << butt3;
        cout << butt4high;
    }

    cout << empty << empty;
    cout << controls;
    cout << empty;
    cout << bottom;
}

void printInstructions() {
    string top =    bCyan + "┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓\n" + white;
    string empty =  bCyan + "┃" + white + "                                                        " + bCyan + "┃\n" + white;
    string bottom = bCyan + "┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛\n" + white;
    vector<string> gameRules = {
        bCyan + "┃" + white + "                   " + bYellow + "Game Instructions" + white + "                    " + bCyan + "┃\n" + white,
        bCyan + "┃" + white + "                                                        " + bCyan + "┃\n" + white,
        bCyan + "┃" + white + "  " + bGreen + "1." + white + " Choose difficulty: " + bYellow + "Rookie" + white + "(small map, 2 packages)   " + bCyan + "┃\n" + white,
        bCyan + "┃" + white + "     or " + bRed + "Expert" + white + "(large map, 5 packages)                   " + bCyan + "┃\n" + white,
        bCyan + "┃" + white + "  " + bGreen + "2." + white + " Use " + bYellow + "WASD" + white + " keys to move on the map (press ENTER)     " + bCyan + "┃\n" + white,
        bCyan + "┃" + white + "  " + bGreen + "3." + white + " Automatically pick up packages (" + bGreen + "1" + white + ") when stepping   " + bCyan + "┃\n" + white,
        bCyan + "┃" + white + "     on package locations                               " + bCyan + "┃\n" + white,
        bCyan + "┃" + white + "  " + bGreen + "4." + white + " Automatically deliver all packages at hand when    " + bCyan + "┃\n" + white,
        bCyan + "┃" + white + "     stepping on customer location (" + bCyan + "C" + white + ")                  " + bCyan + "┃\n" + white,
        bCyan + "┃" + white + "  " + bGreen + "5." + white + " Press " + bBlue + "V" + white + " to save game at any time                   " + bCyan + "┃\n" + white,
        bCyan + "┃" + white + "  " + bGreen + "6." + white + " Press " + bRed + "Q" + white + " to quit game                               " + bCyan + "┃\n" + white,
        bCyan + "┃" + white + "  " + bGreen + "7." + white + " Deliver all packages to customer within step limit " + bCyan + "┃\n" + white,
        bCyan + "┃" + white + "                                                        " + bCyan + "┃\n" + white,
        bCyan + "┃" + white + "  " + bGreen + "Win Condition:" + white + " Steps remaining >= 0 AND all packages  " + bCyan + "┃\n" + white,
        bCyan + "┃" + white + "                 delivered to customer                  " + bCyan + "┃\n" + white,
        bCyan + "┃" + white + "  " + bRed + "Lose Condition:" + white + " Run out of steps with packages        " + bCyan + "┃\n" + white,
        bCyan + "┃" + white + "                 remaining OR quit game                 " + bCyan + "┃\n" + white
    };

    cout << "\033[2J\033[1;1H";
    cout << top;
    for(string line : gameRules) cout << line;
    cout << empty;
    cout << bCyan + "┃" + white + "              Press " + bRed + "Q" + white + " to return to Main Menu            " + bCyan + "┃\n" + white;
    cout << empty;
    cout << bottom;
}




int main() {
    // Load intro menu
    
    

    int currentlySelecting = 0; //1 for instruct, 2 for new game, 3 for continue game

    printMainMenu(currentlySelecting);
    cin >> input;

    bool quitGame = false;
    while(input != "Q" && input != "q") {
        if(input == "W" || input == "w") { //W/w
            currentlySelecting = (currentlySelecting-1+4)%4;
        } else if(input == "S" || input == "s") { // S/s
            currentlySelecting = (currentlySelecting+1+4)%4;
        } else if(input == "E" || input == "e") { // E/e
            if(currentlySelecting == 0) {
                printInstructions();
                cin >> input;
            }
            else break;
        } else if(input == "1") {
            currentlySelecting = 0;
            printInstructions();
            cin >> input;
        } else if(input == "2") {
            currentlySelecting = 1;
            break;
        } else if(input == "3") {
            currentlySelecting = 2;
            break;
        } else if(input == "4") {
            currentlySelecting = 3;
            break;
        } else if(input == "Q" || input == "q") {
            quitGame = true;
            break;
        }
        printMainMenu(currentlySelecting);
        cin >> input;
    }


    if(quitGame) {
        cout << "Thanks for playing!" << endl;
        return 0;
    }
    srand(time(0));

    if(currentlySelecting == 1 || currentlySelecting == 2 || currentlySelecting == 3) { // New game or Load game
        cout << bCyan << "Starting game..." << white << endl;
        // sleep(1);
        Player *p = new Player();
        p->inventory = {};
        p->moveCount = 0;
        string difficulty = (currentlySelecting == 1) ? "easy" : "hard";

         // If loading saved game
        if(currentlySelecting == 3) {
            cout << bCyan << "Loading saved game..." << white << endl;

            if(difficulty == "easy") {
                currentMap = easyBaseMapTemplate;
                numPackages = 2;
            } else {
                currentMap = hardBaseMapTemplate;  
                numPackages = 5;
            }

            if(!loadGameProgress(p, packageLocs, customerLoc, moveCount, stepLimits, numPackages, difficulty, currentMap)) {
                cout << bRed << "Failed to load game. Starting new game." << white << endl;
                placeItemsRandomly(p);
                int minSteps = shortestDelivery(p);
                stepLimits = (difficulty=="easy") ? minSteps*2 : minSteps+2;
            } else {
                cout << bGreen << "Game loaded successfully!" << white << endl;
            }
        } else {
            currentMap = (difficulty=="easy") ? easyBaseMapTemplate : hardBaseMapTemplate;
            numPackages = (difficulty=="easy") ? 2 : 5;
            
            cout << bCyan << "Starting new game..." << white << endl;
            remove("game_save.txt");
            placeItemsRandomly(p);
            int minSteps = shortestDelivery(p);
            stepLimits = (difficulty=="easy") ? minSteps*2 : minSteps+2;
        }

        p->moveLimit = stepLimits;
        string choice = "";

        while(choice != "q" && choice != "Q") {
            displayMapInfo(p);
            cout << bBlue << "==========================================" << white << endl;
            
            // Move count with color coding based on remaining moves
            int remainingMoves = stepLimits - moveCount;
            string moveColor = (remainingMoves > stepLimits * 0.3) ? bGreen : 
                              (remainingMoves > stepLimits * 0.1) ? bYellow : bRed;
            
            cout << "Move count: " << moveColor << moveCount << white << " / " << stepLimits << endl;
            
            // Packages remaining with color
            cout << "Packages remaining: ";
            if(packageLocs.size() == 0) 
                cout << bGreen << packageLocs.size() << white;
            else 
                cout << bYellow << packageLocs.size() << white;
            cout << " / " << numPackages << endl;
            
            // Player inventory
            cout << "Player inventory: { ";
            if(p->inventory.empty()) {
                cout << bRed << "Empty" << white;
            } else {
                for(size_t i = 0; i < p->inventory.size(); i++) 
                    cout << bYellow << "| Package " << p->inventory[i] << " | " << white;
            }
            cout << " }" << endl;
            
            if(deliveringPackage) {
                cout << bGreen << "Successfully delivered " << numDeliveredPackages << " package(s) to customer!" << white << endl;
                numDeliveredPackages = -1;
                deliveringPackage = false;
            }
            
            cout << bBlue << "==========================================" << white << endl;
            cout << "Enter move (" << bYellow << "W/A/S/D" << white << "), " 
                 << bBlue << "V" << white << " to save, " 
                 << bRed << "Q" << white << " to quit: ";

            cin >> choice;

            if(choice == "Q" || choice == "q") break;
            
            // saving function
            if(choice == "V" || choice == "v") {
                if(saveGameProgress(p, packageLocs, customerLoc, moveCount, stepLimits, numPackages, difficulty, currentMap)) {
                    cout << bGreen << "Game saved successfully!" << white << endl;
                } else {
                    cout << bRed << "Failed to save game!" << white << endl;
                }
                // sleep(1);
                displayMapInfo(p);
                continue;
            }
            
            if(checkValidKey(choice)) {
                pair<int, int> dir = {};
                if(choice == "W" || choice == "w") dir = {-1, 0};
                else if(choice == "A" || choice == "a") dir = {0, -1};
                else if(choice == "S" || choice == "s") dir = {1, 0};
                else if(choice == "D" || choice == "d") dir = {0, 1};

                if(checkValidMovement(p, dir)) {
                    p->loc = {p->loc.first+dir.first, p->loc.second+dir.second};
                    moveCount = moveCount+1;
                    pickupPackageFromMap(p);
                    deliverToCustomer(p, currentMap, p->loc);

                    // Check win condition
                    if(packageLocs.empty() && p->inventory.empty()) {
                        winGame = true;
                        break;
                    }
                    
                } else {
                    cout << bRed << "Not a valid move. You hit a wall!" << white << endl;
                    // sleep(1);
                }

            } else {
                cout << bRed << "Not a valid key. Use W/A/S/D to move." << white << endl;
                // sleep(1);
            }
            
            // Check loss condition (out of moves)
            if(moveCount > stepLimits) {
                cout << bRed << "You ran out of moves!" << white << endl;
                // sleep(1);
                break;
            }
        }
        
        // Show end screen
        int packagesDelivered = numPackages - packageLocs.size();
        showEndScreen(winGame, moveCount, stepLimits, packagesDelivered, numPackages);

    // Delete save file when the game ends
        cout << bCyan << "Clearing save data..." << white << endl;
        // remove("game_save.txt");
        // cout << bGreen << "Save file deleted successfully." << white << endl;

        delete p;
        
        //  Exit message before quitting the game
        cout << bCyan << "Exiting the game..." << white << endl;
        // sleep(2);
    }

    cout << bYellow << "Thank you for playing!" << white << endl;
    return 0;
}