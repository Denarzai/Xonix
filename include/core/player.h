#pragma once
#include <string>
#include <sstream>
using namespace std;

struct FriendNode {
    string friendUsername;
    FriendNode* next;

    FriendNode(const string& name)
        : friendUsername(name), next(nullptr) {}
};

struct RequestNode {
    string fromUser;
    RequestNode* next;

    RequestNode(const string& user)
        : fromUser(user), next(nullptr) {}
};

class Player {
private:
    string username;
    string password;
    string nickname;
    string email;
    int playerID;
    time_t registrationTimestamp;

    int totalPoints;
    int wins;
    int losses;
    int highScore;

    int currentThemeID;

    int bonusMultiplierCount;      
    int tilesInCurrentMove;        
    int currentMultiplier;         
    int lastPowerUpThreshold;      
    int tilesCapturedInCurrentMove;
    
    
    int powerUpCount;
    bool powerUpActive;            
    float powerUpTimeRemaining; 

    FriendNode* friendsHead;
    RequestNode* requestHead;

    static const int MAX_FRIENDS = 50;
    static const int MAX_REQUESTS = 20;
    
    int friendCount;
    int requestCount;

public:
    Player();
    Player(const string& user, const string& pass);
    Player(const Player& other);
    Player& operator=(const Player& other);

    ~Player();

    FriendNode* getFriendsHead() const { return friendsHead; }
    RequestNode* getRequestHead() const { return requestHead; }

    void setNickname(const string&);
    void setEmail(const string&);
    void setPlayerID(int id);
    void setRegistrationTimestamp(time_t);
    void setThemeID(int id);

    string getUsername() const;
    string getPassword() const;
    string getNickname() const;
    string getEmail() const;
    int getPlayerID() const;
    int getThemeID() const;
    int getTotalPoints() const;
    int getPowerUps() const;
    int getWins() const;
    int getLosses() const;
    time_t getRegistrationTimestamp() const;
        
    void addPoints(int p);
    void addWin();
    void addLoss();
    void setPowerUps(int s);
    int getHighScore();
    void setHighScore(int n);
    void setTotalPoints(int n) {totalPoints = n;}

    void resetMoveCounter();
    void updateMultiplier();
    int calculatePoints(int basePoints);
    void awardPoints(int points, int pointsEarnedThisGame);
    void checkAndAwardPowerUp(int pointsEarnedThisGame);
    int getTilesInCurrentMove() const;
    int getCurrentMultiplier() const;
    int geTilesCapturedInCurrentMove() const;
    
    void usePowerUp();
    void deactivatePowerUp();
    void updatePowerUpTimer(float deltaTime);
    bool isPowerUpActive() const;
    float getPowerUpTimeRemaining() const;

    int getBonusMultiplierCount() const { return bonusMultiplierCount; }
    void setBonusMultiplierCount(int count) { bonusMultiplierCount = count; }
    void setTilesCapturedInCurrentMove(int count) {tilesCapturedInCurrentMove = count;}

    int getLastPowerUpThreshold() const { return lastPowerUpThreshold; }
    void setLastPowerUpThreshold(int threshold) { lastPowerUpThreshold = threshold; }
    
    bool addFriend(const string& friendName);
    bool isFriend(const string& friendName);
    void removeFriend(const string& friendName);
    void displayFriends() const;
    
    bool addFriendRequest(const string& fromUser);
    bool hasRequestFrom(const string& fromUser);
    void removeFriendRequest(const string& fromUser);
    void displayRequests() const;
    
    bool addSaveID(const string& saveID);
    void displaySaveIDs() const;

    string getFriendsAsString() const;
    string getRequestsAsString() const;
    
 
    bool isFriendLimitReached() const;
    bool isRequestLimitReached() const;
    int getFriendCount() const { return friendCount; }
    int getRequestCount() const { return requestCount; }


};
