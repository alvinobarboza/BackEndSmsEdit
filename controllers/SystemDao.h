#pragma once
#include <iostream>
#include <string>
#include <json/json.h>
#include <drogon/drogon.h>

using namespace drogon;

class Dao
{
    public:
        Json::Value login(Json::Value &request);
        Json::Value saveUser(Json::Value &request);
        Json::Value updateUser(Json::Value &request);
        Json::Value deleteUser(Json::Value &request);
        Json::Value saveVendor(Json::Value &request);
        Json::Value updateVendor(Json::Value &request);
        Json::Value deleteVendor(Json::Value &request);
        Json::Value getUsers();
        Json::Value searchUsers(Json::Value &request);
        Json::Value searchVendors(Json::Value &request);
        Json::Value getVendors();
        bool checkExistingUser(Json::Value &user);
        bool checkExistingVendor(Json::Value &vendor);
    private:
        Json::Value teste {""};
};