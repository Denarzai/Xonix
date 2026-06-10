#include "../../include/core/player.h"
#include <iostream>

Player::Player() {
    username = "";
    password = "";
    nickname = "";
    email = "";
    playerID = -1;
    registrationTimestamp = 0;

    totalPoints = 0;
    wins = 0;
    losses = 0;
    highScore = 0;

    bonusMultiplierCount = 0;
    tilesInCurrentMove = 0;
    currentMultiplier = 1;
    lastPowerUpThreshold = 0;
    tilesCapturedInCurrentMove = 0;

    currentThemeID = 0;
    powerUpCount = 0;
    powerUpActive = false;
    powerUpTimeRemaining = 0.0f;

    friendsHead = nullptr;
    requestHead = nullptr;

    friendCount = 0;
    requestCount = 0;
}

Player::Player(const string& user, const string& pass) {
    username = user;
    password = pass;
    nickname = "";
    email = "";
    playerID = -1;

    totalPoints = 0;
    wins = 0;
    losses = 0;
    highScore = 0;

    bonusMultiplierCount = 0;
    tilesInCurrentMove = 0;
    currentMultiplier = 1;
    lastPowerUpThreshold = 0;
    tilesCapturedInCurrentMove = 0;

    currentThemeID = 0;
    powerUpCount = 0;
    powerUpActive = false;
    powerUpTimeRemaining = 0.0f;

    friendsHead = nullptr;
    requestHead = nullptr;

    friendCount = 0;
    requestCount = 0;
}

Player::Player(const Player& other) {
    username = other.username;
    password = other.password;
    nickname = other.nickname;
    email = other.email;
    playerID = other.playerID;

    totalPoints = other.totalPoints;
    wins = other.wins;
    losses = other.losses;
    highScore = other.highScore;
    bonusMultiplierCount = other.bonusMultiplierCount;

    currentThemeID = other.currentThemeID;
    powerUpCount = other.powerUpCount;
    powerUpActive = other.powerUpActive;
    powerUpTimeRemaining = other.powerUpTimeRemaining;

    
    requestHead = nullptr;
    friendsHead = nullptr;
    requestCount = 0;
    friendCount = 0;

    RequestNode* currReq = other.requestHead;
    RequestNode* lastReq = nullptr;
    while (currReq) {
        RequestNode* newNode = new RequestNode(currReq->fromUser);

        if (!requestHead) requestHead = newNode;
        else lastReq->next = newNode;

        lastReq = newNode;
        currReq = currReq->next;
        requestCount++;
    }
  
    FriendNode* currFr = other.friendsHead;
    FriendNode* lastFr = nullptr;
    while (currFr) {
        FriendNode* newNode = new FriendNode(currFr->friendUsername);

        if (!friendsHead) friendsHead = newNode;
        else lastFr->next = newNode;

        lastFr = newNode;
        currFr = currFr->next;
        friendCount++;
    }
}





Player& Player::operator=(const Player& other)
{
    if (this == &other)
        return *this;

    
    RequestNode* r = requestHead;
    while (r) {
        RequestNode* temp = r;
        r = r->next;
        delete temp;
    }

    FriendNode* f = friendsHead;
    while (f) {
        FriendNode* temp = f;
        f = f->next;
        delete temp;
    }

    
    username = other.username;
    password = other.password;
    nickname = other.nickname;
    email = other.email;
    playerID = other.playerID;

    totalPoints = other.totalPoints;
    wins = other.wins;
    losses = other.losses;
    highScore = other.highScore;
    bonusMultiplierCount = other.bonusMultiplierCount;

    currentThemeID = other.currentThemeID;
    powerUpCount = other.powerUpCount;
    powerUpActive = other.powerUpActive;
    powerUpTimeRemaining = other.powerUpTimeRemaining;

    
    requestHead = nullptr;
    friendsHead = nullptr;
    requestCount = 0;
    friendCount = 0;

    
    RequestNode* currReq = other.requestHead;
    RequestNode* lastReq = nullptr;
    while (currReq) {
        RequestNode* newNode = new RequestNode(currReq->fromUser);

        if (!requestHead) requestHead = newNode;
        else lastReq->next = newNode;

        lastReq = newNode;
        currReq = currReq->next;
        requestCount++;
    }

    
    FriendNode* currFr = other.friendsHead;
    FriendNode* lastFr = nullptr;
    while (currFr) {
        FriendNode* newNode = new FriendNode(currFr->friendUsername);

        if (!friendsHead) friendsHead = newNode;
        else lastFr->next = newNode;

        lastFr = newNode;
        currFr = currFr->next;
        friendCount++;
    }

    return *this;
}


Player::~Player() {
    FriendNode* f = friendsHead;
    while (f != nullptr) {
        FriendNode* temp = f;
        f = f->next;
        delete temp;
    }

    RequestNode* r = requestHead;
    while (r != nullptr) {
        RequestNode* temp = r;
        r = r->next;
        delete temp;
    }
}

void Player::setNickname(const string& n) { nickname = n; }
void Player::setEmail(const string& e) { email = e; }
void Player::setPlayerID(int id) { playerID = id; }
void Player::setRegistrationTimestamp(time_t t) { registrationTimestamp = t; }
void Player::setThemeID(int id) { currentThemeID = id; }

string Player::getUsername() const { return username; }
string Player::getPassword() const { return password; }
string Player::getNickname() const { return nickname; }
string Player::getEmail() const { return email; }
int Player::getPlayerID() const { return playerID; }
int Player::getThemeID() const { return currentThemeID; }
int Player::getTotalPoints() const { return totalPoints; }
int Player::getWins() const { return wins; }
int Player::getLosses() const { return losses; }
int Player::getPowerUps() const { return powerUpCount; }
time_t Player::getRegistrationTimestamp() const { return registrationTimestamp; }

void Player::addPoints(int p) { totalPoints += p; }
void Player::addWin() { wins++; }
void Player::addLoss() { losses++; }
void Player::setHighScore(int n) { highScore = n; }
int Player::getHighScore() { return highScore; }

void Player::setPowerUps(int s) { powerUpCount = s; }

bool Player::addFriend(const string& friendName) {
    if (isFriend(friendName)) {
        return false;
    }

    if (friendCount >= MAX_FRIENDS) {
        return false;
    }
    
    FriendNode* node = new FriendNode(friendName);
    node->next = friendsHead;
    friendsHead = node;
    friendCount++;
    return true;
}

bool Player::isFriend(const string& friendName) {
    FriendNode* curr = friendsHead;
    while (curr) {
        if (curr->friendUsername == friendName)
            return true;
        curr = curr->next;
    }
    return false;
}

void Player::displayFriends() const {
    FriendNode* curr = friendsHead;
    cout << "Friends of " << username << ":\n";
    while (curr) {
        cout << " - " << curr->friendUsername << "\n";
        curr = curr->next;
    }
}

bool Player::addFriendRequest(const string& fromUser) {
    
    if (hasRequestFrom(fromUser)) {
        return false;
    }
    
    
    if (requestCount >= MAX_REQUESTS) {
        return false;
    }
    
    RequestNode* node = new RequestNode(fromUser);
    node->next = requestHead;
    requestHead = node;
    requestCount++;
    return true;
}

bool Player::hasRequestFrom(const string& fromUser) {
    RequestNode* curr = requestHead;
    while (curr) {
        if (curr->fromUser == fromUser)
            return true;
        curr = curr->next;
    }
    return false;
}

void Player::displayRequests() const {
    RequestNode* curr = requestHead;
    cout << "Pending friend requests:\n";
    while (curr) {
        cout << " - " << curr->fromUser << "\n";
        curr = curr->next;
    }
}

bool Player::isFriendLimitReached() const {
    return friendCount >= MAX_FRIENDS;
}

bool Player::isRequestLimitReached() const {
    return requestCount >= MAX_REQUESTS;
}

void Player::resetMoveCounter() {
    currentMultiplier = 1;
}

void Player::updateMultiplier() {
    int threshold = (bonusMultiplierCount >= 3) ? 5 : 10;
    
    if (tilesCapturedInCurrentMove > threshold) {
        if (bonusMultiplierCount > 5) {
            currentMultiplier = 4;
        } 
        else {
            currentMultiplier = 2;
        }
    } 
    else {
        currentMultiplier = 1;
    }
}

int Player::calculatePoints(int basePoints) {
    
    int points = basePoints * currentMultiplier;
    return points;
}

void Player::awardPoints(int points, int pointsEarnedThisGame) {
    totalPoints += points;
    checkAndAwardPowerUp(pointsEarnedThisGame);
}

void Player::checkAndAwardPowerUp(int pointsEarnedThisGame) {
    int thresholds[] = {50, 70, 100, 130};

    for (int i = 0; i < 4; i++) {
        if (pointsEarnedThisGame >= thresholds[i] && lastPowerUpThreshold < thresholds[i]) {
            powerUpCount++;
            lastPowerUpThreshold = thresholds[i];
        }
    }

    if (pointsEarnedThisGame > 130) {
        int pointsAbove130 = pointsEarnedThisGame - 130;
        int extraPowerUps = pointsAbove130 / 30;
        int lastExtraThreshold = lastPowerUpThreshold > 130 ? lastPowerUpThreshold : 130;

        while (extraPowerUps > 0 && lastPowerUpThreshold < pointsEarnedThisGame) {
            lastPowerUpThreshold += 30;
            if (lastPowerUpThreshold <= pointsEarnedThisGame) {
                powerUpCount++;
            }
            extraPowerUps--;
        }
    }
}

int Player::getTilesInCurrentMove() const {
    return tilesInCurrentMove;
}

int Player::getCurrentMultiplier() const {
    return currentMultiplier;
}

int Player::geTilesCapturedInCurrentMove() const {
    return tilesCapturedInCurrentMove;
};


void Player::usePowerUp() {
    if (powerUpCount > 0) {
        powerUpCount--;
        powerUpActive = true;
        powerUpTimeRemaining = 3.0f;  
    } 
}

void Player::deactivatePowerUp() {
    powerUpActive = false;
    powerUpTimeRemaining = 0.0f;
}

void Player::updatePowerUpTimer(float deltaTime) {
    if (powerUpActive) {
        powerUpTimeRemaining -= deltaTime;
        if (powerUpTimeRemaining <= 0.0f) {
            deactivatePowerUp();
        }
    }
}

bool Player::isPowerUpActive() const {
    return powerUpActive;
}

float Player::getPowerUpTimeRemaining() const {
    return powerUpTimeRemaining;
}

void Player::removeFriendRequest(const string &fromUser)
{
    if (!requestHead)
        return;
    if (requestHead->fromUser == fromUser)
    {
        RequestNode *temp = requestHead;
        requestHead = requestHead->next;
        delete temp;
        requestCount--;
        return;
    }
    RequestNode *curr = requestHead;
    while (curr->next)
    {
        if (curr->next->fromUser == fromUser)
        {
            RequestNode *temp = curr->next;
            curr->next = temp->next;
            delete temp;
            requestCount--;
            return;
        }
        curr = curr->next;
    }
}

void Player::removeFriend(const string &friendName)
{
    if (!friendsHead)
        return;
    if (friendsHead->friendUsername == friendName)
    {
        FriendNode *temp = friendsHead;
        friendsHead = friendsHead->next;
        delete temp;
        friendCount--;
        return;
    }
    FriendNode *curr = friendsHead;
    while (curr->next)
    {
        if (curr->next->friendUsername == friendName)
        {
            FriendNode *temp = curr->next;
            curr->next = temp->next;
            delete temp;
            friendCount--;
            return;
        }
        curr = curr->next;
    }
}

string Player::getFriendsAsString() const {
    string result = "";
    FriendNode *curr = friendsHead;
    while (curr) {
        result += curr->friendUsername;
        if (curr->next)
            result += "|";
        curr = curr->next;
    }
    return result;
}

string Player::getRequestsAsString() const {
    string result = "";
    RequestNode *curr = requestHead;
    while (curr) {
        result += curr->fromUser;
        if (curr->next)
            result += "|";
        curr = curr->next;
    }
    return result;
}