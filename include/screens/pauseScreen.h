#ifndef PAUSESCREEN_H
#define PAUSESCREEN_H

#include <SFML/Graphics.hpp>
#include <string>

class PauseScreen {
private:
    int windowWidth;
    int windowHeight;
    sf::Font font;
    int selectedOption;
    
    enum PauseOption {
        RESUME = 0,
        SAVE_GAME = 1,
        RETURN_TO_MENU = 2
    };

    bool savOptionEnabled = true; 
    
public:
    PauseScreen(int width, int height);
    ~PauseScreen();
    
    void handleInput(sf::RenderWindow& window, int& choice, bool& backToPause);
    void draw(sf::RenderWindow& window);
    void run(sf::RenderWindow& window, int& choice);
    void resetSelection();
    void disableSaveOption() { savOptionEnabled = false; }
    void enableSaveOption() { savOptionEnabled = true; }
};

#endif