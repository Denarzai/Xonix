#pragma once
#include "playerManager.h"
#include <string>

class Auth {
private:
    PlayerManager* manager;
    bool isValidUsername(const string& username);
    bool isValidPassword(const string& password);

public:
    Auth(PlayerManager* pm);
    Player* registerUser(const string& username, const string& password,
                         const string& nickname = "", const string& email = "");
    Player* loginUser(const string& username, const string& password);
};
