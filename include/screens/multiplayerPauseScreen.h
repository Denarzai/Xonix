#pragma once

#include <SFML/Graphics.hpp>

class MultiplayerPauseScreen {
private:
    int windowWidth;
    int windowHeight;
    int selectedOption;
    sf::Font font;
    bool isGameRoomMode; 

    void handleInput(sf::RenderWindow& window, int& choice, bool& backToPause);
    void draw(sf::RenderWindow& window);

public:
    MultiplayerPauseScreen(int width, int height);
    ~MultiplayerPauseScreen();

    void resetSelection();
    void run(sf::RenderWindow& window, int& choice);
};
