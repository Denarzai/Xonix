#ifndef LEADERBOARDSCREEN_H
#define LEADERBOARDSCREEN_H

#include <SFML/Graphics.hpp>
#include "../core/playerManager.h"
#include "../core/leaderboard.h"

class LeaderboardScreen {
private:
    PlayerManager& manager;
    Player* currentPlayer;
    Leaderboard& leaderboard;
    sf::Font font;
    int windowWidth;
    int windowHeight;
    
public:
    LeaderboardScreen(PlayerManager& mgr, Leaderboard& lb,int width, int height);
    ~LeaderboardScreen();
    
    void run(sf::RenderWindow& window, Player* pl, bool& backToMenu);
    
private:
    void draw(sf::RenderWindow& window);
    void handleInput(sf::RenderWindow& window, bool& backToMenu);
};

#endif 