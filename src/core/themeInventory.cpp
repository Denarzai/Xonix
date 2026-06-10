#include "../../include/core/themeInventory.h"
#include <fstream>
#include <sstream>

ThemeInventory::ThemeInventory() : currentTheme(nullptr), themeCount(0) {
    
}

ThemeInventory::~ThemeInventory() {
    if (currentTheme) delete currentTheme;
}

Theme* ThemeInventory::getThemeById(int id) {
    return themeTree.search(id);
}

Theme* ThemeInventory::getThemeByName(const std::string& name) {
    return themeTree.searchByName(name);
}

Theme* ThemeInventory::getAllThemesInOrder(int& count) {
    return themeTree.getThemesInOrder(count);
}

void ThemeInventory::setCurrentTheme(int themeId) {
    Theme* theme = themeTree.search(themeId);
    if (theme != nullptr) {
        if (currentTheme) delete currentTheme;
        currentTheme = new Theme(*theme);
        
    }
}

Theme* ThemeInventory::getCurrentTheme() {
    return currentTheme;
}

int ThemeInventory::getThemeCount() {
    return themeCount;
}

void ThemeInventory::loadThemesFromFile(const std::string& filename) {
    std::ifstream file(filename);
    
    if (!file.is_open()) {
        saveThemesToFile(filename);
        return;
    }
    
    int count = 0;
    file >> count;
    file.ignore(); 
    
    for (int i = 0; i < count; i++) {
        std::string line;
        std::getline(file, line);
        
        if (line.empty()) continue;
        
        std::stringstream ss(line);
        std::string token;
        
        int themeId;
        std::string themeName, description, tilesPath, enemyPath, bgPath;
        int tileR, tileG, tileB, enemyR, enemyG, enemyB;
        char delimiter;
        
        
        ss >> themeId;
        ss.ignore(); 
        
        std::getline(ss, themeName, '|');
        std::getline(ss, description, '|');
        std::getline(ss, tilesPath, '|');
        std::getline(ss, enemyPath, '|');
        std::getline(ss, bgPath, '|');
        
        
        ss >> tileR;
        ss.ignore(); 
        ss >> tileG;
        ss.ignore(); 
        ss >> tileB;
        ss.ignore(); 
        
        
        ss >> enemyR;
        ss.ignore(); 
        ss >> enemyG;
        ss.ignore(); 
        ss >> enemyB;
        
        sf::Color tileColor(tileR, tileG, tileB);
        sf::Color enemyColor(enemyR, enemyG, enemyB);
        
        Theme theme(themeId, themeName, description, tilesPath, enemyPath, bgPath, tileColor, enemyColor);
        themeTree.insert(theme);
    }
    
    file.close();
    themeCount = count;
    
    
    Theme* defaultTheme = themeTree.search(0);
    if (defaultTheme) {
        if (currentTheme) delete currentTheme;
        currentTheme = new Theme(*defaultTheme);
        
    }
}

void ThemeInventory::saveThemesToFile(const std::string& filename) {
    std::ofstream file(filename);
    
    if (!file.is_open()) {
        return;
    }
    
    int count = 0;
    Theme* allThemes = themeTree.getThemesInOrder(count);
    
    if (count == 0) {
        file.close();
        return;
    }
    
    file << count << "\n";
    
    for (int i = 0; i < count; i++) {
        Theme& theme = allThemes[i];
        
        sf::Color tileCol = theme.getTileColor();
        sf::Color enemyCol = theme.getEnemyColor();
        
        file << theme.getThemeId() << "|"
             << theme.getThemeName() << "|"
             << theme.getDescription() << "|"
             << theme.getTilesImagePath() << "|"
             << theme.getEnemyImagePath() << "|"
             << theme.getBackgroundImagePath() << "|"
             << (int)tileCol.r << "," << (int)tileCol.g << "," << (int)tileCol.b << "|"
             << (int)enemyCol.r << "," << (int)enemyCol.g << "," << (int)enemyCol.b << "\n";
    }
    
    file.close();
    delete[] allThemes;
}