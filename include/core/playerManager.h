#pragma once
#include "player.h"
#include <string>

struct HashEntry {
    string username;
    int index;
    bool occupied;
};

class PlayerManager {
private:
    Player* players;
    int capacity;
    int count;

    HashEntry* hashTable;
    int hashCapacity;
    int hashCount;

    void resizeArray();
    void resizeHashTable();
    int hash(const string& username) const;
    int findHashPosition(const string& username) const;
    int generateUniqueId() ;

public:
    PlayerManager();
    ~PlayerManager();
    bool registerPlayer(const string& username, const string& password,
                        const string& nickname = "", const string& email = "");
    Player* loginPlayer(const string& username, const string& password);
    Player* getPlayerByUsername(const string& username);
    int getCount() const;
    void saveToFile(const string& filename);
    void loadFromFile(const string& filename);

    int searchPlayers(const string& searchTerm, Player** results, int maxResults);
    bool areFriends(const string& user1, const string& user2);
    int getMutualFriends(const string& user1, const string& user2, Player** results, int maxResults);

    int getRemainingFriendSlots(const string& username);
    int getRemainingRequestSlots(const string& username);

    bool sendFriendRequest(const string& senderUsername, const string& targetUsername);
    bool acceptFriendRequest(const string& receiverUsername, const string& requesterUsername);
    bool rejectFriendRequest(const string& receiverUsername, const string& requesterUsername);
    bool deleteFriend(const string& user1, const string& user2);

};
