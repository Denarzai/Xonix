#ifndef MAINMENU_H
#define MAINMENU_H

#include <SFML/Graphics.hpp>
#include <string>
#include "../core/playerManager.h"
#include "../core/auth.h"

class MainMenu {
private:
    PlayerManager& manager;
    Auth& auth;
    Player* currentPlayer;
    
    sf::Font font;
    sf::Font titleFont;
    
    int windowWidth;
    int windowHeight;
    
    
    const static int MAX_OPTIONS = 8;
    std::string options[MAX_OPTIONS];
    int selectedOption;
    
    sf::Color normalColor;
    sf::Color selectedColor;
    sf::Color titleColor;
    
    float optionYStart;
    float optionSpacing;
    
public:
    MainMenu(PlayerManager& mgr, Auth& authSystem, int width, int height);
    ~MainMenu();
    
    void handleInput(sf::RenderWindow& window, bool& shouldExit, int& nextState);
    void update();
    void draw(sf::RenderWindow& window);
    
    
    int getSelectedOption() const;
    int getSelectedOptionId() const;
    void resetMenu();
    
private:
    void initializeOptions();
    void drawTitle(sf::RenderWindow& window);
    void drawOptions(sf::RenderWindow& window);
    void drawPlayerInfo(sf::RenderWindow& window);
};

#endif 