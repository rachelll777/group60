#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <math.h>
#include <stdlib.h>
#include <conio.h>
#include <iomanip>
#include <unistd.h>
#include <sstream>


#include <Windows.h>
#pragma execution_character_set("utf-8")

#include <fcntl.h>


using namespace std;


int input;
int introSelect = 0; //0: Lore, 1: instructions, 2: gameScreen, 3: achievementScreen
int introPrint = 0; //0: welcome, 1: lore, 2: instructions

string bGreen = "\033[1;32m"; // choose higher
string bRed = "\033[1;31m"; // choose lower
string bYellow = "\033[1;33m"; // Set Color
string bBlue = "\033[1;34m"; // Turn/Quota/Wallet Color
string white = "\033[0m";




string result = "                                                   ";


int N = 20;
int num1 = 5, num2 = 15;
int gameSelect = 1; //0: low, 1: high

double wallet=10, mult=1;
string stringTurns = "00";
string stringTotalTurns = "00";
string stringWallet = "000.00";
string stringQuota = "000.00";


double totalEarned = 0;
double totalLost = 0;
int lostStreak = 0;
int winStreak = 0;
int longestLostStreak = 0;
int longestWinStreak = 0;

bool notLostRoundYetForFlawlessWin = true;
bool endOfSetForFlawlessWin = false;
bool winSet = false;
bool winGame = false;
bool lostGame = false;
bool quitGame = false;


// Intro Screen
void printIntroScreen();

//Game Screen
void printGameScreen();

//Achievement Screen
void printAchievementsScreen();

// Back-end Main Function

//Creaint num 1 and num 2, then animating num 1: turn numbers
void genTurnNumbers();

//Animating num 2: next numbers
void genNextNumbers();

//Animating set changing aka next level transition
void genSetNumbers();

//Generating one round under a set:
// void genRound(Set *&s1);

// Generating one of seven sets:
// void genSet(Set *&s1);

// Achievements Checker, reprinting gamescreen if achievements unlocked
// void checkAchievements(Set *&s1);

// Restart game
void restart();

// Quit Game: lostGame = true;
void quit();

// Converting a vector<string> to a single string, good tool used in printIntroScreen(), printGameScreen, and printAchievementsScreen()
string vectorToString(vector<string> arr) {
    string s = "";
    for(string thing : arr) s = s+thing;
    return s; 
}



void printIntroScreen() {
    string top =                   "┏━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┓\n";
    string empty =                 "┃                                                                    ┃\n";
    vector<string> intro =        {"┃                      Race Against the Clock!                       ┃\n",
                                   "┃  An excellent courier should deliver the packages sound and fast!  ┃\n",
                                   "┃                           Your Mission:                            ┃\n",
                                   "┃       Pick up all packages and deliver them to the customer.       ┃\n",
                                   "┃                  The customer is getting impatient!                ┃\n",
                                   "┃       Hurry up and wisely seek for short cuts to save steps!       ┃\n",
                                   "┃                            Good luck！                             ┃\n"};
    vector<string> lore =         {"┃              Oh, so you want to know lore about this?              ┃\n",
                                   "┃             Well... the game all started with a \033[;36mPanda\033[;37m.             ┃\n",
                                   "┃             He relaxed under the calm spring sunshine.             ┃\n",
                                   "┃                             That's it.                             ┃\n",
                                   "┃                            (Part 01/25)                            ┃\n"};
    vector<string> instructions = {"┃  In each set, you have to reach a quota within a number of turns.  ┃\n",
                                   "┃  In each turn, you're given a number and you guess if the next is  ┃\n",
                                   "┃                          \033[1;32mhigher\033[0m or \033[1;31mlower\033[0m.                          ┃\n",
                                   "┃           The riskier the guess, the greater the reward,           ┃\n",
                                   "┃          but, also the worse the punishment... Good Luck!          ┃\n"};
    vector<string> butt1high =    {"┃                    ╭──────────────────────────╮                    ┃\n",
                                   "┃                    │ [1] Game Background/Lore │                    ┃\n",
                                   "┃                    ╰──────────────────────────╯                    ┃\n"};
    vector<string> butt1 =        {"┃                                                                    ┃\n",
                                   "┃                      [1] Game Background/Lore                      ┃\n",
                                   "┃                                                                    ┃\n"};
    vector<string> butt2high =    {"┃                        ╭──────────────────╮                        ┃\n",
                                   "┃                        │ [2] Instructions │                        ┃\n",
                                   "┃                        ╰──────────────────╯                        ┃\n"};
    vector<string> butt2 =        {"┃                                                                    ┃\n",
                                   "┃                          [2] Instructions                          ┃\n",
                                   "┃                                                                    ┃\n"};
    vector<string> butt3high =    {"┃                        ╭───────────────────╮                       ┃\n",
                                   "┃                        │ [3]  Start  Game  │                       ┃\n",
                                   "┃                        ╰───────────────────╯                       ┃\n"};
    vector<string> butt3 =        {"┃                                                                    ┃\n",
                                   "┃                           [3] Start Game                           ┃\n",
                                   "┃                                                                    ┃\n"};
    vector<string> butt4high =    {"┃                        ╭───────────────────╮                       ┃\n",
                                   "┃                        │  [4]  Continue    │                       ┃\n",
                                   "┃                        ╰───────────────────╯                       ┃\n"};
    vector<string> butt4 =        {"┃                                                                    ┃\n",
                                   "┃                           [4]  Continue                            ┃\n",
                                   "┃                                                                    ┃\n"};
    string controls =              "┃       [W][S]: Scroll through options  [Enter]: Quit                ┃\n";
    string bottom =                "┗━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━┛\n";

    cout << "\033[2J\033[1;1H";
    cout << top << empty << empty;
    if(introPrint == 0) for(string line : intro) cout << line;
    else if(introPrint == 1) for(string line : lore) cout << line;
    else if(introPrint == 2) for(string line : instructions) cout << line;
    cout << empty << empty;
    if(introSelect == 0) for(string line : butt1high) cout << line;
    else for(string line : butt1) cout << line;
    if(introSelect == 1) for(string line : butt2high) cout << line;
    else for(string line : butt2) cout << line;
    if(introSelect == 2) for(string line : butt3high) cout << line;
    else for(string line : butt3) cout << line;
    if(introSelect == 3) for(string line : butt4high) cout << line;
    else for(string line : butt4) cout << line;
    
    cout << empty << empty;
    cout << controls;
    cout << empty << empty;
    cout << bottom;

    input = getch(); //w: 119, 87 s: 115, 83
    if(input == 87 || input == 119) { // W/w
        introSelect = (introSelect-1+3)%3;
        printIntroScreen();
    } else if(input == 83 || input == 115) { // S/s
            introSelect = (introSelect+1+4)%4;
            printIntroScreen();
        } else if(input == 13) { // [Enter]
            return;
    } else return;
}


void printGameScreen() { printIntroScreen(); }

void printAchievementsScreen() { printIntroScreen(); }








void quit() {
    lostGame = true;
}

int main() {
    SetConsoleOutputCP(CP_UTF8);
    _setmode(_fileno(stdout), CP_UTF8);
    cout << fixed << setprecision(2);
    srand(time(0));

    

    printIntroScreen();
    // while(!quitGame && !winGame) {
    //     for(int i = 0; i < 3; i++) {
    //         if(quitGame) {
    //             break;
    //         }
    //     }
    // }
    cout << "Thank you for playing!" << endl;

    // genSet(s1);
    // genSet(s2);
    // genSet(s3);

    
    return 0;
}
