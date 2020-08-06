#include "api_call_Sms.h"
using namespace api::call;
//add definition of your processing function here
//=====================================test get API =============================
void Sms::getUserSMS(const HttpRequestPtr &req,
                             std::function<void (const HttpResponsePtr &)> &&callback,
                             const std::string &search)const
{
    LOG_DEBUG << "get SMS";
   
    Json::Value json, dataJ, searchJ, wild_search;

    wild_search["wild_search"] = search;
    searchJ["search"] = wild_search;
    dataJ["data"]=searchJ;

    time_t t = time(0);
    std::string sTime = std::to_string(t);
    std::string login = "alvino.barboza@youcast.tv.br", secret = "cm3yaca6xr37xp5d0b10vw6d8yhcwd9zk1b7o0be";
    std::string token =  sha1(sTime+login+secret);
        
    //std::cout << dataJ.toStyledString() <<"\n end Json "<< std::endl;
    //std::cout <<"\n token "<<"alvino.barboza@youcast.tv.br:"+sTime+":"+token << std::endl;

    auto client = HttpClient::newHttpClient("https://sms.youcast.tv.br");
    auto requestH = HttpRequest::newHttpRequest();

    requestH->setMethod(drogon::Post);
    requestH->setPath("/api/customer/findCustomerForSales");
    requestH->addHeader("Authorization","alvino.barboza@youcast.tv.br:"+sTime+":"+token);
    requestH->addHeader("Content-Type","application/json");    
    requestH->setBody(dataJ.toStyledString());
        
    auto a = client->sendRequest(requestH);

    json = *a.second->getJsonObject();
    auto resp=HttpResponse::newHttpJsonResponse(json);
    callback(resp);
    
}