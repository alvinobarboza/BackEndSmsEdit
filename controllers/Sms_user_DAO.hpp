#pragma once
#include <iostream>
#include <string>
#include <json/json.h>
#include <drogon/drogon.h>

using namespace drogon;

class SmsDAO 
{
    public:
        Json::Value saveUser(Json::Value &request);
        Json::Value updateUser(Json::Value &request);
        Json::Value deleteUser(Json::Value &request);
        Json::Value getUsers();
        Json::Value getUserById(std::string &id);
        Json::Value getUserByMotvId(std::string &id);
        Json::Value searchUsers(Json::Value &request);
};
