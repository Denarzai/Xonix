#include "../../include/screens/savedGamesScreen.h"
#include <iostream>
#include <ctime>

SavedGamesScreen::SavedGamesScreen(PlayerManager* pm, Player* player, int width, int height)
    : playerManager(pm), currentPlayer(player), windowWidth(width), windowHeight(height),
      selectedSaveIndex(0), totalSaves(0) {
    
    saveGameManager = new SaveGameManager();
    
    if (!font.loadFromFile("../data/Picopixel.ttf")) {
        cerr << "Error loading font!" << endl;
    }
    
    loadPlayerSaves();
}

SavedGamesScreen::~SavedGamesScreen() {
    if (saveGameManager) delete saveGameManager;
}

void SavedGamesScreen::loadPlayerSaves() {
    if (!currentPlayer) {
        totalSaves = 0;
        return;
    }
    
    
    totalSaves = saveGameManager->getSavesForPlayer(currentPlayer->getUsername(), saveIDs);
    selectedSaveIndex = 0;
}

void SavedGamesScreen::handleInput(RenderWindow& window, bool& backToMenu, bool& loadGame, SaveGame& selectedSave) {
    Event e;
    while (window.pollEvent(e)) {
        if (e.type == Event::KeyPressed) {
            if (e.key.code == Keyboard::Escape) {
                backToMenu = true;
            }
            else if (e.key.code == Keyboard::Up) {
                if (selectedSaveIndex > 0) {
                    selectedSaveIndex--;
                }
            }
            else if (e.key.code == Keyboard::Down) {
                if (selectedSaveIndex < totalSaves - 1) {
                    selectedSaveIndex++;
                }
            }
            else if (e.key.code == Keyboard::Return && totalSaves > 0) {
                
                if (saveGameManager->loadGame(saveIDs[selectedSaveIndex], selectedSave)) {
                    loadGame = true;
                }
            }
            else if (e.key.code == Keyboard::Delete && totalSaves > 0) {
                
                if (saveGameManager->deleteSave(saveIDs[selectedSaveIndex])) {
                    loadPlayerSaves();
                    if (selectedSaveIndex >= totalSaves && selectedSaveIndex > 0) {
                        selectedSaveIndex--;
                    }
                }
            }
        }
    }
}

void SavedGamesScreen::update() {
    
}

void SavedGamesScreen::draw(RenderWindow& window) {
    window.clear(Color::Black);
    
    
    Text title("SAVED GAMES", font, 50);
    title.setFillColor(Color::Cyan);
    title.setPosition((windowWidth - title.getLocalBounds().width) / 2.f, 30);
    window.draw(title);
    
    if (totalSaves == 0) {
        Text noSaves("No saved games found", font, 30);
        noSaves.setFillColor(Color::White);
        noSaves.setPosition(windowWidth / 2 - 150, windowHeight / 2 - 50);
        window.draw(noSaves);
    } else {
        
        for (int i = 0; i < totalSaves; i++) {
            Text saveText(saveIDs[i], font, 24);
            
            if (i == selectedSaveIndex) {
                saveText.setFillColor(Color::Yellow);
            } else {
                saveText.setFillColor(Color::White);
            }
            
            saveText.setPosition(50, 120 + i * 50);
            window.draw(saveText);
        }
    }
    
    sf::Text instructions("Use ARROW KEYS to select, ENTER to confirm, ESCAPE to go back", font, 16);
    instructions.setFillColor(sf::Color::White);
    float instrWidth = instructions.getLocalBounds().width;
    instructions.setPosition(windowWidth / 2 - instrWidth / 2, windowHeight - 50);
    window.draw(instructions);
    
    window.display();
}

void SavedGamesScreen::run(RenderWindow& window, bool& backToMenu, bool& loadGame, SaveGame& selectedSave) {
    backToMenu = false;
    loadGame = false;
    
    while (window.isOpen() && !backToMenu && !loadGame) {
        handleInput(window, backToMenu, loadGame, selectedSave);
        update();
        draw(window);
    }
}