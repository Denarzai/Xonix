#include "../../include/core/theme.h"



Theme::Theme(int id, const std::string& name, const std::string& desc,
             const std::string& tilesPath, const std::string& enemyPath,
             const std::string& bgPath, sf::Color tilCol, sf::Color enmCol)
    : themeId(id), themeName(name), description(desc),
      tilesImagePath(tilesPath), enemyImagePath(enemyPath),
      backgroundImagePath(bgPath), tileColor(tilCol), enemyColor(enmCol) {}

Theme::Theme() : themeId(-1), themeName(""), description(""),
                 tilesImagePath(""), enemyImagePath(""), backgroundImagePath(""),
                 tileColor(sf::Color::White), enemyColor(sf::Color::White) {}

int Theme::getThemeId() const { return themeId; }
std::string Theme::getThemeName() const { return themeName; }
std::string Theme::getDescription() const { return description; }
std::string Theme::getTilesImagePath() const { return tilesImagePath; }
std::string Theme::getEnemyImagePath() const { return enemyImagePath; }
std::string Theme::getBackgroundImagePath() const { return backgroundImagePath; }
sf::Color Theme::getTileColor() const { return tileColor; }
sf::Color Theme::getEnemyColor() const { return enemyColor; }

void Theme::setThemeId(int id) { themeId = id; }
void Theme::setThemeName(const std::string& name) { themeName = name; }
void Theme::setTileColor(sf::Color col) { tileColor = col; }
void Theme::setEnemyColor(sf::Color col) { enemyColor = col; }



ThemeNode::ThemeNode(const Theme& t) : theme(t), left(nullptr), right(nullptr), height(1) {}

ThemeNode::~ThemeNode() {
    
    
}



ThemeAVLTree::ThemeAVLTree() : root(nullptr), treeSize(0) {}

ThemeAVLTree::~ThemeAVLTree() { deleteTree(root); }

int ThemeAVLTree::getHeight(ThemeNode* node) {
    return (node == nullptr) ? 0 : node->height;
}

int ThemeAVLTree::getBalanceFactor(ThemeNode* node) {
    return (node == nullptr) ? 0 : getHeight(node->left) - getHeight(node->right);
}

ThemeNode* ThemeAVLTree::rotateRight(ThemeNode* y) {
    ThemeNode* x = y->left;
    ThemeNode* t2 = x->right;
    x->right = y;
    y->left = t2;
    y->height = 1 + (getHeight(y->left) > getHeight(y->right) ? getHeight(y->left) : getHeight(y->right));
    x->height = 1 + (getHeight(x->left) > getHeight(x->right) ? getHeight(x->left) : getHeight(x->right));
    return x;
}

ThemeNode* ThemeAVLTree::rotateLeft(ThemeNode* x) {
    ThemeNode* y = x->right;
    ThemeNode* t2 = y->left;
    y->left = x;
    x->right = t2;
    x->height = 1 + (getHeight(x->left) > getHeight(x->right) ? getHeight(x->left) : getHeight(x->right));
    y->height = 1 + (getHeight(y->left) > getHeight(y->right) ? getHeight(y->left) : getHeight(y->right));
    return y;
}

ThemeNode* ThemeAVLTree::insertNode(ThemeNode* node, const Theme& theme) {
    if (node == nullptr) {
        treeSize++;
        return new ThemeNode(theme);
    }
    
    if (theme.getThemeId() < node->theme.getThemeId()) {
        node->left = insertNode(node->left, theme);
    } else if (theme.getThemeId() > node->theme.getThemeId()) {
        node->right = insertNode(node->right, theme);
    } else {
        return node; 
    }
    
    node->height = 1 + (getHeight(node->left) > getHeight(node->right) ? getHeight(node->left) : getHeight(node->right));
    int balance = getBalanceFactor(node);
    
    
    if (balance > 1 && theme.getThemeId() < node->left->theme.getThemeId()) {
        return rotateRight(node);
    }
    
    if (balance < -1 && theme.getThemeId() > node->right->theme.getThemeId()) {
        return rotateLeft(node);
    }
    
    if (balance > 1 && theme.getThemeId() > node->left->theme.getThemeId()) {
        node->left = rotateLeft(node->left);
        return rotateRight(node);
    }
    
    if (balance < -1 && theme.getThemeId() < node->right->theme.getThemeId()) {
        node->right = rotateRight(node->right);
        return rotateLeft(node);
    }
    
    return node;
}

ThemeNode* ThemeAVLTree::searchNode(ThemeNode* node, int themeId) {
    if (node == nullptr) return nullptr;
    
    if (themeId == node->theme.getThemeId()) {
        return node;
    } else if (themeId < node->theme.getThemeId()) {
        return searchNode(node->left, themeId);
    } else {
        return searchNode(node->right, themeId);
    }
}

ThemeNode* ThemeAVLTree::searchNodeByName(ThemeNode* node, const std::string& name) {
    if (node == nullptr) return nullptr;
    
    if (name == node->theme.getThemeName()) {
        return node;
    }
    
    ThemeNode* leftResult = searchNodeByName(node->left, name);
    if (leftResult != nullptr) return leftResult;
    
    return searchNodeByName(node->right, name);
}

void ThemeAVLTree::inOrderTraversal(ThemeNode* node, Theme** themes, int& index) {
    if (node == nullptr) return;
    
    inOrderTraversal(node->left, themes, index);
    themes[index] = new Theme(node->theme);
    index++;
    inOrderTraversal(node->right, themes, index);
}

void ThemeAVLTree::countNodes(ThemeNode* node, int& count) {
    if (node == nullptr) return;
    count++;
    countNodes(node->left, count);
    countNodes(node->right, count);
}

void ThemeAVLTree::deleteTree(ThemeNode* node) {
    if (node == nullptr) return;
    deleteTree(node->left);
    deleteTree(node->right);
    delete node;
}

void ThemeAVLTree::insert(const Theme& theme) {
    root = insertNode(root, theme);
}

Theme* ThemeAVLTree::search(int themeId) {
    ThemeNode* node = searchNode(root, themeId);
    return (node != nullptr) ? &node->theme : nullptr;
}

Theme* ThemeAVLTree::searchByName(const std::string& name) {
    ThemeNode* node = searchNodeByName(root, name);
    return (node != nullptr) ? new Theme(node->theme) : nullptr;
}

Theme* ThemeAVLTree::getThemesInOrder(int& count) {
    count = getTreeSize();
    if (count == 0) return nullptr;
    
    Theme** tempArray = new Theme*[count];
    int index = 0;
    inOrderTraversal(root, tempArray, index);
    
    Theme* result = new Theme[count];
    for (int i = 0; i < count; i++) {
        result[i] = *tempArray[i];
        delete tempArray[i];
    }
    delete[] tempArray;
    return result;
}

int ThemeAVLTree::getTreeSize() {
    return treeSize;
}