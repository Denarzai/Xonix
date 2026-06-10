#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include "../core/player.h"

enum EndGameOption {
    RESTART = 0,
    RETURN_TO_MENU = 1
};
    

class MultiplayerEndGameScreen {
private:
    int windowWidth;
    int windowHeight;
    sf::Font font;
    sf::Texture gameoverTexture;
    int selectedOption;
    int player1Score;
    int player2Score;
    std::string player1Name;
    std::string player2Name;
    std::string winnerName;

    bool isGameRoomMode; 
    
    
public:
    MultiplayerEndGameScreen(int width, int height, const std::string& p1Name, const std::string& p2Name);
    ~MultiplayerEndGameScreen();
    
    void handleInput(sf::RenderWindow& window, int& choice, bool& backToEnd);
    void draw(sf::RenderWindow& window);
    void run(sf::RenderWindow& window, int& choice, int p1Score, int p2Score);
    void resetSelection();
    void setPlayer1Score(int n) { player1Score = n; }
    void setPlayer2Score(int n) { player2Score = n; }
    void setWinner(const std::string& name) { winnerName = name; }
    void setGameRoomMode(bool isGameRoom);
    int getSelectedOption() const {return selectedOption;}
};
