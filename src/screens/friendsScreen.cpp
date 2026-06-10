#include "../../include/screens/friendsScreen.h"
#include <iostream>

FriendsScreen::FriendsScreen(PlayerManager* mgr, Player* player, int w, int h)
    : manager(mgr), currentPlayer(player), windowWidth(w), windowHeight(h),
      currentState(ScreenState::MAIN_MENU), searchResultCount(0), selectedResultIndex(0), 
      selectedPlayer(nullptr), friendsCount(0), requestsCount(0), selectedRequestUser(""), selectedFriendUser("") {
    
    if (!font.loadFromFile("../data/Picopixel.ttf")) {
        std::cerr << "Error loading font for FriendsScreen!" << std::endl;
    }
    
    normalColor = sf::Color::White;
    selectedColor = sf::Color::Yellow;
    titleColor = sf::Color::Cyan;

    searchBox = new TextBox(200, 98, 300, 35, font);
}

FriendsScreen::~FriendsScreen() {
    delete searchBox;
}

void FriendsScreen::run(sf::RenderWindow& window, bool& backToMenu, Player* player) {
    currentPlayer = player;
    backToMenu = false;
    sf::Event event;
    
    while (window.isOpen()) {
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
                return;
            }
            
            
            
            handleInput(event, backToMenu);
        }
        
        update();
        draw(window);
        
        if (backToMenu) return;
    }
}

void FriendsScreen::handleInput(sf::Event& event, bool& backToMenu) {
    if (event.type != sf::Event::KeyPressed && event.type != sf::Event::TextEntered) return;
    
    switch (currentState) {
        case ScreenState::MAIN_MENU: {
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
                backToMenu = true;
                return;
            }

            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Up) {
                    selectedResultIndex--;
                    if (selectedResultIndex < 0) selectedResultIndex = 2;
                }
                else if (event.key.code == sf::Keyboard::Down) {
                    selectedResultIndex++;
                    if (selectedResultIndex > 2) selectedResultIndex = 0;
                }
                else if (event.key.code == sf::Keyboard::Return) {
                    if (selectedResultIndex == 0) {
                        currentState = ScreenState::VIEW_FRIENDS;
                        selectedResultIndex = 0;
                        friendsCount = currentPlayer->getFriendCount();
                    }
                    else if (selectedResultIndex == 1) {
                        currentState = ScreenState::SEARCH_PLAYERS;
                        searchBox->setText("");
                        searchBox->setSelected(true);
                        searchResultCount = 0;
                        selectedResultIndex = 0;
                    }
                    else if (selectedResultIndex == 2) {
                        currentState = ScreenState::VIEW_REQUESTS;
                        selectedResultIndex = 0;
                        requestsCount = currentPlayer->getRequestCount();
                    }
                }
            }
            break;
        }
        
        case ScreenState::VIEW_FRIENDS: {
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Escape) {
                    currentState = ScreenState::MAIN_MENU;
                    selectedResultIndex = 0;
                }
                else if (event.key.code == sf::Keyboard::Up) {
                    selectedResultIndex--;
                    if (selectedResultIndex < 0) selectedResultIndex = friendsCount - 1;
                }
                else if (event.key.code == sf::Keyboard::Down) {
                    selectedResultIndex++;
                    if (selectedResultIndex >= friendsCount) selectedResultIndex = 0;
                }
                else if (event.key.code == sf::Keyboard::Return) {
                    FriendNode* ptr = currentPlayer->getFriendsHead();
                    for (int i = 0; i < selectedResultIndex && ptr; i++) {
                        ptr = ptr->next;
                    }
                    if (ptr) {
                        selectedFriendUser = ptr->friendUsername;
                        currentState = ScreenState::VIEW_FRIEND_PROFILE;
                        selectedAction = ProfileAction::CANCEL; 
                        selectedPlayer = manager->getPlayerByUsername(selectedFriendUser); 
                    }
                }
            }
            break;
        }
        
        case ScreenState::SEARCH_PLAYERS: {
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Escape) {
                    currentState = ScreenState::MAIN_MENU;
                    selectedResultIndex = 0;
                    searchBox->setSelected(false);
                }
                else if (event.key.code == sf::Keyboard::Return) {
                    if (searchBox->isSelected()) {
                        std::string searchTerm = searchBox->getText();
                        if (!searchTerm.empty()) {
                            searchResultCount = manager->searchPlayers(searchTerm, searchResults, 50);
                            selectedResultIndex = 0;
                            if(searchResultCount > 0) searchBox->setSelected(false);
                        }
                    }
                    else if (searchResultCount > 0 && selectedResultIndex < searchResultCount) {
                        
                        selectedPlayer = searchResults[selectedResultIndex];
                        currentState = ScreenState::PLAYER_PROFILE;
                    }
                }
                else if (event.key.code == sf::Keyboard::Down) {
                    if (searchBox->isSelected()) {
                        if (searchResultCount > 0) {
                            searchBox->setSelected(false);
                            selectedResultIndex = 0;
                        }
                    }
                    else if (searchResultCount > 0) {
                        selectedResultIndex++;
                        if (selectedResultIndex >= searchResultCount) selectedResultIndex = 0;
                    }
                }
                else if (event.key.code == sf::Keyboard::Up) {
                    if (!searchBox->isSelected() && selectedResultIndex == 0 && searchResultCount > 0) {
                        searchBox->setSelected(true);
                    }
                    else if (!searchBox->isSelected() && searchResultCount > 0) {
                        selectedResultIndex--;
                        if (selectedResultIndex < 0) selectedResultIndex = searchResultCount - 1;
                    }
                }
                else {
                    searchBox->handleInput(event);
                }
            }
            else {
                searchBox->handleInput(event);
            }
            break;
        }
        
        case ScreenState::VIEW_REQUESTS: {
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Escape) {
                    currentState = ScreenState::MAIN_MENU;
                    selectedResultIndex = 0;
                }
                else if (event.key.code == sf::Keyboard::Up) {
                    selectedResultIndex--;
                    if (selectedResultIndex < 0) selectedResultIndex = requestsCount - 1;
                }
                else if (event.key.code == sf::Keyboard::Down) {
                    selectedResultIndex++;
                    if (selectedResultIndex >= requestsCount) selectedResultIndex = 0;
                }
                else if (event.key.code == sf::Keyboard::Return) {
                    RequestNode* ptr = currentPlayer->getRequestHead();
                    for (int i = 0; i < selectedResultIndex && ptr; i++) {
                        ptr = ptr->next;
                    }
                    if (ptr) {
                        selectedRequestUser = ptr->fromUser;
                        currentState = ScreenState::HANDLE_REQUEST;
                        selectedAction = ProfileAction::SEND_REQUEST;
                    }
                }
            }
            break;
        }

        case ScreenState::HANDLE_REQUEST: {
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Escape) {
                    currentState = ScreenState::VIEW_REQUESTS;
                    selectedRequestUser = "";
                }
                else if (event.key.code == sf::Keyboard::Left || event.key.code == sf::Keyboard::Right) {
                    selectedAction = (selectedAction == ProfileAction::SEND_REQUEST) ? ProfileAction::CANCEL : ProfileAction::SEND_REQUEST;
                }
                else if (event.key.code == sf::Keyboard::Return) {
                    if (selectedAction == ProfileAction::SEND_REQUEST) {
                        manager->acceptFriendRequest(currentPlayer->getUsername(), selectedRequestUser);
                        manager->saveToFile("players_test.txt");
                    }
                    else {
                        manager->rejectFriendRequest(currentPlayer->getUsername(), selectedRequestUser);
                        manager->saveToFile("players_test.txt");
                    }
                    currentState = ScreenState::VIEW_REQUESTS;
                    selectedRequestUser = "";
                    selectedResultIndex = 0;
                    selectedPlayer = nullptr;
                    requestsCount = currentPlayer->getRequestCount();
                }
            }
            if (selectedPlayer == nullptr) {
                selectedPlayer = manager->getPlayerByUsername(selectedRequestUser);
            }
            break;
        }
        
        case ScreenState::PLAYER_PROFILE: {
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Escape) {
                    currentState = ScreenState::SEARCH_PLAYERS;
                    selectedPlayer = nullptr;
                }
                else if (event.key.code == sf::Keyboard::Left || event.key.code == sf::Keyboard::Right) {
                    selectedAction = (selectedAction == ProfileAction::SEND_REQUEST) ? ProfileAction::CANCEL : ProfileAction::SEND_REQUEST;
                }
                else if (event.key.code == sf::Keyboard::Return) {
                    if (selectedAction == ProfileAction::SEND_REQUEST && selectedPlayer) {
                        manager->sendFriendRequest(currentPlayer->getUsername(), selectedPlayer->getUsername());
                        manager->saveToFile("players_test.txt"); 
                        currentState = ScreenState::SEARCH_PLAYERS;
                        selectedPlayer = nullptr;
                    }
                    else {
                        currentState = ScreenState::SEARCH_PLAYERS;
                        selectedPlayer = nullptr;
                    }
                }
            }
            break;
        }

        case ScreenState::VIEW_FRIEND_PROFILE: {
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Escape) {
                    currentState = ScreenState::VIEW_FRIENDS;
                    selectedFriendUser = "";
                    selectedPlayer = nullptr;
                }
                else if (event.key.code == sf::Keyboard::Left || event.key.code == sf::Keyboard::Right) {
                    selectedAction = (selectedAction == ProfileAction::DELETE_FRIEND) ? ProfileAction::CANCEL : ProfileAction::DELETE_FRIEND;
                }
                else if (event.key.code == sf::Keyboard::Return) {
                    if (selectedAction == ProfileAction::DELETE_FRIEND && selectedPlayer) {
                        manager->deleteFriend(currentPlayer->getUsername(), selectedPlayer->getUsername());
                        manager->saveToFile("players_test.txt"); 
                        currentState = ScreenState::VIEW_FRIENDS;
                        selectedPlayer = nullptr;
                    }
                    else {
                        currentState = ScreenState::VIEW_FRIENDS;
                        selectedPlayer = nullptr;
                    }
                }
            }
            break;
        }
    }
}

void FriendsScreen::update() {
}

void FriendsScreen::draw(sf::RenderWindow& window) {
    window.clear(sf::Color::Black);
    
    switch (currentState) {
        case ScreenState::MAIN_MENU:
            drawMainMenu(window);
            break;
        case ScreenState::VIEW_FRIENDS:
            drawFriendsList(window);
            break;
        case ScreenState::SEARCH_PLAYERS:
            drawSearchPlayers(window);
            break;
        case ScreenState::VIEW_REQUESTS:
            drawPendingRequests(window);
            break;
        case ScreenState::PLAYER_PROFILE:
            drawPlayerProfile(window);
            break;
        case ScreenState::HANDLE_REQUEST:
            drawHandleRequest(window);
            break;
        case ScreenState::VIEW_FRIEND_PROFILE:
            drawFriendProfile(window);
            break;
    }
    
    window.display();
}

void FriendsScreen::drawMainMenu(sf::RenderWindow& window) {
    sf::Text title("FRIENDS", font, 50);
    title.setFillColor(titleColor);
    title.setPosition((windowWidth - title.getLocalBounds().width) / 2.f, 30);
    window.draw(title);
    
    float optionYStart = (windowHeight - (3 * 28 + 2 * 50)) / 2.f;
    float optionSpacing = 50.f;
    float indicatorX = (windowWidth - 200) / 2.f;
    
    std::string options[3] = {"View Friends", "Search Players", "Pending Requests"};
    
    for (int i = 0; i < 3; i++) {
        sf::Text text(options[i], font, 28);
        text.setFillColor(i == selectedResultIndex ? selectedColor : normalColor);
        text.setStyle(i == selectedResultIndex ? sf::Text::Bold : sf::Text::Regular);
        float optionX = windowWidth / 2.0f - text.getLocalBounds().width / 2.0f;
        text.setPosition(optionX, optionYStart + i * optionSpacing);
        window.draw(text);
    }
    
    sf::Text arrow(">", font, 28);
    arrow.setFillColor(selectedColor);
    arrow.setPosition(indicatorX - 50, optionYStart + selectedResultIndex * optionSpacing);
    window.draw(arrow);
    
    sf::Text instructions("Use ARROW KEYS to select, ENTER to confirm, ESCAPE to go back", font, 16);
    instructions.setFillColor(sf::Color::White);
    float instrWidth = instructions.getLocalBounds().width;
    instructions.setPosition(windowWidth / 2 - instrWidth / 2, windowHeight - 50);
    window.draw(instructions);
}

void FriendsScreen::drawFriendsList(sf::RenderWindow& window) {
    sf::Text title("MY FRIENDS", font, 40);
    title.setFillColor(titleColor);
    title.setPosition(50, 30);
    window.draw(title);
    
    
    
    FriendNode* friends = currentPlayer->getFriendsHead();
    friendsCount = currentPlayer->getFriendCount();
    
    if (friendsCount == 0) {
        sf::Text noFriends("No friends yet!", font, 24);
        noFriends.setFillColor(sf::Color::White);
        noFriends.setPosition((windowWidth - noFriends.getLocalBounds().width) / 2.f, (windowHeight - noFriends.getLocalBounds().height) / 2.f);
        window.draw(noFriends);
    }
    else {
        FriendNode* ptr = friends;
        for (int i = 0; i < friendsCount; i++) {
            
            sf::Text friendText(ptr->friendUsername, font, 22);
            friendText.setFillColor(i == selectedResultIndex ? selectedColor : normalColor);
            friendText.setStyle(i == selectedResultIndex ? sf::Text::Bold : sf::Text::Regular);
            friendText.setPosition(100, 120 + i * 40);
            window.draw(friendText);
            ptr = ptr->next;
        }
        
        sf::Text arrow(">", font, 22);
        arrow.setFillColor(selectedColor);
        arrow.setPosition(50, 120 + selectedResultIndex * 40);
        window.draw(arrow);
    }
    
    sf::Text info("Friends: " + std::to_string(friendsCount) + "/50", font, 18);
    info.setFillColor(sf::Color::Cyan);
    info.setPosition(50, windowHeight - 50);
    window.draw(info);
    
    sf::Text instructions("Use ARROW KEYS to select, ENTER to confirm, ESCAPE to go back", font, 16);
    instructions.setFillColor(sf::Color::White);
    float instrWidth = instructions.getLocalBounds().width;
    instructions.setPosition(windowWidth / 2 - instrWidth / 2, windowHeight - 50);
    window.draw(instructions);
}

void FriendsScreen::drawSearchPlayers(sf::RenderWindow& window) {
    sf::Text title("SEARCH PLAYERS", font, 40);
    title.setFillColor(titleColor);
    title.setPosition(50, 30);
    window.draw(title);
    
    sf::Text label("Username:", font, 18);
    label.setFillColor(sf::Color::White);
    label.setPosition(100, 100);
    window.draw(label);
    
    searchBox->draw(window);
    
    sf::Text instruction("Press ENTER to search", font, 16);
    instruction.setFillColor(sf::Color::White);
    instruction.setPosition(200, 150);
    window.draw(instruction);
    
    if (searchResultCount > 0) {
        sf::Text resultsTitle("Results:", font, 22);
        resultsTitle.setFillColor(sf::Color::Cyan);
        resultsTitle.setPosition(100, 200);
        window.draw(resultsTitle);
        
        for (int i = 0; i < searchResultCount && i < 10; i++) {
            sf::Text result(searchResults[i]->getUsername(), font, 20);
            result.setFillColor(i == selectedResultIndex && !searchBox->isSelected() ? selectedColor : normalColor);
            result.setStyle(i == selectedResultIndex && !searchBox->isSelected() ? sf::Text::Bold : sf::Text::Regular);
            result.setPosition(120, 240 + i * 35);
            window.draw(result);
        }
        if(!searchBox->isSelected()){
            sf::Text arrow(">", font, 20);
            arrow.setFillColor(selectedColor);
            arrow.setPosition(80, 240 + selectedResultIndex * 35);
            window.draw(arrow);
        }
    }
    else if (!searchBox->isEmpty()) {
        sf::Text noResults("No results found", font, 20);
        noResults.setFillColor(sf::Color::White);
        noResults.setPosition(100, 250);
        window.draw(noResults);
    }
    
    sf::Text instructions("Use ARROW KEYS to select, ENTER to search/confirm, ESCAPE to go back", font, 16);
    instructions.setFillColor(sf::Color::White);
    float instrWidth = instructions.getLocalBounds().width;
    instructions.setPosition(windowWidth / 2 - instrWidth / 2, windowHeight - 50);
    window.draw(instructions);
}

void FriendsScreen::drawPendingRequests(sf::RenderWindow& window) {
    sf::Text title("PENDING REQUESTS", font, 40);
    title.setFillColor(titleColor);
    title.setPosition(50, 30);
    window.draw(title);
    
    RequestNode* requests = currentPlayer->getRequestHead();
    requestsCount = currentPlayer->getRequestCount();
    
    if (requestsCount == 0) {
        sf::Text noRequests("No pending requests!", font, 24);
        noRequests.setFillColor(sf::Color::White);
        noRequests.setPosition((windowWidth - noRequests.getLocalBounds().width) / 2.f, (windowHeight - noRequests.getLocalBounds().height) / 2.f);
        window.draw(noRequests);
    }
    else {
        RequestNode* ptr = requests;
        for (int i = 0; i < requestsCount; i++) {
            sf::Text request(ptr->fromUser, font, 22);
            request.setFillColor(i == selectedResultIndex ? selectedColor : normalColor);
            request.setStyle(i == selectedResultIndex ? sf::Text::Bold : sf::Text::Regular);
            request.setPosition(100, 120 + i * 40);
            window.draw(request);
            ptr = ptr->next;
        }
        
        sf::Text arrow(">", font, 22);
        arrow.setFillColor(selectedColor);
        arrow.setPosition(50, 120 + selectedResultIndex * 40);
        window.draw(arrow);
    }
    
    sf::Text info("Requests: " + std::to_string(requestsCount) + "/20", font, 18);
    info.setFillColor(sf::Color::Cyan);
    info.setPosition(50, windowHeight - 50);
    window.draw(info);
    
    sf::Text instructions("Press ENTER to view request, ESCAPE to go back", font, 16);
    instructions.setFillColor(sf::Color::White);
    float instrWidth = instructions.getLocalBounds().width;
    instructions.setPosition(windowWidth / 2 - instrWidth / 2, windowHeight - 50);
    window.draw(instructions);
}

void FriendsScreen::drawHandleRequest(sf::RenderWindow& window) {
    sf::Text titleText("FRIEND REQUEST", font, 50);
    titleText.setFillColor(sf::Color::Cyan);
    titleText.setPosition((windowWidth - titleText.getLocalBounds().width) / 2.0f, 30);
    window.draw(titleText);
    
    sf::Text fromText("From: " + selectedRequestUser, font, 24);
    fromText.setFillColor(sf::Color::White);
    fromText.setPosition(150, 120);
    window.draw(fromText);
    
    sf::Text info1("Points: " + std::to_string(selectedPlayer->getTotalPoints()), font, 22);
    info1.setFillColor(sf::Color::White);
    info1.setPosition(150, 170);
    window.draw(info1);
    
    sf::Text info2("Wins: " + std::to_string(selectedPlayer->getWins()), font, 22);
    info2.setFillColor(sf::Color::White);
    info2.setPosition(150, 220);
    window.draw(info2);
    
    sf::Text info3("Losses: " + std::to_string(selectedPlayer->getLosses()), font, 22);
    info3.setFillColor(sf::Color::White);
    info3.setPosition(150, 270);
    window.draw(info3);
    
    sf::Text acceptBtn("Accept", font, 22);
    acceptBtn.setFillColor(selectedAction == ProfileAction::SEND_REQUEST ? selectedColor : normalColor);
    acceptBtn.setStyle(selectedAction == ProfileAction::SEND_REQUEST ? sf::Text::Bold : sf::Text::Regular);
    acceptBtn.setPosition(150, 500);
    window.draw(acceptBtn);
    
    sf::Text rejectBtn("Reject", font, 22);
    rejectBtn.setFillColor(selectedAction == ProfileAction::CANCEL ? selectedColor : normalColor);
    rejectBtn.setStyle(selectedAction == ProfileAction::CANCEL ? sf::Text::Bold : sf::Text::Regular);
    rejectBtn.setPosition(400, 500);
    window.draw(rejectBtn);
    
    sf::Text instructions("Use LEFT/RIGHT to select, ENTER to confirm, ESCAPE to go back", font, 16);
    instructions.setFillColor(sf::Color::White);
    float instrWidth = instructions.getLocalBounds().width;
    instructions.setPosition(windowWidth / 2 - instrWidth / 2, windowHeight - 50);
    window.draw(instructions);
}

void FriendsScreen::drawPlayerProfile(sf::RenderWindow& window) {
    if (!selectedPlayer) return;

    sf::Text titleText("PLAYER PROFILE", font, 50);
    titleText.setFillColor(sf::Color::Cyan);
    titleText.setPosition((windowWidth - titleText.getLocalBounds().width) / 2.0f, 30);
    window.draw(titleText);
    
    sf::Text usernameText("Username: " + selectedPlayer->getUsername(), font, 24);
    usernameText.setFillColor(sf::Color::White);
    usernameText.setPosition(150, 120);
    window.draw(usernameText);
    
    sf::Text info1("Points: " + std::to_string(selectedPlayer->getTotalPoints()), font, 22);
    info1.setFillColor(sf::Color::White);
    info1.setPosition(150, 170);
    window.draw(info1);
    
    sf::Text info2("Wins: " + std::to_string(selectedPlayer->getWins()), font, 22);
    info2.setFillColor(sf::Color::White);
    info2.setPosition(150, 220);
    window.draw(info2);
    
    sf::Text info3("Losses: " + std::to_string(selectedPlayer->getLosses()), font, 22);
    info3.setFillColor(sf::Color::White);
    info3.setPosition(150, 270);
    window.draw(info3);

    sf::Text sendBtn("Send Request", font, 22);
    sendBtn.setFillColor(selectedAction == ProfileAction::SEND_REQUEST ? selectedColor : normalColor);
    sendBtn.setStyle(selectedAction == ProfileAction::SEND_REQUEST ? sf::Text::Bold : sf::Text::Regular);
    sendBtn.setPosition(150, 500);
    window.draw(sendBtn);
    
    sf::Text cancelBtn("Cancel", font, 22);
    cancelBtn.setFillColor(selectedAction == ProfileAction::CANCEL ? selectedColor : normalColor);
    cancelBtn.setStyle(selectedAction == ProfileAction::CANCEL ? sf::Text::Bold : sf::Text::Regular);
    cancelBtn.setPosition(400, 500);
    window.draw(cancelBtn);
    
    sf::Text instructions("Use LEFT/RIGHT to select, ENTER to confirm, ESCAPE to go back", font, 16);
    instructions.setFillColor(sf::Color::White);
    float instrWidth = instructions.getLocalBounds().width;
    instructions.setPosition(windowWidth / 2 - instrWidth / 2, windowHeight - 50);
    window.draw(instructions);
}

void FriendsScreen::drawFriendProfile(sf::RenderWindow& window) {
    if (!selectedPlayer) return;

    sf::Text titleText("PLAYER PROFILE", font, 50);
    titleText.setFillColor(sf::Color::Cyan);
    titleText.setPosition((windowWidth - titleText.getLocalBounds().width) / 2.0f, 30);
    window.draw(titleText);
    
    sf::Text usernameText("Username: " + selectedPlayer->getUsername(), font, 24);
    usernameText.setFillColor(sf::Color::White);
    usernameText.setPosition(150, 120);
    window.draw(usernameText);
    
    sf::Text info1("Points: " + std::to_string(selectedPlayer->getTotalPoints()), font, 22);
    info1.setFillColor(sf::Color::White);
    info1.setPosition(150, 170);
    window.draw(info1);
    
    sf::Text info2("Wins: " + std::to_string(selectedPlayer->getWins()), font, 22);
    info2.setFillColor(sf::Color::White);
    info2.setPosition(150, 220);
    window.draw(info2);
    
    sf::Text info3("Losses: " + std::to_string(selectedPlayer->getLosses()), font, 22);
    info3.setFillColor(sf::Color::White);
    info3.setPosition(150, 270);
    window.draw(info3);

    sf::Text sendBtn("Go Back", font, 22);
    sendBtn.setFillColor(selectedAction == ProfileAction::CANCEL ? selectedColor : normalColor);
    sendBtn.setStyle(selectedAction == ProfileAction::CANCEL ? sf::Text::Bold : sf::Text::Regular);
    sendBtn.setPosition(150, 500);
    window.draw(sendBtn);
    
    sf::Text cancelBtn("Delete Friend", font, 22);
    cancelBtn.setFillColor(selectedAction == ProfileAction::DELETE_FRIEND ? selectedColor : normalColor);
    cancelBtn.setStyle(selectedAction == ProfileAction::DELETE_FRIEND ? sf::Text::Bold : sf::Text::Regular);
    cancelBtn.setPosition(400, 500);
    window.draw(cancelBtn);
    
    sf::Text instructions("Use LEFT/RIGHT to select, ENTER to confirm, ESCAPE to go back", font, 16);
    instructions.setFillColor(sf::Color::White);
    float instrWidth = instructions.getLocalBounds().width;
    instructions.setPosition(windowWidth / 2 - instrWidth / 2, windowHeight - 50);
    window.draw(instructions);
}