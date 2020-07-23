#pragma once
#include <drogon/HttpController.h>
#include <drogon/drogon.h>
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
                METHOD_ADD(Service::saveUser,"/save?id={1}&username={2}&password={3}&vendor={4}",Post);
                METHOD_ADD(Service::updateUser,"/update?id={1}username={2}&password={3}&vendor={4}",Put);
                METHOD_ADD(Service::deleteUser,"/delete?id={1}",Delete);
                METHOD_ADD(Service::getVendors,"/getVendors",Get);
                METHOD_ADD(Service::getUsers,"/getUsers",Get);
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

            void getUsers(const HttpRequestPtr &req,
                            std::function<void (const HttpResponsePtr &)> &&callback) const;

            void getVendors(const HttpRequestPtr &req,
                            std::function<void (const HttpResponsePtr &)> &&callback) const;
        };
    }
}
