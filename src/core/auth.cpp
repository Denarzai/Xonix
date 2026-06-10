#include "../../include/core/auth.h"
#include <cctype>

Auth::Auth(PlayerManager* pm) {
    manager = pm;
}

bool Auth::isValidUsername(const string& username) {
    if (username.length() < 3) return false;
    for (int i = 0; i < username.length(); i++) {
        char c = username[i];
        if (!isalnum(c) && c != '_') return false;
    }
    if (manager->getPlayerByUsername(username)) return false;
    return true;
}

bool Auth::isValidPassword(const string& password) {
    if (password.length() < 6) return false;
    bool hasUpper = false, hasLower = false, hasDigit = false;
    for (int i = 0; i < password.length(); i++) {
        char c = password[i];
        if (isupper(c)) hasUpper = true;
        if (islower(c)) hasLower = true;
        if (isdigit(c)) hasDigit = true;
    }
    return hasUpper && hasLower && hasDigit;
}

Player* Auth::registerUser(const string& username, const string& password,
                           const string& nickname, const string& email) {
    if (!isValidUsername(username)) return nullptr;
    if (!isValidPassword(password)) return nullptr;
    if (manager->registerPlayer(username, password, nickname, email))
        return manager->getPlayerByUsername(username);
    return nullptr;
}

Player* Auth::loginUser(const string& username, const string& password) {
    Player* p = manager->getPlayerByUsername(username);
    if (!p) return nullptr;
    if (p->getPassword() != password) return nullptr;
    return p;
}
