#pragma once
#include <drogon/HttpController.h>
#include <drogon/drogon.h>
#include <iostream>
#include <future>
#include <thread>         
#include <chrono>
#include "sha1.h"
using namespace drogon;
namespace api
{
  namespace call
  {
    class Sms:public drogon::HttpController<Sms>
    {
      public:
        METHOD_LIST_BEGIN
          METHOD_ADD(Sms::searchUserSMS,"/searchUserSMS",Post);
          METHOD_ADD(Sms::createSMS,"/createUserSMS",Put);
          METHOD_ADD(Sms::UpdateSMS,"/updateUserSMS",Post);
          METHOD_ADD(Sms::getUserSMS,"/getUserSMS",Post);
        METHOD_LIST_END
          void searchUserSMS(const HttpRequestPtr &req,
                          std::function<void (const HttpResponsePtr &)> &&callback) const;
          void UpdateSMS(const HttpRequestPtr &req,
                          std::function<void (const HttpResponsePtr &)> &&callback) const;
          void createSMS(const HttpRequestPtr &req,
                          std::function<void (const HttpResponsePtr &)> &&callback) const;
          void getUserSMS(const HttpRequestPtr &req,
                          std::function<void (const HttpResponsePtr &)> &&callback) const;
    };
  }
}
Json::Value getCredentials(std::string &vendor);
Json::Value smsCall(std::string &url, 
                    std::string &path, 
                    std::string &user_token, 
                    std::string &secret, 
                    Json::Value &body);
struct Urls
{
    std::string getUSer = "/api/customer/getData";  
    std::string wildSearch = "/api/customer/findCustomerForSales";
    std::string updateUser = "/api/integration/updateMotvCustomer";
    std::string createUser = "/api/integration/createMotvCustomer";
};