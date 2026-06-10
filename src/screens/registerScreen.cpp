#include "../../include/screens/registerScreen.h"

RegisterScreen::RegisterScreen(PlayerManager& m, Auth& a, int w, int h)
    : manager(m), auth(a), windowWidth(w), windowHeight(h)
{
    font.loadFromFile("../data/Picopixel.ttf");

    float elementWidth = 250;    
    float x = (windowWidth - elementWidth) / 2;

    float totalElementsHeight = 40 + 40 + 40 + 40 + 50 + 50;
    float spacing = 20; 
    float y = (windowHeight - totalElementsHeight) / 2;

    usernameBox = new TextBox(x, y, 250, 40, font);
    y += spacing + 40;
    passwordBox = new TextBox(x, y, 250, 40, font);
    y += spacing + 40;
    nicknameBox = new TextBox(x, y, 250, 40, font);
    y += spacing + 40;
    emailBox = new TextBox(x, y, 250, 40, font);
    y += spacing + 50;

    registerBtn = new Button(x, y, 250, 50, "Register", font);
    y += spacing + 50;
    goLoginBtn = new Button(x, y, 250, 50, "Go to Login", font);

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

    nicknamePlaceholder.setFont(font);
    nicknamePlaceholder.setString("Enter nickname");
    nicknamePlaceholder.setCharacterSize(16);
    nicknamePlaceholder.setFillColor(sf::Color(150, 150, 150));
    nicknamePlaceholder.setPosition(nicknameBox->getPosition().x + 5, nicknameBox->getPosition().y + 9);

    emailPlaceholder.setFont(font);
    emailPlaceholder.setString("Enter email");
    emailPlaceholder.setCharacterSize(16);
    emailPlaceholder.setFillColor(sf::Color(150, 150, 150));
    emailPlaceholder.setPosition(emailBox->getPosition().x + 5, emailBox->getPosition().y + 9);
}

RegisterScreen::~RegisterScreen() {
    delete usernameBox;
    delete passwordBox;
    delete nicknameBox;
    delete emailBox;
    delete registerBtn;
    delete goLoginBtn;

    usernameBox = nullptr;
    passwordBox = nullptr;
    nicknameBox = nullptr;
    emailBox = nullptr;
    registerBtn = nullptr;
    goLoginBtn = nullptr;
}

void RegisterScreen::run(sf::RenderWindow& window, bool& goToLogin)
{
    goToLogin = false;

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
            {
                usernameBox->setSelected(usernameBox->isHovered(mousePos));
                passwordBox->setSelected(passwordBox->isHovered(mousePos));
                nicknameBox->setSelected(nicknameBox->isHovered(mousePos));
                emailBox->setSelected(emailBox->isHovered(mousePos));

                if (registerBtn->isHovered(mousePos))
                {
                    string u = usernameBox->getText();
                    string p = passwordBox->getText();
                    string n = nicknameBox->getText();
                    string e = emailBox->getText();

                    Player* registered = auth.registerUser(u, p, n, e);

                    if (registered) {
                        successMsg = "Registration successful!";
                        errorMsg = "";
                        goToLogin = true;
                        return;
                    }
                    else {
                        errorMsg = "Username taken or invalid!";
                        successMsg = "";
                    }
                }

                if (goLoginBtn->isHovered(mousePos))
                {
                    goToLogin = true;
                    return;
                }
            }

            usernameBox->handleInput(event);
            passwordBox->handleInput(event);
            nicknameBox->handleInput(event);
            emailBox->handleInput(event);
        }

        window.clear(sf::Color::Black);

        sf::Text titleText("REGISTER", font, 110);
        titleText.setFillColor(sf::Color::White);
        float titleX = (windowWidth - titleText.getLocalBounds().width) / 2.0f;
        titleText.setPosition(titleX, 50);
        window.draw(titleText);


        usernameBox->draw(window);
        passwordBox->draw(window);
        nicknameBox->draw(window);
        emailBox->draw(window);

        registerBtn->draw(window);
        goLoginBtn->draw(window);

        if (usernameBox->getText().empty()) window.draw(usernamePlaceholder);
        if (passwordBox->getText().empty()) window.draw(passwordPlaceholder);
        if (nicknameBox->getText().empty()) window.draw(nicknamePlaceholder);
        if (emailBox->getText().empty()) window.draw(emailPlaceholder);

        sf::Text err(errorMsg, font, 20);
        err.setFillColor(sf::Color::Red);
        err.setPosition((windowWidth - err.getLocalBounds().width) / 2, 570);
        window.draw(err);

        sf::Text succ(successMsg, font, 20);
        succ.setFillColor(sf::Color::Green);
        succ.setPosition((windowWidth - succ.getLocalBounds().width) / 2, 570);
        window.draw(succ);

        window.display();
    }
}

void RegisterScreen::resetInputFields() {
    usernameBox->setText("");
    passwordBox->setText("");
    nicknameBox->setText("");
    emailBox->setText("");
}

void RegisterScreen::resetErrorMsg() {
    errorMsg = "";
    successMsg = "";
}
