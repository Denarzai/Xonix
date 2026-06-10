#pragma once
#include <SFML/Graphics.hpp>
#include "../core/matchmaking.h"
#include "../core/playerManager.h"
#include "../core/leaderboard.h"

class GameRoomScreen {
private:
    MatchmakingSystem matchmaking;
    PlayerManager* playerManager;
    Leaderboard* leaderboard;
    Player* currentPlayer;
    
    int windowWidth, windowHeight;
    sf::Font font;
    sf::Clock updateClock;
    
    bool playerAddedToQueue;
    float queueCheckTimer;
    
    bool queueInitialized;
    Player* matchedPlayer1;
    Player* matchedPlayer2;
    bool matchFound;
    
public:
    GameRoomScreen(PlayerManager* pm, Leaderboard* lb, Player* cp, int w, int h);
    ~GameRoomScreen();
    
    void handleInput(sf::RenderWindow& window, bool& backToMenu);
    void update();
    void draw(sf::RenderWindow& window);
    void drawQueueList(sf::RenderWindow& window);
    void drawWaitingMessage(sf::RenderWindow& window);
    
    void getMatchedPlayers(Player*& p1, Player*& p2) const;
    bool arePlayersMatched() const;
    void dequeueMatchedPlayers();
    
    void run(sf::RenderWindow& window, bool& backToMenu, 
             Player*& matchedP1, Player*& matchedP2, bool& matchFound);
};