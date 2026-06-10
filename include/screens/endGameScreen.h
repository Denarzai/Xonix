#ifndef ENDGAMESCREEN_H
#define ENDGAMESCREEN_H

#include <SFML/Graphics.hpp>
#include <string>
#include "../core/player.h"


class EndGameScreen {
private:
    int windowWidth;
    int windowHeight;
    sf::Font font;
    sf::Texture gameoverTexture;
    int selectedOption;
    int finalScore;
    std::string playerName;
    
    enum EndGameOption {
        RESTART = 0,
        RETURN_TO_MENU = 1
    };
    
public:
    EndGameScreen(int width, int height, const std::string& name);
    ~EndGameScreen();
    
    void handleInput(sf::RenderWindow& window, int& choice, bool& backToEnd);
    void draw(sf::RenderWindow& window, bool newHighscore);
    void run(sf::RenderWindow& window, int& choice, bool newHighScore);
    void resetSelection();
    void setFinalScore(int n) {finalScore = n;}
};

#endif