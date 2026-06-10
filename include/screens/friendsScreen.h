#pragma once
#include <SFML/Graphics.hpp>
#include "../core/playerManager.h"
#include "../ui/textBox.h"
#include <string>

enum class ScreenState {
    MAIN_MENU,
    VIEW_FRIENDS,
    SEARCH_PLAYERS,
    VIEW_REQUESTS,
    PLAYER_PROFILE, 
    HANDLE_REQUEST,
    VIEW_FRIEND_PROFILE
};

enum class ProfileAction {
    SEND_REQUEST,
    CANCEL,
    DELETE_FRIEND
};

class FriendsScreen {
private:
    PlayerManager* manager;
    Player* currentPlayer;
    int windowWidth, windowHeight;

    ProfileAction selectedAction;
    
    ScreenState currentState;
    std::string searchInput;
    Player* searchResults[50];
    int searchResultCount;
    Player* selectedPlayer;
    int selectedResultIndex;
    int friendsCount;
    int requestsCount;
    
    sf::Font font;
    sf::Color normalColor;
    sf::Color selectedColor;
    sf::Color titleColor;
    TextBox* searchBox;

    std::string selectedRequestUser;
    std::string selectedFriendUser;

public:
    FriendsScreen(PlayerManager* mgr, Player* player, int w, int h);
    ~FriendsScreen();
    
    void run(sf::RenderWindow& window, bool& backToMenu, Player* player);
    
private:
    void handleInput(sf::Event& event, bool& backToMenu);
    void update();
    void draw(sf::RenderWindow& window);
    
    void drawMainMenu(sf::RenderWindow& window);
    void drawFriendsList(sf::RenderWindow& window);
    void drawSearchPlayers(sf::RenderWindow& window);
    void drawPendingRequests(sf::RenderWindow& window);
    void drawPlayerProfile(sf::RenderWindow& window);
    void drawHandleRequest(sf::RenderWindow& window);
    void drawFriendProfile(sf::RenderWindow& window);
};