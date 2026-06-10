#pragma once
#include <SFML/Graphics.hpp>
#include "../core/playerManager.h"
#include "../core/player.h"
#include "../core/saveGameManager.h"
#include "../core/saveGame.h"

using namespace sf;

class SavedGamesScreen {
private:
    int windowWidth;
    int windowHeight;
    Font font;
    
    PlayerManager* playerManager;
    Player* currentPlayer;
    SaveGameManager* saveGameManager;
    
    int selectedSaveIndex;
    int totalSaves;
    string saveIDs[10];
    
public:
    SavedGamesScreen(PlayerManager* pm, Player* player, int width, int height);
    ~SavedGamesScreen();
    
    void run(RenderWindow& window, bool& backToMenu, bool& loadGame, SaveGame& selectedSave);
    void handleInput(RenderWindow& window, bool& backToMenu, bool& loadGame, SaveGame& selectedSave);
    void update();
    void draw(RenderWindow& window);
    void setCurrentPlayer(Player* p) {currentPlayer = p;}
    void loadPlayerSaves();
    void displaySaveInfo(RenderWindow& window, int index);
};