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
          METHOD_ADD(Sms::createSMS,"/createUserSMS",Post);
          METHOD_ADD(Sms::UpdateSMS,"/updateUserSMS",Put);
          METHOD_ADD(Sms::getUserSMS,"/getUserSMS",Post);
          METHOD_ADD(Sms::subscribeSMS,"/subscribeUserSMS",Put);
          METHOD_ADD(Sms::cancelSubscriptionSMS,"/cancelSubscriptionUserSMS",Put);
          METHOD_ADD(Sms::getAllowedProductsSMS,"/getAllowedProductsUserSMS",Post);
          METHOD_ADD(Sms::getSubscriptionSMS,"/getSubscriptionUserSMS",Post);
          METHOD_ADD(Sms::getPortalAvailable,"/getPortalAvailable",Post);
        METHOD_LIST_END
          void searchUserSMS(const HttpRequestPtr &req,
                          std::function<void (const HttpResponsePtr &)> &&callback) const;
          void UpdateSMS(const HttpRequestPtr &req,
                          std::function<void (const HttpResponsePtr &)> &&callback) const;
          void createSMS(const HttpRequestPtr &req,
                          std::function<void (const HttpResponsePtr &)> &&callback) const;
          void getUserSMS(const HttpRequestPtr &req,
                          std::function<void (const HttpResponsePtr &)> &&callback) const;
          void subscribeSMS(const HttpRequestPtr &req,
                          std::function<void (const HttpResponsePtr &)> &&callback) const;
          void cancelSubscriptionSMS(const HttpRequestPtr &req,
                          std::function<void (const HttpResponsePtr &)> &&callback) const;
          void getAllowedProductsSMS(const HttpRequestPtr &req,
                          std::function<void (const HttpResponsePtr &)> &&callback) const;
          void getSubscriptionSMS(const HttpRequestPtr &req,
                          std::function<void (const HttpResponsePtr &)> &&callback) const;
          void getPortalAvailable(const HttpRequestPtr &req,
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
    std::string subscribeUser = "/api/integration/subscribe";
    std::string cancelUser = "/api/integration/cancel";
    std::string getAllowedProductUser = "/api/sales/getAllowedProductsForCustomer";
    std::string getSubscriptionInfo = "/api/subscription/getCustomerSubscriptionInfo";
    std::string getPortalAvailable = "/api/devices/motv/getWhitelistedPortalPairs";
};