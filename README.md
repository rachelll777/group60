# **Group60 Package Panic 📦**
## Group Members:
* Hu Xinyue-3036292556
* Jia Qinhuan-3036457223
* Wu Yuerong-3036440579
* Zhang Linghui-3036292544
* Zhou Zhixuan-3036445660


## Game Desciption 🚚
> **Package Panic** is a fast-paced maze game where you become a delivery driver on a mission! Your goal is simple: **Collect** all the packages scattered in the maze and **Deliver** them to the customer. *The catch?* **Every time you play, the maze is different!** The game randomly generates the positions of `player`, `packages`, and `customer`. A smart algorithm calculates the shortest possible path for each unique layout, setting a tight and fair **move limit** for you to beat base on level difficulty. **Jump in and see if you have what it takes to be the fastest deliverer in town!**


## Demostration vedio 📹
https


## Implemented Features:
1. **Generation of random events**: The positions of the player, customer, and packages are randomly generated for a unique experience every time.
2. **Data structures for storing data**: Vectors, pairs, and maps are utilised to store game state including player, customer and packages' locations, player's inventory, pathfinding solutions and step limitation.
3. **Dynamic memory management**: The Player object is dynamically allocated using new and deallocated with delete at the end of the game session(dynamic array). Our code used vectors and pointers as well. 
4. **File input/output**: The saveprogress file enables saving and loading of game progress.
5. **Multiple files**: The codebase is organized across multiple header and implementation files (main.cpp, intro.cpp, end1.cpp, saveprogress.cpp)
6. **Multiple difficulty levels**: `Easy mode`: 10x10 map, 2 packages, `Hard mode`: 17 times 17 map, 5 packages

--------------------------
## Instructions:
1. **Start the Game:**
   - Launch the game and choose your difficulty level (**Easy** or **Hard**).
   
2. **Controls:**
   - Use the following keys to control your character:
     - **`A`**: Move left
     - **`D`**: Move right
     - **`W`**: Jump
     - **`Q`**: Quit the game
     - Important: In order to jump, you must hit **`W`** first, then hit **`A`** fast to jump leftwards (hit **`D`** fast to jump rightwards).
       

3. **Completion and Scoring:**
   
--------------------------
### Compilation:
1. Navigate to the project directory
```bash
   cd /path/to/group60
2. Compile using the make command: 
```bash
`make`
3. If you don't have make, manually compile using the command:
```bash
`g++ main.cpp player.cpp saveprogress.cpp end1.cpp -o delivery_game.exe`
4. Execution: 
```bash
`./delivery_game.exe`








