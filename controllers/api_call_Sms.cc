#include "api_call_Sms.h"
using namespace api::call;

//=====================================test get API =============================
void Sms::searchUserSMS(const HttpRequestPtr &req,
                            std::function<void (const HttpResponsePtr &)> &&callback)const
{
    LOG_DEBUG << "Search SMS";
    Urls urls;

	std::string tempVendor, path, secret, user_token, url;   
    Json::Value json, dataJ, searchJ, wild_search, search, temp, request;
    Json::Reader reader; 

    reader.parse(std::string{req->getBody()}, search);
    
    std::cout << req->getBody() << std::endl;

	//-------------------verification for empty body---------------------
    if(req->getBody()==""||req->getBody()=="undefined")
    {
        json["response"] = "Não Há informações no body";  
    }
	//-------------------verification for empty fields---------------------
    else if(search["vendor"].asString() == ""||search["vendor"].asString() == "undefined")
    {
        json["response"] = "Campo vendor vazio!"; 
        
        LOG_DEBUG << "Search SMS" << req->getBody();
    }
    else
    {   
        tempVendor = search["vendor"].asString();
        temp = getCredentials(tempVendor);
        
        path = urls.wildSearch;
        url = temp["url"].asString();
        secret = temp["token"].asString();
        user_token = temp["user"].asString();
        
        wild_search["wild_search"] = search["search"];
        searchJ["search"] = wild_search;
        dataJ["data"]=searchJ;

        LOG_DEBUG;
        json = smsCall(url, path, user_token, secret, dataJ);  
    }
    auto resp=HttpResponse::newHttpJsonResponse(json);
    callback(resp);    
}

void Sms::UpdateSMS(const HttpRequestPtr &req,
                            std::function<void (const HttpResponsePtr &)> &&callback)const
{
    LOG_DEBUG << "Update SMS";
	Urls urls;

	Json::Value json, temp, response, request;
    std::string tempVendor, path, secret, user_token, url;	
    Json::Reader reader; 
    
    reader.parse(std::string{req->getBody()}, json);
    temp = json;
    
 //-------------------verification for empty body---------------------
    if(req->getBody() == ""||req->getBody()=="undefined")
    {
        response["response"]= "Não Há informações no body";
    }
 //-------------------verification for empty fields---------------------
    else if(temp["vendor"].asString() == ""||temp["vendor"].asString() == "undefined")
    {
        response["WrongData"] = json;
        response["response"] = "Algum campo vazio!";
    }
    else
    {
        tempVendor = json["vendor"].asString();
        temp = getCredentials(tempVendor);

        path = urls.updateUser;
        url = temp["url"].asString();
        secret = temp["token"].asString();
        user_token = temp["user"].asString();
        request["data"] = json["data"]; 
        
        response = smsCall(url, path, user_token, secret, request);
    }  
    auto resp=HttpResponse::newHttpJsonResponse(response);
    callback(resp);    
}

void Sms::createSMS(const HttpRequestPtr &req,
                            std::function<void (const HttpResponsePtr &)> &&callback)const
{
    LOG_DEBUG << "Create SMS";
	Urls urls;

	Json::Value json, temp, response, request;
    std::string tempVendor, path, secret, user_token, url;	
    Json::Reader reader; 
    
    reader.parse(std::string{req->getBody()}, json);
    temp = json;
    
 //-------------------verification for empty body---------------------
    if(req->getBody() == ""||req->getBody()=="undefined")
    {
        response["response"]= "Não Há informações no body";
    }
 //-------------------verification for empty fields---------------------
    else if(temp["vendor"].asString() == ""||temp["vendor"].asString() == "undefined")
    {
        response["WrongData"] = json;
        response["response"] = "Algum campo vazio!";
    }
    else
    {
        tempVendor = json["vendor"].asString();
        temp = getCredentials(tempVendor);

        path = urls.createUser;
        url = temp["url"].asString();
        secret = temp["token"].asString();
        user_token = temp["user"].asString();
        request["data"] = json["data"]; 
        
        response = smsCall(url, path, user_token, secret, request);
    }  
    auto resp=HttpResponse::newHttpJsonResponse(response);
    callback(resp);    
}

void Sms::getUserSMS(const HttpRequestPtr &req,
                            std::function<void (const HttpResponsePtr &)> &&callback) const
{
    LOG_DEBUG << "Get SMS User";
    Urls urls;

    Json::Value json, temp, response, request;
    Json::Reader reader; 
    std::string tempVendor, path, secret, user_token, url;

    reader.parse(std::string{req->getBody()}, json);
    temp = json;

 //-------------------verification for empty body---------------------
    if(req->getBody() == ""||req->getBody()=="undefined")
    {
        response["response"]= "Não Há informações no body";
    }
 //-------------------verification for empty fields---------------------
    else if(temp["vendor"].asString()==""||
            temp["id"].asString()==""||temp["vendor"].asString()=="undefined")
    {
        response["response"] = "Algum campo vazio!";
        response["WrongData"] = json;
    }
    else 
    {  //-------------------verification if url match existing url on database---------------------
        tempVendor = json["vendor"].asString();
        temp = getCredentials(tempVendor);
        path = urls.getUSer;
        url = temp["url"].asString();
        secret = temp["token"].asString();
        user_token = temp["user"].asString();
        reader.parse("{\"data\":{\"viewers_id\":"+json["id"].asString()+"}}", request);

        response = smsCall(url, path, user_token, secret, request);         
    }  
    auto resp=HttpResponse::newHttpJsonResponse(response);
    callback(resp);    
}

void Sms::subscribeSMS(const HttpRequestPtr &req,
                          std::function<void (const HttpResponsePtr &)> &&callback) const
{
    LOG_DEBUG;
    Json::Value json, response, temp, request;
    Json::Reader reader;
    Urls urls;

    std::string tempVendor, path, secret, user_token, url;

    reader.parse(std::string{req->getBody()}, json);
    temp = json;
    //-------------------verification for empty body---------------------
    if(req->getBody() == ""||req->getBody()=="undefined")
    {
        response["response"]= "Não Há informações no body";
    }
 //-------------------verification for empty fields---------------------
    else if(temp["vendor"].asString()==""||
            temp["id_user"].asString()==""||
            temp["id_product"].asString()==""||
            temp["vendor"].asString()=="undefined")
    {
        response["response"] = "Algum campo vazio!";
        response["WrongData"] = json;
    }
    else 
    {  //-------------------verification if url match existing url on database---------------------
        tempVendor = json["vendor"].asString();
        temp = getCredentials(tempVendor);
        path = urls.subscribeUser;
        url = temp["url"].asString();
        secret = temp["token"].asString();
        user_token = temp["user"].asString();
        reader.parse("{\"data\":{\"viewers_id\":"+json["id_user"].asString()+
                        ", \"products_id\":"+json["id_product"].asString()+"}}", request);

        response = smsCall(url, path, user_token, secret, request);         
    }  

    auto resp=HttpResponse::newHttpJsonResponse(response);
    callback(resp);
}

void Sms::cancelSubscriptionSMS(const HttpRequestPtr &req,
                          std::function<void (const HttpResponsePtr &)> &&callback) const
{
    LOG_DEBUG;
    Json::Value json, response, temp, request;
    Json::Reader reader;
    Urls urls;

    std::string tempVendor, path, secret, user_token, url;

    reader.parse(std::string{req->getBody()}, json);
    temp = json;
    //-------------------verification for empty body---------------------
    if(req->getBody() == ""||req->getBody()=="undefined")
    {
        response["response"]= "Não Há informações no body";
    }
 //-------------------verification for empty fields---------------------
    else if(temp["vendor"].asString()==""||
            temp["id_user"].asString()==""||
            temp["id_product"].asString()==""||
            temp["vendor"].asString()=="undefined")
    {
        response["response"] = "Algum campo vazio!";
        response["WrongData"] = json;
    }
    else 
    {  //-------------------verification if url match existing url on database---------------------
        tempVendor = json["vendor"].asString();
        temp = getCredentials(tempVendor);
        path = urls.cancelUser;
        url = temp["url"].asString();
        secret = temp["token"].asString();
        user_token = temp["user"].asString();
        reader.parse("{\"data\":{\"viewers_id\":"+json["id_user"].asString()+
                        ", \"products_id\":"+json["id_product"].asString()+"}}", request);

        response = smsCall(url, path, user_token, secret, request);         
    }  

    auto resp=HttpResponse::newHttpJsonResponse(response);
    callback(resp);
}

void Sms::getAllowedProductsSMS(const HttpRequestPtr &req,
                          std::function<void (const HttpResponsePtr &)> &&callback) const
{
    LOG_DEBUG << "Get SMS User";
    Urls urls;

    Json::Value json, temp, response, request;
    Json::Reader reader; 
    std::string tempVendor, path, secret, user_token, url;

    reader.parse(std::string{req->getBody()}, json);
    temp = json;

 //-------------------verification for empty body---------------------
    if(req->getBody() == ""||req->getBody()=="undefined")
    {
        response["response"]= "Não Há informações no body";
    }
 //-------------------verification for empty fields---------------------
    else if(temp["vendor"].asString()==""||
            temp["id"].asString()==""||temp["vendor"].asString()=="undefined")
    {
        response["response"] = "Algum campo vazio!";
        response["WrongData"] = json;
    }
    else 
    {  //-------------------verification if url match existing url on database---------------------
        tempVendor = json["vendor"].asString();
        temp = getCredentials(tempVendor);
        path = urls.getAllowedProductUser;
        url = temp["url"].asString();
        secret = temp["token"].asString();
        user_token = temp["user"].asString();
        reader.parse("{\"data\":{\"viewers_id\":"+json["id"].asString()+"}}", request);

        response = smsCall(url, path, user_token, secret, request);         
    }  
    auto resp=HttpResponse::newHttpJsonResponse(response);
    callback(resp);
}

void Sms::getSubscriptionSMS(const HttpRequestPtr &req,
                          std::function<void (const HttpResponsePtr &)> &&callback) const
{
    LOG_DEBUG << "Get SMS User";
    Urls urls;

    Json::Value json, temp, response, request;
    Json::Reader reader; 
    std::string tempVendor, path, secret, user_token, url;

    reader.parse(std::string{req->getBody()}, json);
    temp = json;

 //-------------------verification for empty body---------------------
    if(req->getBody() == ""||req->getBody()=="undefined")
    {
        response["response"]= "Não Há informações no body";
    }
 //-------------------verification for empty fields---------------------
    else if(temp["vendor"].asString()==""||
            temp["id"].asString()==""||temp["vendor"].asString()=="undefined")
    {
        response["response"] = "Algum campo vazio!";
        response["WrongData"] = json;
    }
    else 
    {  //-------------------verification if url match existing url on database---------------------
        tempVendor = json["vendor"].asString();
        temp = getCredentials(tempVendor);
        path = urls.getSubscriptionInfo;
        url = temp["url"].asString();
        secret = temp["token"].asString();
        user_token = temp["user"].asString();
        reader.parse("{\"data\":{\"viewers_id\":"+json["id"].asString()+"}}", request);

        response = smsCall(url, path, user_token, secret, request);         
    }  
    auto resp = HttpResponse::newHttpJsonResponse(response);
    callback(resp);
}

void Sms::getPortalAvailable(const HttpRequestPtr &req,
                          std::function<void (const HttpResponsePtr &)> &&callback) const
{
    LOG_DEBUG << "Get SMS Portal";
	Urls urls;

	Json::Value json, temp, response, request;
    std::string tempVendor, path, secret, user_token, url;	
    Json::Reader reader; 
    
    reader.parse(std::string{req->getBody()}, json);
    temp = json;
    
 //-------------------verification for empty body---------------------
    if(req->getBody() == ""||req->getBody()=="undefined")
    {
        response["response"]= "Não Há informações no body";
    }
 //-------------------verification for empty fields---------------------
    else if(temp["vendor"].asString() == ""||temp["vendor"].asString() == "undefined")
    {
        response["WrongData"] = json;
        response["response"] = "Algum campo vazio!";
    }
    else
    {
        tempVendor = json["vendor"].asString();
        temp = getCredentials(tempVendor);

        path = urls.getPortalAvailable;
        url = temp["url"].asString();
        secret = temp["token"].asString();
        user_token = temp["user"].asString();
        reader.parse("{\"data\":{}}", request); 
        
        response = smsCall(url, path, user_token, secret, request);
    }  
    auto resp=HttpResponse::newHttpJsonResponse(response);
    callback(resp);
}

Json::Value getCredentials(std::string &vendor)
{
    LOG_DEBUG;
    Json::Value obj;
    auto clientPtr = drogon::app().getDbClient();
    try
    {
        auto r = clientPtr->execSqlSync("SELECT * FROM sms.vendor WHERE upper(ds_vendor_name) = upper($1);", vendor);
        
        int i = 0;
        for (auto const &row : r)
        {
            obj["user"] = row["ds_vendor_user_token"].as<std::string>();
            obj["token"] = row["ds_vendor_token"].as<std::string>();
            obj["url"] = row["ds_vendor_url"].as<std::string>();
            i++;
        }                  
    }
    catch (const drogon::orm::DrogonDbException &e)
    {
        LOG_DEBUG << "catch:" << e.base().what();
        obj["erro"] = "Erro ao listar, verifique os logs do servidor!";
    }
    return obj;
}

Json::Value smsCall(std::string &url, 
                    std::string &path, 
                    std::string &user_token, 
                    std::string &secret, 
                    Json::Value &body)
{
    LOG_DEBUG << "Beginning";
    time_t t = time(0);
    Json::Value response;

    if (url == "")
    {
        response["response"] = "Url vazia";
        LOG_DEBUG << "End";
        return response;
    }    

    Json::Reader reader;
    std::string sTime = std::to_string(t);
    std::string token =  sha1(sTime+user_token+secret);

    std::cout << user_token+":"+sTime+":"+token << std::endl;
   
    auto client = HttpClient::newHttpClient("https://"+url);
    auto requestH = HttpRequest::newHttpRequest();
    //LOG_DEBUG;
    requestH->setMethod(drogon::Post);
    requestH->setPath(path);
    requestH->addHeader("Authorization",user_token+":"+sTime+":"+token);
    requestH->setBody(body.toStyledString());
    //LOG_DEBUG;
    auto a = client->sendRequest(requestH, 1);
    //LOG_DEBUG ;

    //std::cout << a.second->getBody();

    if(a.first != ReqResult::Ok)
    {
        response["response"] = "Problemas durante comunicação até servidor MOTV";
    }
    else
    {
        reader.parse(std::string{a.second->getBody()}, response);
    }
    LOG_DEBUG << "End";
    return response;
}
