#pragma once

#include "../core/theme.h"

class ThemeInventory {
private:
    ThemeAVLTree themeTree;
    Theme* currentTheme;
    int themeCount;
    
public:
    ThemeInventory();
    ~ThemeInventory();
    
    Theme* getThemeById(int id);
    Theme* getThemeByName(const std::string& name);
    Theme* getAllThemesInOrder(int& count);
    void setCurrentTheme(int themeId);
    Theme* getCurrentTheme();
    void loadThemesFromFile(const std::string& filename);
    void saveThemesToFile(const std::string& filename);
    int getThemeCount();
};
