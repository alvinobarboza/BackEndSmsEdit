#include "api_call_Sms.h"
using namespace api::call;
//add definition of your processing function here
//=====================================test get API =============================
void Sms::searchUserSMS(const HttpRequestPtr &req,
                            std::function<void (const HttpResponsePtr &)> &&callback)const
{
    LOG_DEBUG << "Search SMS";
   
	std::string urlCompare, path, secret, user_token, url;   
    Json::Value json, dataJ, searchJ, wild_search, search, temp, request;
    Json::Reader reader; 

    reader.parse(std::string{req->getBody()}, search);
    temp = search;
	
	//-------------------verification for empty body---------------------
    if(req->getBody()=="")
    {
        json["response"] = "Não Há informações no body";  
        LOG_DEBUG << "first if";     
    }
	//-------------------verification for empty fields---------------------
    else if(search["search"].asString() == ""||
            search["user_token"].asString() == ""|| 
            search["secret"].asString() == ""|| 
            search["url"].asString() == "")
    {
        json["response"] = "Campos vazios!";
        json["WrongData"] = temp;
        LOG_DEBUG << "second if"; 
    }
    else
    {        
        url = search["url"].asString();
        bool checker = checkUrl(url);

        if(checker)
        {            
            path = "/api/customer/findCustomerForSales";
            url = search["url"].asString();
            secret = search["secret"].asString();
            user_token = search["user_token"].asString();
			
            wild_search["wild_search"] = search["search"];
			searchJ["search"] = wild_search;
			dataJ["data"]=searchJ;
            
            json = smsCall(url, path, user_token, secret, dataJ);  
        }else
        {
            json["response"] = "URL Invávila!";
        }        
    }
    auto resp=HttpResponse::newHttpJsonResponse(json);
    callback(resp);    
}

void Sms::UpdateSMS(const HttpRequestPtr &req,
                            std::function<void (const HttpResponsePtr &)> &&callback)const
{
    LOG_DEBUG << "Update SMS";
	
	Json::Value json, temp, response, request;
    std::string urlCompare, path, secret, user_token, url;	
    Json::Reader reader; 

    reader.parse(std::string{req->getBody()}, json);
    temp = json;

 //-------------------verification for empty body---------------------
    if(req->getBody() == "")
    {
        response["response"]= "Não Há informações no body";
    }
 //-------------------verification for empty fields---------------------
    else if(temp["url"].asString()==""||
            temp["data"].asString()==""||
            temp["user_token"].asString()==""||
            temp["secret"].asString()=="")
    {
        response["WrongData"] = json;
        response["response"] = "Algum campo vazio!";
    }
    else
    {   
        url = json["url"].asString();
        bool checker = checkUrl(url);

        if(checker)
        {
            path = "/api/integration/updateMotvCustomer";
            url = json["url"].asString();
            secret = json["secret"].asString();
            user_token = json["user_token"].asString();
            request["data"] = json["data"]; 
			
            response = smsCall(url, path, user_token, secret, request);  
        }else
        {
            response["response"] = "URL Invávila!";
        }        
    }  
    auto resp=HttpResponse::newHttpJsonResponse(response);
    callback(resp);    
}

void Sms::getUserSMS(const HttpRequestPtr &req,
                            std::function<void (const HttpResponsePtr &)> &&callback) const
{
    LOG_DEBUG << "Get SMS User";

    Json::Value json, temp, response, request;
    Json::Reader reader; 
    std::string urlCompare, path, secret, user_token, url;

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
        response["response"] = "Algum campo vazio!";
        response["WrongData"] = json;
    }
    else 
    {  //-------------------verification if url match existing url on database---------------------
        url = json["url"].asString();
        bool checker = checkUrl(url);

        if(checker)
        {
            path = "/api/customer/getData";
            url = json["url"].asString();
            secret = json["secret"].asString();
            user_token = json["user_token"].asString();
            reader.parse("{\"data\":{\"viewers_id\":"+json["id"].asString()+"}}", request);

            response = smsCall(url, path, user_token, secret, request);  
        }else
        {
            response["response"] = "URL Invávila!";
        }        
    }  
    auto resp=HttpResponse::newHttpJsonResponse(response);
    callback(resp);    
}

Json::Value smsCall(std::string &url, 
                    std::string &path, 
                    std::string &user_token, 
                    std::string &secret, 
                    Json::Value &body)
{
    LOG_DEBUG;
    time_t t = time(0);
    Json::Value response;
    std::string sTime = std::to_string(t);
    std::string token =  sha1(sTime+user_token+secret);
   
    auto client = HttpClient::newHttpClient("https://"+url);
    auto requestH = HttpRequest::newHttpRequest();
    
    requestH->setMethod(drogon::Post);
    requestH->setPath(path);
    requestH->addHeader("Authorization",user_token+":"+sTime+":"+token);
    requestH->addHeader("Content-Type","application/json");    
    requestH->setBody(body.toStyledString());
    
    auto a = client->sendRequest(requestH);    
    response = *a.second->getJsonObject();
    
    return response;
}

bool checkUrl(std::string &url)
{    
    std::string urlCompare;
    bool check = false;

    auto clientPtr = drogon::app().getDbClient();
    try
    {
        auto r = clientPtr->execSqlSync("select * from sms.vendor;");
        for (auto const &row : r)
        {                
            urlCompare = row["ds_vendor_url"].as<std::string>();
            if(url == urlCompare)
                check = true;
        }
    }
    catch (const drogon::orm::DrogonDbException &e)
    {
        LOG_DEBUG << "catch:" << e.base().what();
        url = "Erro ao logar, verifique os logs do servidor!";
        return false;
    }

    return check;
}