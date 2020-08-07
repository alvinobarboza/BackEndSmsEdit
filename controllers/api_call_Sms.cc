#include "api_call_Sms.h"
using namespace api::call;
//add definition of your processing function here
//=====================================test get API =============================
void Sms::getUserSMS(const HttpRequestPtr &req,
                             std::function<void (const HttpResponsePtr &)> &&callback)const
{
    LOG_DEBUG << "get SMS";
   
    Json::Value json, dataJ, searchJ, wild_search, search, temp;
    Json::Reader reader; 

    reader.parse(std::string{req->getBody()}, search);
    temp = search;

    if(req->getBody()=="")
    {
        json["response"] = "Não Há informações no body";

        //std::cout<<"test1"<<std::endl;
    }
    else if(search["search"].asString() == ""||search["user_token"].asString() == ""|| search["secret"].asString() == ""|| search["url"].asString() == "")
    {
        json["response"] = "Campos vazios!";

        json["WrongData"] = temp;
        
        //std::cout<<"test2"<<std::endl;
    }
    else
    {
       // std::cout<<"test3"<<std::endl;
        
        wild_search["wild_search"] = search["search"];
        searchJ["search"] = wild_search;
        dataJ["data"]=searchJ;

        time_t t = time(0);
        std::string sTime = std::to_string(t);
        std::string login = search["user_token"].asString(), secret = search["secret"].asString();
        std::string token =  sha1(sTime+login+secret);
            
        std::cout << dataJ.toStyledString() <<"\n end Json "<< std::endl;
        //std::cout <<"\n token "<<"alvino.barboza@youcast.tv.br:"+sTime+":"+token << std::endl;

        auto client = HttpClient::newHttpClient("https://"+search["url"].asString());
        auto requestH = HttpRequest::newHttpRequest();

        requestH->setMethod(drogon::Post);
        requestH->setPath("/api/customer/findCustomerForSales");
        requestH->addHeader("Authorization",login+":"+sTime+":"+token);
        requestH->addHeader("Content-Type","application/json");    
        requestH->setBody(dataJ.toStyledString());
            
        auto a = client->sendRequest(requestH);

        json = *a.second->getJsonObject();
    }

    auto resp=HttpResponse::newHttpJsonResponse(json);
    callback(resp);
    
}

void Sms::UpdateSMS(const HttpRequestPtr &req,
                             std::function<void (const HttpResponsePtr &)> &&callback)const
{
    LOG_DEBUG << "Update SMS";

    Json::Value json;
    Json::Reader reader; 

    reader.parse(std::string{req->getBody()}, json);

    if(req->getBody() == "")
    {
        json["response"]= "Não Há informações no body";
    }
    
    // time_t t = time(0);
    // std::string sTime = std::to_string(t);
    // std::string login = search["user_token"].asString(), secret = search["secret"].asString();
    // std::string token =  sha1(sTime+login+secret);
        
    // std::cout << json.toStyledString() <<"\n end Json "<< std::endl;
    // //std::cout <<"\n token "<<"alvino.barboza@youcast.tv.br:"+sTime+":"+token << std::endl;

    // auto client = HttpClient::newHttpClient("https://"+search["url"].asString());
    // auto requestH = HttpRequest::newHttpRequest();

    // requestH->setMethod(drogon::Post);
    // requestH->setPath("/api/integration/updateMotvCustomer");
    // requestH->addHeader("Authorization",login+":"+sTime+":"+token);
    // requestH->addHeader("Content-Type","application/json");    
    // requestH->setBody(json.toStyledString());
        
    // auto a = client->sendRequest(requestH);

    // json = *a.second->getJsonObject();    

    std::cout << json << std::endl;

    auto resp=HttpResponse::newHttpJsonResponse(json);
    callback(resp);
    
}