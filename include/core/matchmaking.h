#pragma once
#include "playerManager.h"

class MatchmakingSystem {
private:
    static const int MAX_QUEUE_SIZE = 100;
    
    Player* waitingQueue[MAX_QUEUE_SIZE];
    int front;
    int back;
    int queueSize;
    
    
    void insertInPriorityOrder(Player* player);
    
public:
    MatchmakingSystem();
    ~MatchmakingSystem();
    
    
    void addPlayerToQueue(Player* player);
    void removePlayerFromQueue();
    bool canMatchPlayers() const;
    
    
    void getMatchedPair(Player*& player1, Player*& player2);
    
    
    int getQueueSize() const;
    Player* getPlayerAt(int index) const;
    void resetQueue();
};