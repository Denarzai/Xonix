#ifndef PROFILESCREEN_H
#define PROFILESCREEN_H

#include <SFML/Graphics.hpp>
#include "../core/player.h"

class ProfileScreen {
private:
    Player* currentPlayer;
    sf::Font font;
    int windowWidth;
    int windowHeight;
    
public:
    ProfileScreen(Player* player, int width, int height);
    ~ProfileScreen();
    
    void setCurrentPlayer(Player* p) {currentPlayer = p;}
    void run(sf::RenderWindow& window, bool& backToMenu);
    
private:
    void draw(sf::RenderWindow& window);
    void handleInput(sf::RenderWindow& window, bool& backToMenu);
};

#endif 