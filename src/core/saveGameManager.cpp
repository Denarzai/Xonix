#include "../../include/core/saveGameManager.h"
#include <iostream>
#include <ctime>
#include <cstdlib>

SaveGameManager::SaveGameManager() {
    savesDirectory = "../data/saves/";
    manifestFile = savesDirectory + "manifest.txt";
}

SaveGameManager::~SaveGameManager() {
}

void SaveGameManager::addToManifest(const string& saveID, const string& username) {
    ofstream file(manifestFile, ios::app);  
    if (file) {
        file << username << "," << saveID << "\n";
        file.close();
    }
}

void SaveGameManager::removeFromManifest(const string& saveID) {
    
    ifstream inFile(manifestFile);
    string tempFile = manifestFile + ".tmp";
    ofstream outFile(tempFile);
    
    string line;
    while (getline(inFile, line)) {
        if (line.find(saveID) == string::npos) {  
            outFile << line << "\n";
        }
    }
    
    inFile.close();
    outFile.close();
    
    
    remove(manifestFile.c_str());
    rename(tempFile.c_str(), manifestFile.c_str());
}

string SaveGameManager::generateSaveID(const string& username) {
    time_t now = time(nullptr);
    int randomNum = rand() % 10000;
    string saveID = username + "_" + to_string(now) + "_" + to_string(randomNum);
    return saveID;
}

bool SaveGameManager::saveGame(const SaveGame& game) {
    string filename = savesDirectory + game.getSaveID() + ".txt";
    ofstream file(filename);
    
    if (!file) {
        cerr << "Error: Could not create save file: " << filename << endl;
        return false;
    }
    
    
    file << game.getSaveID() << ",";
    file << game.getTimestamp() << ",";
    file << game.getUsername() << ",";
    file << game.getPlayerID() << ",";
    
    
    file << game.getGameTimer() << ",";
    file << game.getTotalPointsEarnedThisGame() << ",";
    
    int playerX, playerY;
    game.getPlayerPosition(playerX, playerY);
    file << playerX << ",";
    file << playerY << ",";
    
    
    file << game.getCurrentPlayerTotalPoints() << ",";
    file << game.getBonusMultiplierCount() << ",";
    file << game.getPowerUpCount() << ",";
    file << (game.getPowerUpActive() ? 1 : 0) << ",";
    file << game.getPowerUpTimeRemaining() << ",";
    
    
    int minSpd, maxSpd;
    game.getDifficulty(minSpd, maxSpd);
    file << minSpd << ",";
    file << maxSpd << ",";
    
    
    file << game.getEnemyCount() << "\n";
    
    for (int i = 0; i < game.getEnemyCount(); i++) {
        SavedEnemy enemy = game.getEnemy(i);
        file << enemy.x << ",";
        file << enemy.y << ",";
        file << enemy.dx << ",";
        file << enemy.dy << "\n";
    }
    
    
    int tempGrid[25][40];
    game.getGrid(tempGrid);
    for (int i = 0; i < 25; i++) {
        for (int j = 0; j < 40; j++) {
            file << tempGrid[i][j];
            if (j < 39) file << ",";
        }
        file << "\n";
    }
    
    file.close();

    addToManifest(game.getSaveID(), game.getUsername());


    cout << "Game saved successfully: " << filename << endl;
    return true;
}

bool SaveGameManager::loadGame(const string& saveID, SaveGame& game) {
    string filename = savesDirectory + saveID + ".txt";
    ifstream file(filename);
    
    if (!file) {
        cerr << "Error: Could not open save file: " << filename << endl;
        return false;
    }
    
    string line;
    int lineNum = 0;
    int gridRow = 0;
    int tempGrid[25][40];
    
    while (getline(file, line)) {
        if (line.empty()) continue;
        
        if (lineNum == 0) {
            
            string tmp;
            int field = 0;
            int playerX = 0, playerY = 0;
            int minSpd = 0, maxSpd = 0;
            
            for (int i = 0; i < line.length(); i++) {
                if (line[i] == ',') {
                    if (field == 0) game.setSaveID(tmp);
                    else if (field == 1) game.setTimestamp((time_t)atoll(tmp.c_str()));
                    else if (field == 2) game.setUsername(tmp);
                    else if (field == 3) game.setPlayerID(atoi(tmp.c_str()));
                    else if (field == 4) game.setGameTimer(atof(tmp.c_str()));
                    else if (field == 5) game.setTotalPointsEarnedThisGame(atoi(tmp.c_str()));
                    
                   
                    else if (field == 6) playerX = atoi(tmp.c_str());
                    else if (field == 7) {
                        playerY = atoi(tmp.c_str());
                        game.setPlayerPosition(playerX, playerY);
                    }
                    
                    else if (field == 8) game.setCurrentPlayerTotalPoints(atoi(tmp.c_str()));
                    else if (field == 9) game.setBonusMultiplierCount(atoi(tmp.c_str()));
                    else if (field == 10) game.setPowerUpCount(atoi(tmp.c_str()));
                    else if (field == 11) game.setPowerUpActive(atoi(tmp.c_str()) == 1);
                    else if (field == 12) game.setPowerUpTimeRemaining(atof(tmp.c_str()));
                    
                    else if (field == 13) minSpd = atoi(tmp.c_str());
                    else if (field == 14) {
                        maxSpd = atoi(tmp.c_str());
                        game.setDifficulty(minSpd, maxSpd);
                    }
                    
                    else if (field == 15) game.setEnemyCount(atoi(tmp.c_str()));
                    
                    tmp = "";
                    field++;
                } else {
                    tmp += line[i];
                }
            }
            if (!tmp.empty()) game.setEnemyCount(atoi(tmp.c_str()));
            lineNum++;
        } 
        else if (lineNum > 0 && lineNum <= game.getEnemyCount()) {
            
            string tmp;
            int field = 0;
            int ex = 0, ey = 0, edx = 0, edy = 0;
            
            for (int i = 0; i < line.length(); i++) {
                if (line[i] == ',') {
                    if (field == 0) ex = atoi(tmp.c_str());
                    else if (field == 1) ey = atoi(tmp.c_str());
                    else if (field == 2) edx = atoi(tmp.c_str());
                    
                    tmp = "";
                    field++;
                } else {
                    tmp += line[i];
                }
            }
            if (!tmp.empty()) edy = atoi(tmp.c_str());
            
            game.setEnemy(lineNum - 1, ex, ey, edx, edy);
            lineNum++;
        } 
        else {
            
            string tmp;
            int col = 0;
            
            for (int i = 0; i < line.length(); i++) {
                if (line[i] == ',') {
                    tempGrid[gridRow][col] = atoi(tmp.c_str());
                    tmp = "";
                    col++;
                } else {
                    tmp += line[i];
                }
            }
            if (!tmp.empty()) tempGrid[gridRow][col] = atoi(tmp.c_str());
            
            gridRow++;
        }
    }
    
    game.setGrid(tempGrid);
    file.close();
    return true;
}

bool SaveGameManager::deleteSave(const string& saveID) {
    string filename = savesDirectory + saveID + ".txt";
    if (remove(filename.c_str()) == 0) {
        removeFromManifest(saveID); 
        return true;
    } 
    else {
        cerr << "Error deleting save file: " << filename << endl;
        return false;
    }
}

bool SaveGameManager::saveExists(const string& saveID) {
    string filename = savesDirectory + saveID + ".txt";
    ifstream file(filename);
    bool exists = file.good();
    file.close();
    return exists;
}

int SaveGameManager::getSavesForPlayer(const string& username, string saveIDs[10]) {
    ifstream file(manifestFile);
    int count = 0;
    
    if (!file) {
        return 0;  
    }
    
    string line;
    while (getline(file, line) && count < 10) {
        
        int commaPos = line.find(',');
        if (commaPos != string::npos) {
            string fileUsername = line.substr(0, commaPos);
            string saveID = line.substr(commaPos + 1);
            
            if (fileUsername == username) {
                saveIDs[count++] = saveID;
            }
        }
    }
    
    file.close();
    return count;
}