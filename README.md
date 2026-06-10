# Xonix

## Project Overview
Xonix is a C++17 SFML game built as a university Data Structures project. It extends the classic Xonix concept with a screen-based game flow, user accounts, single-player and multiplayer modes, a leaderboard, matchmaking, friend management, save/resume support, and an inventory-driven theme system.

The application is organized around a central state machine in `src/main.cpp`, with dedicated screens for login, registration, menus, gameplay, multiplayer flow, leaderboard, profile, friends, themes, and saved games.

## Tech Stack
- C++17
- SFML for graphics, windowing, audio, and network support
- CMake build system
- File-based persistence for users, leaderboard entries, themes, and saved games

## Features
- Login and registration flow with file-backed player data
- Main menu navigation with access to gameplay, leaderboard, profile, friends, themes, saved games, logout, and exit
- Single-player gameplay with difficulty selection
- Multiplayer gameplay with a separate game room and end-game flow
- Score tracking, captured-tile progression, and power-up handling
- Leaderboard display and updates
- Friend management with search, requests, and friend list interactions
- Save and resume support for stored game sessions
- Theme selection through an inventory screen
- Pause and end-game screens for both single-player and multiplayer modes

## Data Structures Used
- Min-heap for the leaderboard implementation in `Leaderboard`
- Hash table for player lookup and account management in `PlayerManager`
- Linked lists for friends and pending requests in `Player`
- Array-based priority matchmaking queue in `MatchmakingSystem`
- AVL tree for theme storage and ordered retrieval in `ThemeAVLTree`
- Fixed-size arrays and 2D grids for game state, enemies, and save data

## How to Compile and Run
### Requirements
- CMake 3.10 or newer
- A C++17-compatible compiler
- SFML installed and discoverable by CMake

### Build
From the project root:

```bash
cmake -S . -B build
cmake --build build
```

### Run
Run the executable from the build directory so the relative data paths resolve correctly:

```bash
cd build
./xonix
```

On Windows, run:

```powershell
cd build
.\xonix.exe
```

The project expects its data files under `data/` and copies the `images/` folder into the build output during configuration.

## Role in Project
I served as the team lead in a 3-person group and coordinated the overall structure, SFML integration, and gameplay flow.

Sameer Ahmed focused on the overall project setup, SFML integration, menu structure, input validation, file handling, gameplay scoring and power-ups, Player 1 controls in multiplayer, serialization and deserialization for saved games, and theme selection and in-order traversal in the inventory system. He also handled integration testing across the project.

Muhammad Subhan worked on the user authentication flow, start and end menu logic, enemy interaction and tile capture behavior, Player 2 controls and interaction rules in multiplayer, heap implementation for the leaderboard, linked list implementation for the friend system, and gameplay testing.

Fatima Manzoor contributed to data persistence and structuring for the login system, collision handling and scoring updates, score display and power-up effects in multiplayer, score-based matchmaking updates, hash table support for friend requests, file I/O and save-state reconstruction, AVL insertions and search in the inventory system, and testing for the leaderboard and matchmaking systems.

### Team Members
- Sameer Ahmed - GitHub: [sameer7075](https://github.com/sameer7075)
- Muhammad Subhan - GitHub: [Denarzai](https://github.com/Denarzai)
- Fatima Manzoor - GitHub: pending