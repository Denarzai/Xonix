#pragma once
#include <SFML/Graphics.hpp>
#include "../core/themeInventory.h"
#include "../core/playerManager.h"

class InventoryScreen {
private:
    ThemeInventory* themeInventory;
    PlayerManager* manager;
    float windowWidth;
    float windowHeight;

public:
    InventoryScreen(ThemeInventory* inventory, PlayerManager* playerManager, float width, float height);
    void run(sf::RenderWindow& window, Player* currentPlayer, bool& backToMenu);
};
