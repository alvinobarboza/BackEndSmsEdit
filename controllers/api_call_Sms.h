#pragma once
#include <drogon/HttpController.h>
using namespace drogon;
namespace api
{
  namespace call
  {
    class Sms:public drogon::HttpController<Sms>
    {
      public:
        METHOD_LIST_BEGIN
        //use METHOD_ADD to add your custom processing function here;
        //METHOD_ADD(Sms::get,"/{2}/{1}",Get);//path is /api/call/Sms/{arg2}/{arg1}
        //METHOD_ADD(Sms::your_method_name,"/{1}/{2}/list",Get);//path is /api/call/Sms/{arg1}/{arg2}/list
        //ADD_METHOD_TO(Sms::your_method_name,"/absolute/path/{1}/{2}/list",Get);//path is /absolute/path/{arg1}/{arg2}/list

        METHOD_LIST_END
        // your declaration of processing function maybe like this:
        // void get(const HttpRequestPtr& req,std::function<void (const HttpResponsePtr &)> &&callback,int p1,std::string p2);
        // void your_method_name(const HttpRequestPtr& req,std::function<void (const HttpResponsePtr &)> &&callback,double p1,int p2) const;
    };
  }
}
