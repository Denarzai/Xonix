#pragma once

#include <string>
#include <SFML/Graphics.hpp>

class Theme {
private:
    int themeId;
    std::string themeName;
    std::string description;
    std::string tilesImagePath;
    std::string enemyImagePath;
    std::string backgroundImagePath;
    sf::Color tileColor;
    sf::Color enemyColor;

public:
    Theme(int id, const std::string& name, const std::string& desc,
          const std::string& tilesPath, const std::string& enemyPath,
          const std::string& bgPath, sf::Color tilCol, sf::Color enmCol);
    
    Theme(); 
    
    
    int getThemeId() const;
    std::string getThemeName() const;
    std::string getDescription() const;
    std::string getTilesImagePath() const;
    std::string getEnemyImagePath() const;
    std::string getBackgroundImagePath() const;
    sf::Color getTileColor() const;
    sf::Color getEnemyColor() const;
    
    
    void setThemeId(int id);
    void setThemeName(const std::string& name);
    void setTileColor(sf::Color col);
    void setEnemyColor(sf::Color col);
};



class ThemeNode {
public:
    Theme theme;
    ThemeNode* left;
    ThemeNode* right;
    int height;
    
    ThemeNode(const Theme& t);
    ~ThemeNode();
};



class ThemeAVLTree {
private:
    ThemeNode* root;
    int treeSize;
    
    int getHeight(ThemeNode* node);
    int getBalanceFactor(ThemeNode* node);
    ThemeNode* rotateRight(ThemeNode* y);
    ThemeNode* rotateLeft(ThemeNode* x);
    ThemeNode* insertNode(ThemeNode* node, const Theme& theme);
    ThemeNode* searchNode(ThemeNode* node, int themeId);
    ThemeNode* searchNodeByName(ThemeNode* node, const std::string& name);
    void inOrderTraversal(ThemeNode* node, Theme** themes, int& index);
    void deleteTree(ThemeNode* node);
    void countNodes(ThemeNode* node, int& count);

public:
    ThemeAVLTree();
    ~ThemeAVLTree();
    
    void insert(const Theme& theme);
    Theme* search(int themeId);
    Theme* searchByName(const std::string& name);
    Theme* getThemesInOrder(int& count);
    int getTreeSize();
};
