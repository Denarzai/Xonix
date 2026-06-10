#include <SFML/Graphics.hpp>
#include <time.h>
#include <iostream>
#include "../include/core/playerManager.h"
#include "../include/core/auth.h"
#include "../include/core/leaderboard.h"
#include "../include/core/difficulty.h"
#include "../include/core/themeInventory.h"
#include "../include/screens/loginScreen.h"
#include "../include/screens/registerScreen.h"
#include "../include/screens/mainMenu.h"
#include "../include/screens/gameScreen.h"
#include "../include/screens/leaderboardScreen.h"
#include "../include/screens/profileScreen.h"
#include "../include/screens/startMenuScreen.h"
#include "../include/screens/savedGamesScreen.h"
#include "../include/screens/friendsScreen.h"
#include "../include/screens/multiplayerGameScreen.h"
#include "../include/screens/multiplayerPauseScreen.h"
#include "../include/screens/multiplayerEndGameScreen.h"
#include "../include/screens/gameRoomScreen.h"
#include "../include/screens/inventoryScreen.h"

using namespace sf;

enum class GameState {
    LOGIN,
    REGISTER,
    MAINMENU,
    GAME_START_MENU,
    GAME_SINGLE_PLAYER,
    MULTIPLAYER,
    GAME_ROOM,
    LEADERBOARD,
    PROFILE,
    FRIENDS,
    INVENTORY,
    SAVED_GAMES,
    LOGOUT,
    EXIT
};


int main() {
    srand(time(0));
    //RenderWindow window(VideoMode(720, 450), "Xonix Game!");
    RenderWindow window(VideoMode(1200, 630), "Xonix Game!");
    window.setFramerateLimit(60);

    
    PlayerManager manager;
    Auth auth(&manager);
    Player* currentPlayer = nullptr;
    bool loggedIn = false;
    manager.loadFromFile("players_test.txt");
    Leaderboard leaderboard;
    leaderboard.loadFromFile("leaderboard.txt");

    LoginScreen login(manager, auth, window.getSize().x, window.getSize().y);
    RegisterScreen reg(manager, auth, window.getSize().x, window.getSize().y);
    MainMenu mainMenu(manager, auth, window.getSize().x, window.getSize().y);
    StartMenuScreen startMenu(window.getSize().x, window.getSize().y);
    LeaderboardScreen leaderboardScreen(manager, leaderboard, window.getSize().x, window.getSize().y);
    ProfileScreen profile(currentPlayer, window.getSize().x, window.getSize().y);
    PauseScreen pauseScreen(window.getSize().x, window.getSize().y);
    EndGameScreen endScreen(window.getSize().x, window.getSize().y, currentPlayer ? currentPlayer->getUsername() : "Unknown");
    SavedGamesScreen savedGamesScreen(&manager, currentPlayer, window.getSize().x, window.getSize().y);
    FriendsScreen friendsScreen(&manager, currentPlayer, window.getSize().x, window.getSize().y);
    MultiplayerPauseScreen multiplayerPauseScreen(window.getSize().x, window.getSize().y);
    

    GameScreen* gameScreen = nullptr;
    MultiplayerGameScreen* multiplayerScreen = nullptr;
    MultiplayerEndGameScreen* multiplayerEndGameScreen = nullptr;
    GameRoomScreen* gameRoomScreen = nullptr;
    ThemeInventory themeInventory;
    themeInventory.loadThemesFromFile("../data/themes.txt");
    InventoryScreen inventoryScreen(&themeInventory, &manager, window.getSize().x, window.getSize().y);


    GameState state = GameState::LOGIN;
    Difficulty difficulty = Difficulty::EASY;
    bool shouldExit = false;

    while (window.isOpen() && !shouldExit) {
        switch (state) {
        case GameState::LOGIN: {
            bool goToRegister = false;
            login.run(window, goToRegister, loggedIn, currentPlayer);
            if (goToRegister) {
                state = GameState::REGISTER;
                login.resetErrorMsg();
                login.resetInputFields();
            }
            if (loggedIn) {
                state = GameState::MAINMENU;
                login.resetErrorMsg();
                login.resetInputFields();
                mainMenu.resetMenu();
                themeInventory.setCurrentTheme(currentPlayer->getThemeID());
            }
            break;
        }
        
        case GameState::REGISTER: {
            bool goToLogin = false;
            reg.run(window, goToLogin);
            if (goToLogin) {
                state = GameState::LOGIN;
                reg.resetErrorMsg();
                reg.resetInputFields();
            }
            manager.saveToFile("players_test.txt");
            break;
        }
        
        case GameState::MAINMENU: {
            int nextState = 0;
            mainMenu.handleInput(window, shouldExit, nextState);
            mainMenu.update();
            mainMenu.draw(window);
            
            if (nextState == 100) {
                state = GameState::GAME_START_MENU;
            }
            else if (nextState == 1) {
                state = GameState::GAME_SINGLE_PLAYER;
            } else if (nextState == 2) {
                state = GameState::LEADERBOARD;
            } else if (nextState == 3) {
                state = GameState::PROFILE;
            } else if (nextState == 4) {
                state = GameState::FRIENDS;
            } else if (nextState == 5) {
                state = GameState::INVENTORY;
            } else if (nextState == 6) {
                state = GameState::SAVED_GAMES;
            } else if (nextState == 7) {
                state = GameState::LOGOUT;
            } else if (nextState == 8) {
                shouldExit = true;
            }
            break;
        }

        case GameState::GAME_START_MENU: {
            int choice = 0;
            bool backToMenu = false;

            startMenu.run(window, choice, backToMenu);

            if (backToMenu) {
                state = GameState::MAINMENU;
                mainMenu.resetMenu();
            } 
            else {
                if (choice == 1) difficulty = Difficulty::EASY;
                if (choice == 2) difficulty = Difficulty::MEDIUM;
                if (choice == 3) difficulty = Difficulty::HARD;

                if (choice == 4) state = GameState::MULTIPLAYER;
                if (choice == 5) state = GameState::GAME_ROOM;

                if (choice <= 3) {
                    state = GameState::GAME_SINGLE_PLAYER;
                }
            }
            break;
        }

        case GameState::GAME_SINGLE_PLAYER: {
            if (gameScreen) delete gameScreen;  
            gameScreen = new GameScreen(&manager, currentPlayer, &leaderboard, &difficulty, &themeInventory, window.getSize().x, window.getSize().y);
            bool backToMenu = false;
            gameScreen->run(window, backToMenu, &pauseScreen, &endScreen);
            if (backToMenu) {
                state = GameState::MAINMENU;
                mainMenu.resetMenu();
                manager.saveToFile("players_test.txt");
                leaderboard.saveToFile("leaderboard.txt");
            }
            break;
        }

        case GameState::MULTIPLAYER: {
            Player* tempPlayer1 = new Player("Player 1", "P1");
            Player* tempPlayer2 = new Player("Player 2", "P2");
            if(multiplayerEndGameScreen) delete multiplayerEndGameScreen;
            multiplayerEndGameScreen = new MultiplayerEndGameScreen(window.getSize().x, window.getSize().y, 
                                                                tempPlayer1->getUsername(), tempPlayer2->getUsername());
            
            tempPlayer1->setTotalPoints(0);
            tempPlayer2->setTotalPoints(0);
            
            if (multiplayerScreen) delete multiplayerScreen;
            Difficulty tmp = Difficulty::MEDIUM;
            multiplayerScreen = new MultiplayerGameScreen(&manager, tempPlayer1, tempPlayer2, &leaderboard, &tmp, &themeInventory,
                                                         window.getSize().x, window.getSize().y);
            
            bool backToMenu = false;
            multiplayerScreen->run(window, backToMenu, &multiplayerPauseScreen, multiplayerEndGameScreen);
            
            if (backToMenu) {
                state = GameState::MAINMENU;
                mainMenu.resetMenu();
            }

            delete tempPlayer1;
            delete tempPlayer2;

            break;
        }

        case GameState::GAME_ROOM: {
            bool continueInGameRoom = true;
            if(gameRoomScreen) delete gameRoomScreen;
                gameRoomScreen = new GameRoomScreen(&manager, &leaderboard, currentPlayer, 
                                                                window.getSize().x, window.getSize().y);
            
            while (continueInGameRoom && window.isOpen()) {
                
                
                bool backToMenu = false;
                Player* matchedP1 = nullptr;
                Player* matchedP2 = nullptr;
                bool matchFound = false;

                gameRoomScreen->run(window, backToMenu, matchedP1, matchedP2, matchFound);
                
                if (backToMenu) {
                    continueInGameRoom = false;
                    state = GameState::MAINMENU;
                    mainMenu.resetMenu();
                    break;
                }
                
                if (matchFound && matchedP1 && matchedP2) {  
                    if (multiplayerEndGameScreen) delete multiplayerEndGameScreen;
                    multiplayerEndGameScreen = new MultiplayerEndGameScreen(window.getSize().x, window.getSize().y, 
                                                                            matchedP1->getUsername(), matchedP2->getUsername());                 
                    if (multiplayerScreen) delete multiplayerScreen;
                    Difficulty tmp = Difficulty::MEDIUM;
                    multiplayerScreen = new MultiplayerGameScreen(&manager, matchedP1, matchedP2, 
                                                                &leaderboard, &tmp, &themeInventory,
                                                                window.getSize().x, window.getSize().y);

                    multiplayerEndGameScreen->setGameRoomMode(true);
                    
                    bool backFromGame = false;
                    multiplayerScreen->run(window, backFromGame, &multiplayerPauseScreen, multiplayerEndGameScreen);
                    
                    int endChoice = multiplayerEndGameScreen->getSelectedOption();
                    
                    if (endChoice == 1) {
                        continueInGameRoom = false;
                        state = GameState::MAINMENU;
                        mainMenu.resetMenu();
                    }

                    //gameRoomScreen->dequeueMatchedPlayers();
                    
                    if (multiplayerScreen) delete multiplayerScreen;
                    multiplayerScreen = nullptr;
    
                    multiplayerEndGameScreen->setGameRoomMode(false);

                    matchedP1 = nullptr;
                    matchedP2 = nullptr;
                    matchFound = false;
                }
            }
            break;
        }
        case GameState::LEADERBOARD: {
            bool backToMenu = false;
            leaderboardScreen.run(window, currentPlayer, backToMenu);
            if (backToMenu) {
                state = GameState::MAINMENU;
                mainMenu.resetMenu();
            }
            break;
        }
        
        case GameState::PROFILE: {
            bool backToMenu = false;
            profile.setCurrentPlayer(currentPlayer);
            profile.run(window, backToMenu);
            if (backToMenu) {
                state = GameState::MAINMENU;
                mainMenu.resetMenu();
            }
            break;
        }
        
        case GameState::FRIENDS: {
            bool backToMenu = false;
            friendsScreen.run(window, backToMenu, currentPlayer);
            if (backToMenu) {
                state = GameState::MAINMENU;
                mainMenu.resetMenu();
            }
            break;
        }
        
        case GameState::INVENTORY: {
            bool backToMenu = false;
            inventoryScreen.run(window, currentPlayer, backToMenu);
            if (backToMenu) {
                state = GameState::MAINMENU;
                mainMenu.resetMenu();
            }
            break;
        }

        case GameState::SAVED_GAMES: {
            SaveGame loadedSave;
            bool backToMenu = false;
            bool loadGame = false;
            savedGamesScreen.setCurrentPlayer(currentPlayer);
            savedGamesScreen.loadPlayerSaves();
            savedGamesScreen.run(window, backToMenu, loadGame, loadedSave);
            
            if (backToMenu) {
                state = GameState::MAINMENU;
                mainMenu.resetMenu();
            } 
            else if (loadGame) {
                if (gameScreen) delete gameScreen;
                gameScreen = new GameScreen(&manager, currentPlayer, &leaderboard, &difficulty, &themeInventory, window.getSize().x, window.getSize().y);
                gameScreen->loadGameFromSave(loadedSave);
                
                bool backFromGame = false;
                gameScreen->run(window, backFromGame, &pauseScreen, &endScreen);
                
                if (backFromGame) {
                    state = GameState::MAINMENU;
                    mainMenu.resetMenu();
                    manager.saveToFile("players_test.txt");
                    leaderboard.saveToFile("leaderboard.txt");
                }
            }
            break;
        }
        
        case GameState::LOGOUT: {
            loggedIn = false;
            currentPlayer = nullptr;
            state = GameState::LOGIN;
            login.resetErrorMsg();
            login.resetInputFields();
            manager.saveToFile("players_test.txt");
            leaderboard.saveToFile("leaderboard.txt");
            break;
        }
        
        case GameState::EXIT: {
            shouldExit = true;
            window.close();
            break;
        }
        
        } 
    } 
    if (gameScreen) delete gameScreen;
    if (multiplayerScreen) delete multiplayerScreen;
    if (multiplayerEndGameScreen) delete multiplayerEndGameScreen;
    if (gameRoomScreen) delete gameRoomScreen;
    return 0;
}