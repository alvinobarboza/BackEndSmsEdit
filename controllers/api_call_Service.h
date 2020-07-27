#pragma once
#include <drogon/HttpController.h>
#include <drogon/drogon.h>
#include <iostream>
#include <future>

using namespace drogon;
namespace api
{
    namespace call
    {
        class Service:public drogon::HttpController<Service>
        {
        public:
            METHOD_LIST_BEGIN
                METHOD_ADD(Service::login,"/login?user={1}&passwd={2}",Post);
                METHOD_ADD(Service::saveUser,"/saveUser?username={1}&password={2}&vendor={3}",Post);
                METHOD_ADD(Service::updateUser,"/updateUser?id={1}&username={2}&password={3}&vendor={4}",Put);
                METHOD_ADD(Service::deleteUser,"/deleteUser?id={1}",Delete);
                METHOD_ADD(Service::saveVendor,"/saveVendor?vendorname={1}&url={2}",Post);
                METHOD_ADD(Service::updateVendor,"/updateVendor?id={1}&vendorname={2}&url={3}",Put);
                METHOD_ADD(Service::deleteVendor,"/deleteVendor?id={1}",Delete);
                METHOD_ADD(Service::getVendors,"/getVendors",Get);
                METHOD_ADD(Service::getUsers,"/getUsers",Get);
            //    METHOD_ADD(Service::getUserSMS,"/getUserSMS?id={1}",Get);
            METHOD_LIST_END
            
            void login(const HttpRequestPtr &req,
                            std::function<void (const HttpResponsePtr &)> &&callback,
                            std::string &&userId,
                            const std::string &password);
                        
            void saveUser(const HttpRequestPtr &req,
                            std::function<void (const HttpResponsePtr &)> &&callback,
                            const std::string &username,
                            const std::string &password,
                            const std::string &vendor) const;

            void updateUser(const HttpRequestPtr &req,
                            std::function<void (const HttpResponsePtr &)> &&callback,
                            const std::string &id,
                            const std::string &username,
                            const std::string &password,
                            const std::string &vendor) const;

            void deleteUser(const HttpRequestPtr &req,
                            std::function<void (const HttpResponsePtr &)> &&callback,
                            const std::string &id) const;

            void saveVendor(const HttpRequestPtr &req,
                            std::function<void (const HttpResponsePtr &)> &&callback,
                            const std::string &vendorname,
                            const std::string &url) const;

            void updateVendor(const HttpRequestPtr &req,
                            std::function<void (const HttpResponsePtr &)> &&callback,
                            const std::string &id,
                            const std::string &vendorname,
                            const std::string &url) const;

            void deleteVendor(const HttpRequestPtr &req,
                            std::function<void (const HttpResponsePtr &)> &&callback,
                            const std::string &id) const;

            void getUsers(const HttpRequestPtr &req,
                            std::function<void (const HttpResponsePtr &)> &&callback) const;

            void getVendors(const HttpRequestPtr &req,
                            std::function<void (const HttpResponsePtr &)> &&callback) const;

            // void getUserSMS(const HttpRequestPtr &req,
            //                 std::function<void (const HttpResponsePtr &)> &&callback,
            //                 const std::string &id) const;                
        };
    }
}
