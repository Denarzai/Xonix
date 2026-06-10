#pragma once
using namespace std;
#include <SFML/Graphics.hpp>
#include "../core/playerManager.h"
#include "../core/auth.h"
#include "../ui/button.h"
#include "../ui/textBox.h"


class LoginScreen {
private:
    
    sf::Text usernamePlaceholder;
    sf::Text passwordPlaceholder;

    PlayerManager& manager;
    Auth& auth;

    sf::Font font;
    TextBox *usernameBox;
    TextBox *passwordBox;
    Button *loginBtn;
    Button *goRegisterBtn;
    string errorMsg;

    int windowWidth;
    int windowHeight;

public:
    LoginScreen(PlayerManager& m, Auth& a, int w, int h);
    void run(sf::RenderWindow& window, bool& goToRegister, bool& loggedIn, Player*& currentPlayer);
    void resetInputFields();
    void resetErrorMsg();
    ~LoginScreen();
};
