#ifndef LEADERBOARD_H
#define LEADERBOARD_H

#include <string>

struct LBNode {
    std::string name;
    int score;

    LBNode() : name(""), score(0) {}
    LBNode(std::string n, int s) : name(n), score(s) {}
};

class Leaderboard {
private:
    LBNode heap[10];   
    int size;          

    void heapifyUp(int index);
    void heapifyDown(int index);

public:
    Leaderboard();

    void loadFromFile(const std::string& filename);
    void saveToFile(const std::string& filename);

    void update(std::string n, int totalScore);
    bool playerInLeaderboard(std::string n);

    int getSortedDescending(LBNode outArray[10]) const;
    int getSortedAscending(LBNode outArray[10]) const;

    int getCount() const {return size;}
};

#endif
