#pragma once
#include <drogon/HttpController.h>
#include <drogon/drogon.h>
#include <iostream>
#include <future>
#include "Dao.h"

using namespace drogon;
namespace api
{
    namespace call
    {
        class Service:public drogon::HttpController<Service>
        {
        public:
            METHOD_LIST_BEGIN
                METHOD_ADD(Service::login,"/login",Post);
                METHOD_ADD(Service::saveUser,"/saveUser",Post);
                METHOD_ADD(Service::updateUser,"/updateUser",Put);
                METHOD_ADD(Service::deleteUser,"/deleteUser",Delete);
                METHOD_ADD(Service::saveVendor,"/saveVendor",Post);
                METHOD_ADD(Service::updateVendor,"/updateVendor",Put);
                METHOD_ADD(Service::deleteVendor,"/deleteVendor",Delete);
                METHOD_ADD(Service::getVendors,"/getVendors",Get);
                METHOD_ADD(Service::getUsers,"/getUsers",Get);
                METHOD_ADD(Service::searchVendors,"/getVendors",Post);
                METHOD_ADD(Service::searchUsers,"/getUsers",Post);                         
            METHOD_LIST_END
            
            void login(const HttpRequestPtr &req,
                            std::function<void (const HttpResponsePtr &)> &&callback);
                        
            void saveUser(const HttpRequestPtr &req,
                            std::function<void (const HttpResponsePtr &)> &&callback) const;

            void updateUser(const HttpRequestPtr &req,
                            std::function<void (const HttpResponsePtr &)> &&callback) const;

            void deleteUser(const HttpRequestPtr &req,
                            std::function<void (const HttpResponsePtr &)> &&callback) const;

            void saveVendor(const HttpRequestPtr &req,
                            std::function<void (const HttpResponsePtr &)> &&callback) const;

            void updateVendor(const HttpRequestPtr &req,
                            std::function<void (const HttpResponsePtr &)> &&callback) const;

            void deleteVendor(const HttpRequestPtr &req,
                            std::function<void (const HttpResponsePtr &)> &&callback) const;

            void getUsers(const HttpRequestPtr &req,
                            std::function<void (const HttpResponsePtr &)> &&callback) const;

            void getVendors(const HttpRequestPtr &req,
                            std::function<void (const HttpResponsePtr &)> &&callback) const;

            void searchUsers(const HttpRequestPtr &req,
                            std::function<void (const HttpResponsePtr &)> &&callback) const;

            void searchVendors(const HttpRequestPtr &req,
                            std::function<void (const HttpResponsePtr &)> &&callback) const;                
        };
    }
}
