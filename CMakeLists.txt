cmake_minimum_required(VERSION 3.10)
project(Xonix)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(SFML REQUIRED network audio graphics window system)

# Copy images folder to build
file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/images" DESTINATION "${CMAKE_CURRENT_BINARY_DIR}/")

# Include directories
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/include")

# Source files
set(SOURCES
    src/main.cpp
    src/core/player.cpp
    src/core/playerManager.cpp
    src/core/leaderboard.cpp
    src/core/auth.cpp
    src/core/saveGame.cpp
    src/core/saveGameManager.cpp
    src/core/multiplayer.cpp
    src/core/matchmaking.cpp
    src/core/theme.cpp
    src/core/themeInventory.cpp
    src/ui/button.cpp
    src/ui/textBox.cpp
    src/screens/loginScreen.cpp
    src/screens/registerScreen.cpp
    src/screens/gameScreen.cpp
    src/screens/leaderboardScreen.cpp
    src/screens/mainMenu.cpp
    src/screens/profileScreen.cpp    
    src/screens/startMenuScreen.cpp  
    src/screens/endGameScreen.cpp 
    src/screens/pauseScreen.cpp   
    src/screens/savedGamesScreen.cpp   
    src/screens/friendsScreen.cpp 
    src/screens/multiplayerGameScreen.cpp   
    src/screens/multiplayerPauseScreen.cpp   
    src/screens/multiplayerEndGameScreen.cpp   
    src/screens/gameRoomScreen.cpp   
    src/screens/inventoryScreen.cpp   
)

# Create executable
add_executable(xonix ${SOURCES})

# Link SFML libraries
target_link_libraries(xonix PRIVATE sfml-system sfml-window sfml-graphics sfml-network sfml-audio)
