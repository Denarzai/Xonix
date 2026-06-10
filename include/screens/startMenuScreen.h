#ifndef STARTMENUSCREEN_H
#define STARTMENUSCREEN_H

#include <SFML/Graphics.hpp>

class StartMenuScreen {
private:
    sf::Font font;
    sf::Font titleFont;

    int windowWidth;
    int windowHeight;

    const static int OPTION_COUNT = 5;
    std::string options[OPTION_COUNT];
    int selectedOption;

    sf::Color normalColor;
    sf::Color selectedColor;

    float optionYStart;
    float optionSpacing;

public:
    StartMenuScreen(int width, int height);
    void run(sf::RenderWindow& window, int& choice, bool& backToMenu);

private:
    void draw(sf::RenderWindow& window);
    void drawTitle(sf::RenderWindow& window);
    void drawOptions(sf::RenderWindow& window);
};

#endif
