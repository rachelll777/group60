# **Group60 Package Panic 📦**
## Group Members:
* Hu Xinyue-3036292556
* Jia Qinhuan-3036457223
* Wu Yuerong-3036440579
* Zhang Linghui-3036292544
* Zhou Zhixuan-3036445660


## Game Desciption 🚚
> **Package Panic** is a fast-paced maze game where you become a delivery driver on a mission! Your goal is simple: **Collect** all the packages scattered in the maze and **Deliver** them to the customer. *The catch?* **Every time you play, the maze is different!** The game randomly generates the positions of `player`, `packages`, and `customer`. A smart algorithm calculates the shortest possible path for each unique layout, setting a tight and fair **move limit** for you to beat base on level difficulty. **Jump in and see if you have what it takes to be the fastest deliverer in town!**


## Demostration video 📹
      https
--------------------------

## Implemented Features 🔍
1. **Generation of random events**: The positions of the player, customer, and packages are randomly generated for a unique experience every time.
2. **Data structures for storing data**: Vectors, pairs, and maps are utilised to store game state including player, customer and packages' locations, player's inventory, pathfinding solutions and step limitation.
3. **Dynamic memory management**: The Player object is dynamically allocated using new and deallocated with delete at the end of the game session(dynamic array). Our code used vectors and pointers as well. 
4. **File input/output**: The saveprogress file enables saving and loading of game progress.
5. **Multiple files**: The codebase is organized across multiple header and implementation files。
6. **Multiple difficulty levels**: `Easy mode`: 10x10 map, 2 packages, `Hard mode`: 17x17 map, 5 packages

--------------------------
## Instructions 🗺️
1. **Start the Game:**
   - Use **`W/S`**to navigate, Use**`E`** to confirm and enter.
   - **Game Instruction**: You can view the game rules there.
   - Launch the game and choose your difficulty level
      - **`Easy`**: Small Map and Less Packages. Step limit is `double the shortest distance` player can explore and practice more.
      - **`Hard`**: Large Map and More Packages. Step limit is `the shortest distance+2`, be careful!
   
3. **Controls:**
   - Use the following keys to control your character:
      - **`W`**: Move up
      - **`A`**: Move left
      - **`S`**: Move down
      - **`D`**: Move right
      - **`Q`**: Quit the game without saving
      - **`V`**: Save the game then Quit
      - Important: The package is **automatically picked up and delivered** when the player passed package and customer respectively.

4. **Completion and Scoring:**
   - **Win**: Player delivered all Packages within step limits. The Success Page also show player's efficiency. 
   - **Lose**: Player is unabled to delivered all Packages within step limits. Or player quit the game without saving.

5. **To Continue the Saved Game:**
   - **`Continue`**: You can continue play the saved game on menu! The status of the game will always be the newest game version you saved.
   - **To Practice**: You can always play the saved game even you failed, as long as you don't save a new one.

--------------------------
## Non-Standard Libraries 📖
   N/A

--------------------------
## Compilation 💻
1. Open a terminal and navigate to the project's root directory:
   ```bash
   cd group60
2. Compile the game using the make command:
   ```bash
   make
3. Run the game:
   ```bash
   ./delivery_game
--------------------------
# **🎉Happy Gaming 🎉**


