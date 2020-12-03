#include "api_call_Sms.h"
using namespace api::call;

// --- integrated --
void Sms::getUserSMS(const HttpRequestPtr &req,
                            std::function<void (const HttpResponsePtr &)> &&callback) const
{
    LOG_DEBUG ;
    Urls urls;

    Json::Value json, temp, response, request;
    Json::Reader reader; 
    std::string tempVendor, path;

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
        auto pair = getCredentials(tempVendor);
        path = urls.getUSer;
        
        std::string id = json["id"].asString();
        reader.parse("{\"data\":{\"viewers_id\":"+id+"}}", request);

        response = smsCall(pair, path, request); 

        if(response["status"].asInt() == 1)
        {
            std::string id = response["response"]["viewers_id"].asString();
            response = integratedUser(response, id);
        }
    }  
    auto resp=HttpResponse::newHttpJsonResponse(response);
    callback(resp);    
}

// --- integrated --
void Sms::searchUserSMS(const HttpRequestPtr &req,
                            std::function<void (const HttpResponsePtr &)> &&callback)const
{
    LOG_DEBUG ;
    Urls urls;

	std::string tempVendor, path;   
    Json::Value response, dataJ, searchJ, wild_search, search, temp;
    Json::Reader reader; 

    reader.parse(std::string{req->getBody()}, search);
    
    std::cout << req->getBody() << std::endl;

	//-------------------verification for empty body---------------------
    if(req->getBody()==""||req->getBody()=="undefined")
    {
        response["response"] = "Não Há informações no body";  
    }
	//-------------------verification for empty fields---------------------
    else if(search["vendor"].asString() == ""||search["vendor"].asString() == "undefined")
    {
        response["response"] = "Campo vendor vazio!"; 
        
        LOG_DEBUG << "Search SMS" << req->getBody();
    }
    else
    {   
        tempVendor = search["vendor"].asString();
        auto pair = getCredentials(tempVendor);      
        path = urls.wildSearch;
                
        wild_search["wild_search"] = search["search"];
        searchJ["search"] = wild_search;
        dataJ["data"]=searchJ;

        temp = smsCall(pair, path, dataJ); 

        response = integratedSearch(temp);
    }
    //LOG_DEBUG << response.toStyledString();
    auto resp=HttpResponse::newHttpJsonResponse(response);
    callback(resp);    
}

// --- integrated --
void Sms::createSMS(const HttpRequestPtr &req,
                            std::function<void (const HttpResponsePtr &)> &&callback)const
{
    LOG_DEBUG ;
	Urls urls;

	Json::Value json, temp, response, request;
    std::string tempVendor, path;	
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
        auto pair = getCredentials(tempVendor);
        path = urls.createUser;        
        request["data"] = json["data"];         
        response = smsCall(pair, path, request);

        if(response["status"].asInt() == 1)
        {
            int id = response["response"].asInt();
            response = integratedCreate(json["data"], id);
            LOG_DEBUG;
        }
        LOG_DEBUG;        
    }  
    auto resp=HttpResponse::newHttpJsonResponse(response);
    callback(resp);    
}

// --- integrated --
void Sms::UpdateSMS(const HttpRequestPtr &req,
                            std::function<void (const HttpResponsePtr &)> &&callback)const
{
    LOG_DEBUG ;
	Urls urls;

	Json::Value json, temp, response, request;
    std::string tempVendor, path;	
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
        auto pair = getCredentials(tempVendor);
        path = urls.updateUser;        
        request["data"] = json["data"]; 
        
        response = smsCall(pair, path, request);

        if(response["status"].asInt() == 1)
        {
            int id = response["response"].asInt();
            response = integratedUpdate(json["data"], id);
            LOG_DEBUG;
        }
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

    std::string tempVendor, path;

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
        auto pair = getCredentials(tempVendor);
        path = urls.subscribeUser;
        
        reader.parse("{\"data\":{\"viewers_id\":"+json["id_user"].asString()+
                        ", \"products_id\":"+json["id_product"].asString()+"}}", request);

        response = smsCall(pair, path, request);         
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

    std::string tempVendor, path;

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
        auto pair = getCredentials(tempVendor);
        path = urls.cancelUser;
        
        reader.parse("{\"data\":{\"viewers_id\":"+json["id_user"].asString()+
                        ", \"products_id\":"+json["id_product"].asString()+"}}", request);

        response = smsCall(pair, path, request);         
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
    std::string tempVendor, path;

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
        auto pair = getCredentials(tempVendor);
        path = urls.getAllowedProductUser;
        
        reader.parse("{\"data\":{\"viewers_id\":"+json["id"].asString()+"}}", request);

        response = smsCall(pair, path, request);         
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
    std::string tempVendor, path;

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
        auto pair = getCredentials(tempVendor);
        path = urls.getSubscriptionInfo;
        
        reader.parse("{\"data\":{\"viewers_id\":"+json["id"].asString()+"}}", request);

        response = smsCall(pair, path, request);         
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
    std::string tempVendor, path;	
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
        auto pair = getCredentials(tempVendor);

        path = urls.getPortalAvailable;
        
        reader.parse("{\"data\":{}}", request); 
        
        response = smsCall(pair, path, request);
    }  
    auto resp=HttpResponse::newHttpJsonResponse(response);
    callback(resp);
}

std::pair<std::string, std::string> getCredentials(std::string &vendor)
{
    time_t t = time(0);
    Json::Value obj;
    std::string user, secret, url;
    std::pair<std::string, std::string> credentials;
    auto clientPtr = drogon::app().getDbClient();
    try
    {
        auto r = clientPtr->execSqlSync("SELECT * FROM sms.vendor WHERE upper(ds_vendor_name) = upper($1);", vendor);
        
        for (auto const &row : r)
        {
            user = row["ds_vendor_user_token"].as<std::string>();
            secret = row["ds_vendor_token"].as<std::string>();
            url = row["ds_vendor_url"].as<std::string>();
        }                  
    }
    catch (const drogon::orm::DrogonDbException &e)
    {
        LOG_DEBUG << "catch:" << e.base().what();
        obj["erro"] = "Erro ao listar, verifique os logs do servidor!";
    }

    std::string sTime = std::to_string(t);
    std::string sha =  sha1(sTime+user+secret);
    std::string token = user+":"+sTime+":"+sha;
    std::cout << user+":"+sTime+":"+sha << std::endl;

    credentials.first = token;
    credentials.second = url;

    return credentials;
}

Json::Value smsCall(std::pair<std::string, std::string> &credendials,
                    std::string &path,                      
                    Json::Value &body)
{
    Json::Value response;

    if (credendials.second == "")
    {
        response["response"] = "Url vazia";
        LOG_DEBUG << "End";
        return response;
    }    
    
    auto client = HttpClient::newHttpClient("https://"+credendials.second);
    auto requestH = HttpRequest::newHttpRequest();
    //LOG_DEBUG;
    requestH->setMethod(drogon::Post);
    requestH->setPath(path);
    requestH->addHeader("Authorization",credendials.first);
    requestH->setBody(body.toStyledString());
    //LOG_DEBUG << body.toStyledString();
    auto a = client->sendRequest(requestH, 1);
    //LOG_DEBUG ;

    //std::cout << a.second->getBody();

    if(a.first != ReqResult::Ok)
    {
        response["response"] = "Problemas durante comunicação até servidor MOTV";
    }
    else
    {   
        Json::Reader reader;     
        reader.parse(std::string{a.second->getBody()}, response);
        //LOG_DEBUG << "End";
    }
    return response;
}

Json::Value integratedSearch(Json::Value &searchResult)
{
    LOG_DEBUG;
    if(searchResult["status"].asInt() == 1)
    {
        SmsDAO dao;
        std::string id;
        Json::Value returnResult, unit;
        int verification = searchResult["response"].size();

        if(verification)
        {
            for (Json::Value &user : searchResult["response"])
            {            
                id = user["viewers_id"].asString();
                Json::Value temp = dao.getUserByMotvId(id);
                // LOG_DEBUG ;
                if(temp["0"].empty())
                {
                    // LOG_DEBUG ;
                    unit["userid"]      = "";
                    unit["userSMSid"]   = id;
                    unit["profilename"] = user["devices"][0]["motv_portals_name"];
                    unit["name"]        = user["viewers_firstname"];
                    unit["lastname"]    = user["viewers_lastname"];
                    unit["login"]       = user["devices"][0]["device_motv_login"];
                    unit["birthdate"]   = "";
                    unit["email"]       = "";
                    unit["tel1"]        = "";
                    unit["tel2"]        = "";

                    returnResult[id] = unit;
                }
                else
                {
                    returnResult[id] = temp["0"];
                    // LOG_DEBUG ;
                }
            }
            returnResult["searchStatus"] = 1;
        }
        else
        {
            returnResult["searchStatus"] = 0;
        }
        
        return returnResult;
    }
    else
    {
        // LOG_DEBUG ;
        std::cout << searchResult;
        return searchResult;
    }
}

Json::Value integratedCreate(Json::Value &user, int & id)
{
    SmsDAO dao;
    user["smsID"] = id;
    Json::Value response = dao.saveUser(user);
    LOG_DEBUG;
    if(response["response"].asInt())
    {
        response["response"] = "Criado com sucesso!";
    }
    else
    {
        response["response"] = "Erro ao criar, contate suporte!";
    }
    return response;
}

Json::Value integratedUpdate(Json::Value &user, int & id)
{
    SmsDAO dao;
    user["smsID"] = id;
    Json::Value response = dao.updateUser(user);
    
    if(response["response"].asInt())
    {
        response["response"] = "Atualizado com sucesso!";
    }
    else
    {
        response["response"] = "Erro ao criar, contate suporte!";
    }
    return response;
}

Json::Value integratedUser(Json::Value &user, std::string & id)
{
    SmsDAO dao;
    Json::Value response, temp;
    temp = dao.getUserByMotvId(id);

    if(temp["0"].empty())
    {
        response["userid"]      = "";
        response["userSMSid"]   = id;
        response["profilename"] = user["response"]["devices"][0]["motv_portals_name"];
        response["name"]        = user["response"]["viewers_firstname"];
        response["lastname"]    = user["response"]["viewers_lastname"];
        response["login"]       = user["response"]["devices"][0]["device_motv_login"];
        response["birthdate"]   = "";
        response["email"]       = "";
        response["tel1"]        = "";
        response["tel2"]        = "";

    }
    else
    {
        response = temp;
    }
    return response;
}

