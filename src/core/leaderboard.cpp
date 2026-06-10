#include "../../include/core/leaderboard.h"
#include <fstream>
#include <iostream>

Leaderboard::Leaderboard() {
    size = 0;
}

void Leaderboard::heapifyUp(int index) {
    while (index > 0) {
        int parent = (index - 1) / 2;
        if (heap[index].score < heap[parent].score) {
            LBNode temp = heap[index];
            heap[index] = heap[parent];
            heap[parent] = temp;

            index = parent;
        } 
        else break;
    }
}

void Leaderboard::heapifyDown(int index) {
    while (true) {
        int left = 2 * index + 1;
        int right = left + 1;
        int smallest = index;

        if (left < size && heap[left].score < heap[smallest].score)
            smallest = left;
        if (right < size && heap[right].score < heap[smallest].score)
            smallest = right;

        if (smallest != index) {
            LBNode temp = heap[index];
            heap[index] = heap[smallest];
            heap[smallest] = temp;

            index = smallest;
        } 
        else break;
    }
}

void Leaderboard::loadFromFile(const std::string& filename) {
    size = 0;
    std::ifstream file("../data/" + filename);
    if (!file) return;

    std::string username, scoreStr;
    int score;

    while ( getline(file, username, ',') && getline(file, scoreStr, '\n') ){
        score = std::stoi(scoreStr);
        if (size < 10) {
            heap[size] = LBNode(username, score);
            heapifyUp(size);
            size++;
        } else {
            if (score > heap[0].score) {
                heap[0] = LBNode(username, score);
                heapifyDown(0);
            }
        }
    }
    file.close();
}

void Leaderboard::saveToFile(const std::string& filename) {
    std::ofstream file("../data/" + filename);

    for (int i = 0; i < size; i++) {
        file << heap[i].name << "," << heap[i].score << "\n";
    }

    file.close();
}

void Leaderboard::update(std::string n, int totalScore) {
    for (int i = 0; i < size; i++) {
        if (heap[i].name == n) {
            
            if (totalScore > heap[i].score) {
                heap[i].score = totalScore;
                heapifyUp(i);
                heapifyDown(i);
            }
            return;
        }
    }

    if (size < 10) {
        heap[size] = LBNode(n, totalScore);
        heapifyUp(size);
        size++;
        return;
    }

    if (totalScore > heap[0].score) {
        heap[0] = LBNode(n, totalScore);
        heapifyDown(0);
    }
}

int Leaderboard::getSortedDescending(LBNode outArray[10]) const {
    LBNode temp[10];
    for (int i = 0; i < size; i++) temp[i] = heap[i];

    for (int i = 0; i < size; i++) {
        int maxIdx = i;
        for (int j = i + 1; j < size; j++) {
            if (temp[j].score > temp[maxIdx].score)
                maxIdx = j;
        }
        LBNode t = temp[i];
        temp[i] = temp[maxIdx];
        temp[maxIdx] = t;
    }

    for (int i = 0; i < size; i++) outArray[i] = temp[i];

    return size; 
}

int Leaderboard::getSortedAscending(LBNode outArray[10]) const {
    LBNode temp[10];
    for (int i = 0; i < size; i++) temp[i] = heap[i];

    for (int i = 0; i < size; i++) {
        int maxIdx = i;
        for (int j = i + 1; j < size; j++) {
            if (temp[j].score < temp[maxIdx].score)
                maxIdx = j;
        }
        LBNode t = temp[i];
        temp[i] = temp[maxIdx];
        temp[maxIdx] = t;
    }

    for (int i = 0; i < size; i++) outArray[i] = temp[i];

    return size; 
}

bool Leaderboard::playerInLeaderboard(std::string n){
    for (int i = 0; i < size; i++) {
        if (heap[i].name == n) {
            return true;
        }
    }
    return false;
}
