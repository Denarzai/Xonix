#include "../../include/core/matchmaking.h"
#include <iostream>

MatchmakingSystem::MatchmakingSystem() : front(0), back(-1), queueSize(0) {
    for (int i = 0; i < MAX_QUEUE_SIZE; i++) {
        waitingQueue[i] = nullptr;
    }
}

MatchmakingSystem::~MatchmakingSystem() {
    
    front = 0;
    back = -1;
    queueSize = 0;
}

void MatchmakingSystem::insertInPriorityOrder(Player* player) {
    if (queueSize >= MAX_QUEUE_SIZE) {
        std::cerr << "Queue is full!" << std::endl;
        return;
    }
    
    
    if (queueSize == 0) {
        back = front;
        waitingQueue[front] = player;
        queueSize++;
        return;
    }
    
    
    int insertPos = front;
    int playerScore = player->getTotalPoints();
    
    
    for (int i = front; i <= back; i++) {
        if (waitingQueue[i]->getTotalPoints() < playerScore) {
            insertPos = i;
            break;
        }
        insertPos = i + 1;
    }
    
    
    for (int i = back + 1; i > insertPos; i--) {
        waitingQueue[i] = waitingQueue[i - 1];
    }
    
    
    waitingQueue[insertPos] = player;
    back++;
    queueSize++;
}

void MatchmakingSystem::addPlayerToQueue(Player* player) {
    if (!player) return;
    
    if (queueSize >= MAX_QUEUE_SIZE) {

        return;
    }
    
    
    for (int i = front; i <= back; i++) {
        if (waitingQueue[i] == player) {
            return;
        }
    }
    
    insertInPriorityOrder(player);
}

void MatchmakingSystem::removePlayerFromQueue() {
    if (queueSize <= 0) {
        return;
    }
    
    
    front++;
    queueSize--;
    
    
    if (queueSize == 0) {
        front = 0;
        back = -1;
    }
}

bool MatchmakingSystem::canMatchPlayers() const {
    return queueSize >= 2;
}

void MatchmakingSystem::getMatchedPair(Player*& player1, Player*& player2) {
    if (!canMatchPlayers()) {
        player1 = nullptr;
        player2 = nullptr;
        return;
    }
    
    
    player1 = waitingQueue[front];
    player2 = waitingQueue[front + 1];
    
    
    removePlayerFromQueue();
    removePlayerFromQueue();
}

int MatchmakingSystem::getQueueSize() const {
    return queueSize;
}

Player* MatchmakingSystem::getPlayerAt(int index) const {
    if (index < 0 || index >= queueSize) {
        return nullptr;
    }
    return waitingQueue[front + index];
}

void MatchmakingSystem::resetQueue() {
    for (int i = 0; i < MAX_QUEUE_SIZE; i++) {
        waitingQueue[i] = nullptr;
    }
    front = 0;
    back = -1;
    queueSize = 0;
}