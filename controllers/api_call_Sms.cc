#include "api_call_Sms.h"
using namespace api::call;
//add definition of your processing function here
//=====================================test get API =============================
void Sms::searchUserSMS(const HttpRequestPtr &req,
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
    }
    else if(search["search"].asString() == ""||
            search["user_token"].asString() == ""|| 
            search["secret"].asString() == ""|| 
            search["url"].asString() == "")
    {
        json["response"] = "Campos vazios!";
        json["WrongData"] = temp; 
    }
    else
    {        
        wild_search["wild_search"] = search["search"];
        searchJ["search"] = wild_search;
        dataJ["data"]=searchJ;

        time_t t = time(0);
        std::string sTime = std::to_string(t);
        std::string login = search["user_token"].asString(), secret = search["secret"].asString();
        std::string token =  sha1(sTime+login+secret);
            
        std::cout << dataJ.toStyledString() <<"\n end Json "<< std::endl;

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

    std::string tempS;
    Json::Value json, temp, response, request;
    Json::Reader reader; 

    reader.parse(std::string{req->getBody()}, json);
    temp = json;


    if(req->getBody() == "")
    {
        response["response"]= "Não Há informações no body";
    }
    else if(temp["url"].asString()==""||
            temp["data"].asString()==""||
            temp["user_token"].asString()==""||
            temp["secret"].asString()=="")
    {
        std::cout<<temp<<std::endl;
        response["WrongData"] = json;
        response["response"] = "Algum campo vazio!";
    }
    else
    {   
        request["data"] = json["data"];
        
        time_t t = time(0);
        std::string sTime = std::to_string(t);
        std::string login = json["user_token"].asString(), secret = json["secret"].asString();
        std::string token =  sha1(sTime+login+secret);
                
        auto client = HttpClient::newHttpClient("https://"+json["url"].asString());
        auto requestH = HttpRequest::newHttpRequest();

        requestH->setMethod(drogon::Post);
        requestH->setPath("/api/integration/updateMotvCustomer");
        requestH->addHeader("Authorization",login+":"+sTime+":"+token);
        requestH->addHeader("Content-Type","application/json");    
        requestH->setBody(request.toStyledString());
        
        auto a = client->sendRequest(requestH);

        LOG_DEBUG;
        try
        {
            response = *a.second->getJsonObject();    
        }
        catch(std::string &e)
        {
            LOG_DEBUG<<e;
        }
       // response["response"] = tempS;

        std::cout << response << std::endl;
    }  
    LOG_DEBUG;
    auto resp=HttpResponse::newHttpJsonResponse(response);
    callback(resp);
    
}

void Sms::getUserSMS(const HttpRequestPtr &req,
                            std::function<void (const HttpResponsePtr &)> &&callback) const
{
    LOG_DEBUG << "Update SMS";

    Json::Value json, temp, response;
    Json::Reader reader; 
    std::string url;

    reader.parse(std::string{req->getBody()}, json);
    temp = json;

//-------------------verification for empty body---------------------
    if(req->getBody() == "")
    {
        response["response"]= "Não Há informações no body";
    }
//-------------------verification for empty fields---------------------
    else if(temp["url"].asString()==""||
            temp["id"].asString()==""||
            temp["user_token"].asString()==""||
            temp["secret"].asString()=="")
    {
        std::cout<<temp<<std::endl;
        response["WrongData"] = json;
        response["response"] = "Algum campo vazio!";
    }
    else 
    {    
        //-------------------verification if url match existing url on database---------------------
        auto clientPtr = drogon::app().getDbClient();

         try
        {
            auto r = clientPtr->execSqlSync("select * from sms.vendor;");
            for (auto const &row : r)
            {                
                url = row["ds_vendor_url"].as<std::string>();
                if(temp["url"].asString() == url)
                    LOG_DEBUG<<"igual";                
            }
        }
        catch (const drogon::orm::DrogonDbException &e)
        {
            LOG_DEBUG << "catch:" << e.base().what();
            url = "Erro ao logar, verifique os logs do servidor!";
        }

        time_t t = time(0);
        std::string sTime = std::to_string(t);
        std::string login = json["user_token"].asString(), secret = json["secret"].asString();
        std::string token =  sha1(sTime+login+secret);
        
        std::cout << json.toStyledString() <<"\n End Json "<< std::endl;

        auto client = HttpClient::newHttpClient("https://"+json["url"].asString());
        auto requestH = HttpRequest::newHttpRequest();

        requestH->setMethod(drogon::Post);
        requestH->setPath("/api/customer/getData");
        requestH->addHeader("Authorization",login+":"+sTime+":"+token);
        requestH->addHeader("Content-Type","application/json");    
        requestH->setBody(json.toStyledString());
        
        auto a = client->sendRequest(requestH);
        
        response = *a.second->getJsonObject();    


        std::cout << json << std::endl;
    }  

    auto resp=HttpResponse::newHttpJsonResponse(response);
    callback(resp);
    
}