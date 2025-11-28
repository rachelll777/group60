#include <iostream>
#include <string>
#include <iomanip>
using namespace std;

// colour code
#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN    "\033[36m"
#define BOLD    "\033[1m"

void showEndScreen(bool winGame, int moveCount, int stepLimits, 
                  int packagesDelivered, int totalPackages) {
    cout << "\033[2J\033[1;1H"; 
    
    // calculating score and efficiency
    int movesRemaining = stepLimits - moveCount;
    double efficiency = (moveCount > 0) ? (double)packagesDelivered / moveCount * 100 : 0;
    int completionRate = (totalPackages > 0) ? (packagesDelivered * 100 / totalPackages) : 0;
    
    if (winGame) {
        // winning
        cout << BOLD << YELLOW;
        cout << "╔══════════════════════════════════════════════════╗" << endl;
        cout << "║            *** VICTORY CELEBRATION ***           ║" << endl;
        cout << "╠══════════════════════════════════════════════════╣" << RESET << endl;
        cout << YELLOW << "║  " << RESET << BOLD << GREEN << "CONGRATULATIONS! YOU'RE AN ELITE COURIER!" << YELLOW << "       ║" << RESET << endl;
        cout << YELLOW << "║                                                  ║" << RESET << endl;
        
        // make the text align
        cout << YELLOW << "║  " << RESET << "[P] " << "Packages Delivered: " 
             << BOLD << GREEN << setw(2) << packagesDelivered << "/" << setw(1) << totalPackages 
             << RESET << YELLOW << "                    ║" << RESET << endl;
             
        cout << YELLOW << "║  " << RESET << "[M] " << "Moves Used: " 
             << BOLD << CYAN << setw(2) << moveCount << "/" << setw(2) << stepLimits 
             << RESET << YELLOW << "                           ║" << RESET << endl;
             
        cout << YELLOW << "║  " << RESET << "[*] " << "Moves Remaining: " 
             << BOLD << GREEN << setw(2) << movesRemaining 
             << RESET << YELLOW << "                         ║" << RESET << endl;
             
        cout << YELLOW << "║  " << RESET << "[%] " << "Delivery Efficiency: " 
             << BOLD << MAGENTA << setw(3) << fixed << setprecision(1) << efficiency << "%" 
             << RESET << YELLOW << "                  ║" << RESET << endl;
        
        cout << YELLOW << "║                                                  ║" << RESET << endl;
        
        if(efficiency > 80) {
            cout << YELLOW << "║  " << BOLD << "[#] EXCEPTIONAL PERFORMANCE! MASTER COURIER!" << YELLOW << "   ║" << RESET << endl;
        } else if(efficiency > 60) {
            cout << YELLOW << "║  " << BOLD << "[+] GREAT JOB! PROFESSIONAL DELIVERY!" << YELLOW << "       ║" << RESET << endl;
        } else {
            cout << YELLOW << "║  " << BOLD << "[=] MISSION ACCOMPLISHED! GOOD WORK!" << YELLOW << "            ║" << RESET << endl;
        }
        
        cout << YELLOW << "╚══════════════════════════════════════════════════╝" << RESET << endl;
        
    } else {
        // losing
        cout << BOLD << RED;
        cout << "╔══════════════════════════════════════════════════╗" << endl;
        cout << "║              XXX MISSION FAILED XXX              ║" << endl;
        cout << "╠══════════════════════════════════════════════════╣" << RESET << endl;
        cout << RED << "║  " << RESET << BOLD << RED << "DELIVERY SERVICE TERMINATED - TRY AGAIN!" << RED << "        ║" << RESET << endl;
        cout << RED << "║                                                  ║" << RESET << endl;
        
        if (moveCount > stepLimits) {
            cout << RED << "║  " << RESET << BOLD << "[!] RAN OUT OF MOVES! TIME'S UP!" << RED << "                ║" << RESET << endl;
        } else {
            cout << RED << "║  " << RESET << BOLD << "[X] MISSION ABORTED - EARLY EXIT" << RED << "                ║" << RESET << endl;
        }
        
        cout << RED << "║                                                  ║" << RESET << endl;
        
        cout << RED << "║  " << RESET << "[P] " << "Packages Delivered: " 
             << BOLD << YELLOW << setw(2) << packagesDelivered << "/" << setw(1) << totalPackages 
             << RESET << RED << "                    ║" << RESET << endl;
             
        cout << RED << "║  " << RESET << "[M] " << "Moves Used: " 
             << BOLD << CYAN << setw(2) << moveCount << "/" << setw(2) << stepLimits 
             << RESET << RED << "                           ║" << RESET << endl;
             
        cout << RED << "║  " << RESET << "[T] " << "Completion Rate: " 
             << BOLD << YELLOW << setw(3) << completionRate << "%" 
             << RESET << RED << "                       ║" << RESET << endl;
        
        cout << RED << "║                                                  ║" << RESET << endl;
        cout << RED << "║  " << RESET << BOLD << CYAN << "Don't give up! Better luck next time!" << RED << "           ║" << RESET << endl;
        cout << RED << "╚══════════════════════════════════════════════════╝" << RESET << endl;
    }
    
    cout << endl << BOLD << CYAN;
    cout << "Press any key to return to main menu..." << RESET;
    getchar();
    getchar();
}