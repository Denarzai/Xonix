#include "../../include/screens/loginScreen.h"

LoginScreen::LoginScreen(PlayerManager& m, Auth& a, int w, int h)
    : manager(m), auth(a), windowWidth(w), windowHeight(h)
{
    font.loadFromFile("../data/Picopixel.ttf");

    float elementWidth = 250;    
    float x = (windowWidth - elementWidth) / 2;

    float totalElementsHeight = 40 + 40 + 50 + 50;
    float spacing = 20; 
    float y = (windowHeight - totalElementsHeight + 30) / 2;

    usernameBox = new TextBox(x, y, 250, 40, font);
    y += 40 + spacing;  

    passwordBox = new TextBox(x, y, 250, 40, font);
    y += 40 + spacing;

    loginBtn = new Button(x, y, 250, 50, "Login", font);
    y += 50 + spacing;

    goRegisterBtn = new Button(x, y, 250, 50, "Go to Register", font);

    usernamePlaceholder.setFont(font);
    usernamePlaceholder.setString("Enter username");
    usernamePlaceholder.setCharacterSize(16);
    usernamePlaceholder.setFillColor(sf::Color(150, 150, 150));
    usernamePlaceholder.setPosition(usernameBox->getPosition().x + 5, usernameBox->getPosition().y + 9);

    passwordPlaceholder.setFont(font);
    passwordPlaceholder.setString("Enter password");
    passwordPlaceholder.setCharacterSize(16);
    passwordPlaceholder.setFillColor(sf::Color(150, 150, 150));
    passwordPlaceholder.setPosition(passwordBox->getPosition().x + 5, passwordBox->getPosition().y + 9);
}

LoginScreen::~LoginScreen() {
    delete usernameBox;
    delete passwordBox;
    delete loginBtn;
    delete goRegisterBtn;

    usernameBox = nullptr;
    passwordBox = nullptr;
    loginBtn = nullptr;
    goRegisterBtn = nullptr;
}


void LoginScreen::run(sf::RenderWindow& window, bool& goToRegister, bool& loggedIn, Player*& currentPlayer)
{
    sf::Event event;

    while (window.pollEvent(event)) 
    {
        if (event.type == sf::Event::Closed)
            window.close();

        if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
        {
            sf::Vector2f mp = window.mapPixelToCoords(sf::Mouse::getPosition(window));

            usernameBox->setSelected(usernameBox->isHovered(mp));
            passwordBox->setSelected(passwordBox->isHovered(mp));
        }

        usernameBox->handleInput(event);
        passwordBox->handleInput(event);
        sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
        if ((event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)){
            if (loginBtn->isHovered(mousePos))
            {
                std::string u = usernameBox->getText();
                std::string p = passwordBox->getText();

                Player* logged = auth.loginUser(u, p); 

                if (logged) {
                    loggedIn = true;
                    currentPlayer = logged;
                    return;   
                }
                else 
                {
                    errorMsg = "Invalid username or password";
                }
            }

            if (goRegisterBtn->isHovered(mousePos))
            {
                goToRegister = true;
                return; 
            }
        }
    }

    window.clear(sf::Color::Black);

    sf::Text titleText("LOGIN", font, 110);
    titleText.setFillColor(sf::Color::White);
    float titleX = (windowWidth - titleText.getLocalBounds().width) / 2.0f;
    titleText.setPosition(titleX, 110);
    window.draw(titleText);

    usernameBox->draw(window);
    passwordBox->draw(window);
    loginBtn->draw(window);
    goRegisterBtn->draw(window);

    if (usernameBox->getText().empty()) window.draw(usernamePlaceholder);
    if (passwordBox->getText().empty()) window.draw(passwordPlaceholder);

    sf::Text err(errorMsg, font, 20);
    err.setFillColor(sf::Color::Red);
    err.setPosition((windowWidth - err.getLocalBounds().width) / 2, 500);
    window.draw(err);

    window.display();
}

void LoginScreen::resetInputFields() {
    usernameBox->setText("");
    passwordBox->setText("");
}

void LoginScreen::resetErrorMsg() {
    errorMsg = "";
}
