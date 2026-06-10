#pragma once
#include "saveGame.h"
#include <fstream>

class SaveGameManager {
private:
    string savesDirectory;
    string manifestFile;
    void addToManifest(const string& saveID, const string& username);  
    void removeFromManifest(const string& saveID);  
    
public:
    SaveGameManager();
    ~SaveGameManager();
    
    
    bool saveGame(const SaveGame& game);
    bool loadGame(const string& saveID, SaveGame& game);
    
    
    bool deleteSave(const string& saveID);
    bool saveExists(const string& saveID);
    
    
    string generateSaveID(const string& username);
    int getSavesForPlayer(const string& username, string saveIDs[10]);
};