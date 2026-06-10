#pragma once
using namespace std;
#include <SFML/Graphics.hpp>
#include "../core/playerManager.h"
#include "../core/auth.h"
#include "../ui/button.h"
#include "../ui/textBox.h"

class RegisterScreen {
private:
    sf::Text usernamePlaceholder;
    sf::Text passwordPlaceholder;
    sf::Text nicknamePlaceholder;
    sf::Text emailPlaceholder;

    PlayerManager& manager;
    Auth& auth;

    sf::Font font;
    TextBox *usernameBox;
    TextBox *passwordBox;
    TextBox *nicknameBox;
    TextBox *emailBox;

    Button *registerBtn;
    Button *goLoginBtn;
    string errorMsg;
    string successMsg;

    int windowWidth;
    int windowHeight;

public:
    RegisterScreen(PlayerManager& m, Auth& a, int w, int h);
    ~RegisterScreen();
    void run(sf::RenderWindow& window, bool& goToLogin);
    void resetInputFields();
    void resetErrorMsg();
};
