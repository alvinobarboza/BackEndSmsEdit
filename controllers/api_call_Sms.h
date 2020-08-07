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
          METHOD_ADD(Sms::getUserSMS,"/searchUserSMS",Post);
          METHOD_ADD(Sms::UpdateSMS,"/updateUserSMS",Put);
        METHOD_LIST_END
          void getUserSMS(const HttpRequestPtr &req,
                          std::function<void (const HttpResponsePtr &)> &&callback) const;
          void UpdateSMS(const HttpRequestPtr &req,
                          std::function<void (const HttpResponsePtr &)> &&callback) const;
    };
  }
}
