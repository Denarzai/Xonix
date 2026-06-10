#include "../../include/core/playerManager.h"
#include <fstream>
#include <iostream>

PlayerManager::PlayerManager()
{
    capacity = 10;
    count = 0;
    players = new Player[capacity];

    hashCapacity = 20;
    hashCount = 0;
    hashTable = new HashEntry[hashCapacity];
    for (int i = 0; i < hashCapacity; i++)
        hashTable[i].occupied = false;
}

PlayerManager::~PlayerManager()
{
    delete[] players;
    delete[] hashTable;
}

void PlayerManager::resizeArray()
{
    int newCap = capacity * 2;
    Player *newArr = new Player[newCap];
    for (int i = 0; i < count; i++)
        newArr[i] = players[i];
    delete[] players;
    players = newArr;
    capacity = newCap;
}

void PlayerManager::resizeHashTable()
{
    int oldCap = hashCapacity;
    HashEntry *oldTable = hashTable;

    hashCapacity *= 2;
    hashTable = new HashEntry[hashCapacity];
    for (int i = 0; i < hashCapacity; i++)
    {
        hashTable[i].occupied = false;
    }
    hashCount = 0;

    for (int i = 0; i < oldCap; i++)
    {
        if (oldTable[i].occupied)
        {
            string username = oldTable[i].username;
            int playerIdx = oldTable[i].index;

            int h = hash(username);
            for (int j = 0; j < hashCapacity; j++)
            {
                int idx = (h + j) % hashCapacity;
                if (!hashTable[idx].occupied)
                {
                    hashTable[idx].occupied = true;
                    hashTable[idx].username = username;
                    hashTable[idx].index = playerIdx;
                    hashCount++;
                    break;
                }
            }
        }
    }
    delete[] oldTable;
}

int PlayerManager::hash(const string &username) const
{
    long long h = 0;
    for (char c : username)
        h = (h * 31 + c) % hashCapacity;
    return (int)h;
}

int PlayerManager::findHashPosition(const string &username) const
{
    int h = hash(username);
    for (int i = 0; i < hashCapacity; i++)
    {
        int idx = (h + i) % hashCapacity;

        if (hashTable[idx].occupied && hashTable[idx].username == username)
        {
            return idx;
        }

        if (!hashTable[idx].occupied)
        {
            return -1;
        }
    }
    return -1;
}

int PlayerManager::generateUniqueId()
{
    bool unique = true;
    int newID;
    do
    {
        newID = rand() % 9000 + 1000;
        unique = true;
        ifstream file("../data/players_test.txt");
        if (!file)
            return newID;
        string tmp;
        while (getline(file, tmp, ','))
        {
            if (tmp.empty())
                continue;

            string username, password, nickname, email;
            int id = 0, points = 0, wins = 0, losses = 0, theme = 0, power = 0;
            username = tmp;
            getline(file, tmp, ',');
            password = tmp;

            getline(file, tmp, ',');
            id = tmp.empty() ? 0 : stoi(tmp);
            getline(file, tmp, ',');
            points = tmp.empty() ? 0 : stoi(tmp);
            getline(file, tmp, ',');
            wins = tmp.empty() ? 0 : stoi(tmp);
            getline(file, tmp, ',');
            losses = tmp.empty() ? 0 : stoi(tmp);

            getline(file, nickname, ',');
            getline(file, email, ',');
            getline(file, tmp, ',');
            theme = tmp.empty() ? 0 : stoi(tmp);
            getline(file, tmp);
            power = tmp.empty() ? 0 : stoi(tmp);

            if (newID == id)
            {
                unique = false;
                file.close();
                break;
            }
        }
        file.close();
    } while (!unique);

    return newID;
}

bool PlayerManager::registerPlayer(const string &username, const string &password,
                                   const string &nickname, const string &email)
{
    if (getPlayerByUsername(username) != nullptr)
        return false;

    if (count == capacity)
        resizeArray();

    Player p(username, password);
    p.setNickname(nickname);
    p.setEmail(email);
    p.setPlayerID(generateUniqueId());
    p.setRegistrationTimestamp(time(nullptr));
    players[count] = p;

    if (hashCount * 100 / hashCapacity >= 70)
        resizeHashTable();

    int h = hash(username);
    for (int i = 0; i < hashCapacity; i++)
    {
        int idx = (h + i) % hashCapacity;
        if (!hashTable[idx].occupied)
        {
            hashTable[idx].occupied = true;
            hashTable[idx].username = username;
            hashTable[idx].index = count;
            hashCount++;
            break;
        }
    }

    count++;
    return true;
}

Player *PlayerManager::loginPlayer(const string &username, const string &password)
{
    Player *p = getPlayerByUsername(username);
    if (!p)
        return nullptr;
    if (p->getPassword() != password)
        return nullptr;
    return p;
}

Player *PlayerManager::getPlayerByUsername(const string &username)
{
    int pos = findHashPosition(username);
    if (pos == -1)
        return nullptr;
    return &players[hashTable[pos].index];
}

int PlayerManager::getCount() const
{
    return count;
}

void PlayerManager::loadFromFile(const string &filename)
{
    ifstream file("../data/" + filename);
    if (!file)
        return;

    string tmp;
    while (getline(file, tmp, ','))
    {
        if (tmp.empty())
            continue;

        string username, password, nickname, email;
        string friendsList, requestsList;
        int id = 0, points = 0, wins = 0, losses = 0, theme = 0, power = 0;
        int bonusCount = 0, highScore = 0;

        username = tmp;
        getline(file, tmp, ',');
        password = tmp;
        getline(file, tmp, ',');
        id = tmp.empty() ? 0 : stoi(tmp);
        getline(file, tmp, ',');
        points = tmp.empty() ? 0 : stoi(tmp);
        getline(file, tmp, ',');
        wins = tmp.empty() ? 0 : stoi(tmp);
        getline(file, tmp, ',');
        losses = tmp.empty() ? 0 : stoi(tmp);
        getline(file, nickname, ',');
        getline(file, email, ',');
        getline(file, tmp, ',');
        theme = tmp.empty() ? 0 : stoi(tmp);
        getline(file, tmp, ',');
        power = tmp.empty() ? 0 : stoi(tmp);
        getline(file, tmp, ',');
        bonusCount = tmp.empty() ? 0 : stoi(tmp);
        getline(file, tmp, ',');
        highScore = tmp.empty() ? 0 : stoi(tmp);
        getline(file, friendsList, ',');
        getline(file, requestsList, '\n');

        if (count == capacity)
            resizeArray();

        Player p(username, password);
        p.setPlayerID(id);
        p.setNickname(nickname);
        p.setEmail(email);
        p.setThemeID(theme);
        p.setTotalPoints(points);
        p.setPowerUps(power);
        p.setBonusMultiplierCount(bonusCount);
        p.setHighScore(highScore);

        for (int i = 0; i < wins; i++)
            p.addWin();
        for (int i = 0; i < losses; i++)
            p.addLoss();

        if (!friendsList.empty())
        {
            stringstream ss(friendsList);
            string friendName;
            while (getline(ss, friendName, '|'))
            {
                if (!friendName.empty())
                {
                    p.addFriend(friendName);
                
                }
                else break;
            }
        }

        if (!requestsList.empty())
        {
            stringstream ss(requestsList);
            string requesterName;
            while (getline(ss, requesterName, '|'))
            {
                if (!requesterName.empty()) {
                    p.addFriendRequest(requesterName);
                }
                else break;
            }
        }

        players[count] = p;

        if (hashCount * 100 / hashCapacity >= 70)
            resizeHashTable();

        int h = hash(username);
        for (int i = 0; i < hashCapacity; i++)
        {
            int idx = (h + i) % hashCapacity;
            if (!hashTable[idx].occupied)
            {
                hashTable[idx].occupied = true;
                hashTable[idx].username = username;
                hashTable[idx].index = count;
                hashCount++;
                break;
            }
        }
        count++;
    }

    file.close();
}

void PlayerManager::saveToFile(const string& filename)
{
    ofstream file("../data/" + filename);
    if (!file) {
        cerr << "Error: Could not open file for writing: " << filename << endl;
        return;
    }

    for (int i = 0; i < count; i++) {
        Player& p = players[i];
        file << p.getUsername() << ","
             << p.getPassword() << ","
             << p.getPlayerID() << ","
             << p.getTotalPoints() << ","
             << p.getWins() << ","
             << p.getLosses() << ","
             << p.getNickname() << ","
             << p.getEmail() << ","
             << p.getThemeID() << ","
             << p.getPowerUps() << ","
             << p.getBonusMultiplierCount() << ","
             << p.getHighScore() << ","
             << p.getFriendsAsString() << ","
             << p.getRequestsAsString() << "\n";
    }

    file.close();
}



int PlayerManager::searchPlayers(const string& searchTerm, Player** results, int maxResults) {
    int found = 0;
    string lowerSearch = searchTerm;
    
    for (char& c : lowerSearch) c = tolower(c);
    
    for (int i = 0; i < count && found < maxResults; i++) {
        string username = players[i].getUsername();
         
        for (char& c : username) c = tolower(c);
        
        if (username.find(lowerSearch) != string::npos) {
            results[found] = &players[i];
            found++;
        }
    }
    
    return found;
}

bool PlayerManager::areFriends(const string& user1, const string& user2) {
    Player* p1 = getPlayerByUsername(user1);
    Player* p2 = getPlayerByUsername(user2);
    
    if (!p1 || !p2) return false;
    
    return p1->isFriend(user2) && p2->isFriend(user1);
}

int PlayerManager::getMutualFriends(const string& user1, const string& user2, Player** results, int maxResults) {
    Player* p1 = getPlayerByUsername(user1);
    Player* p2 = getPlayerByUsername(user2);
    
    if (!p1 || !p2) return 0;
    
    int mutualCount = 0;
    FriendNode* curr = p1->getFriendsHead();
    
    while (curr && mutualCount < maxResults) {
        
        if (p2->isFriend(curr->friendUsername)) {
            Player* mutual = getPlayerByUsername(curr->friendUsername);
            if (mutual) {
                results[mutualCount] = mutual;
                mutualCount++;
            }
        }
        curr = curr->next;
    }
    
    return mutualCount;
}

int PlayerManager::getRemainingFriendSlots(const string& username) {
    Player* p = getPlayerByUsername(username);
    if (!p) return -1;
    return 50 - p->getFriendCount();
}

int PlayerManager::getRemainingRequestSlots(const string& username) {
    Player* p = getPlayerByUsername(username);
    if (!p) return -1;
    return 20 - p->getRequestCount();
}

bool PlayerManager::sendFriendRequest(const string& senderUsername, const string& targetUsername) {
    if (senderUsername == targetUsername) return false;

    Player* sender = getPlayerByUsername(senderUsername);
    Player* target = getPlayerByUsername(targetUsername);

    if (!sender || !target) return false;
    if (target->isFriend(senderUsername)) return false;
    if (target->hasRequestFrom(senderUsername)) return false;
    if (target->isRequestLimitReached()) return false;

    return target->addFriendRequest(senderUsername);
}

bool PlayerManager::acceptFriendRequest(const string& receiverUsername, const string& requesterUsername) {
    Player* receiver = getPlayerByUsername(receiverUsername);
    Player* requester = getPlayerByUsername(requesterUsername);

    if (!receiver || !requester) return false;
    if (!receiver->hasRequestFrom(requesterUsername)) return false;
    if (receiver->isFriendLimitReached() || requester->isFriendLimitReached()) return false;

    bool receiverAdded = receiver->addFriend(requesterUsername);
    bool requesterAdded = requester->addFriend(receiverUsername);

    if (receiverAdded && requesterAdded) {
        receiver->removeFriendRequest(requesterUsername);
        return true;
    }
    
    if (receiverAdded) receiver->removeFriend(requesterUsername);
    if (requesterAdded) requester->removeFriend(receiverUsername);
    
    return false;
}

bool PlayerManager::rejectFriendRequest(const string& receiverUsername, const string& requesterUsername) {
    Player* receiver = getPlayerByUsername(receiverUsername);
    if (!receiver) return false;
    if (!receiver->hasRequestFrom(requesterUsername)) return false;

    receiver->removeFriendRequest(requesterUsername);
    return true;
}

bool PlayerManager::deleteFriend(const string& user1, const string& user2) {
    Player* u1 = getPlayerByUsername(user1);
    Player* u2 = getPlayerByUsername(user2);

    if (!u1 || !u2) return false;

    if(u1->isFriend(user2) && u2->isFriend(user1)){
        u1->removeFriend(user2);
        u2->removeFriend(user1);
        return true;
    }
    
    return false;
}