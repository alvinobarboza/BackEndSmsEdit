#include "api_call_Service.hpp"
using namespace api::call;


//================================simple login method==========================
void Service::login(const HttpRequestPtr &req,
           std::function<void (const HttpResponsePtr &)> &&callback)
{
    Json::Value ret, loginData;
    Json::Reader reader; 
    Dao dao;

    reader.parse(std::string{req->getBody()}, loginData);
    
    if(req->getBody()=="")
    {
        ret["response"] = "Não Há informações no body";
    }   
    else
    {
        ret = dao.login(loginData);
    }
    
    //ret["token"]=drogon::utils::getUuid();
    auto resp=HttpResponse::newHttpJsonResponse(ret);
    callback(resp);
}

//===============================save method===================================
void Service::saveUser(const HttpRequestPtr &req,
            std::function<void (const HttpResponsePtr &)> &&callback) const
{

    Json::Value ret, user; 
    Json::Reader reader; 
    Dao dao;

    reader.parse(std::string{req->getBody()}, user);
    
    if(req->getBody()=="")
    {
        ret["response"] = "Não Há informações no body";
    }
    else
    {        
        ret = dao.saveUser(user);
    }

    auto resp=HttpResponse::newHttpJsonResponse(ret);
    callback(resp);
}

//===============================update method=================================
void Service::updateUser(const HttpRequestPtr &req,
            std::function<void (const HttpResponsePtr &)> &&callback) const
{
    Json::Value ret, user;
    Dao dao;
    Json::Reader reader; 

    reader.parse(std::string{req->getBody()}, user);
    
    if(req->getBody()=="")
    {
        ret["response"] = "Não Há informações no body";
    }
    else
    {
        ret = dao.updateUser(user);
    } 
        
    auto resp=HttpResponse::newHttpJsonResponse(ret);
    callback(resp);
}

//===============================delete method=================================
void Service::deleteUser(const HttpRequestPtr &req,
                std::function<void (const HttpResponsePtr &)> &&callback) const
{
    std::string response = "";
    Json::Value ret, user;
    Json::Reader reader; 
    Dao dao;

    reader.parse(std::string{req->getBody()}, user);
    
    if(req->getBody()=="")
    {
        ret["response"] = "Não Há informações no body";
    } 
    else
    {
        ret = dao.deleteUser(user);
    }
            
    auto resp=HttpResponse::newHttpJsonResponse(ret);
    callback(resp);
}

//===============================save vendor method============================
void Service::saveVendor(const HttpRequestPtr &req,
            std::function<void (const HttpResponsePtr &)> &&callback) const
{
    Json::Value ret, vendor;
    Json::Reader reader; 
    Dao dao;

    reader.parse(std::string{req->getBody()}, vendor);
    
    if(req->getBody()=="")
    {
        ret["response"] = "Não Há informações no body";
    } 
    else
    {        
        ret = dao.saveVendor(vendor);
    }

    auto resp=HttpResponse::newHttpJsonResponse(ret);
    callback(resp);
}

//===============================update vendor method==========================
void Service::updateVendor(const HttpRequestPtr &req,
            std::function<void (const HttpResponsePtr &)> &&callback) const
{
    Json::Value ret, vendor;
    Json::Reader reader; 
    Dao dao;
    
    reader.parse(std::string{req->getBody()}, vendor);
    
    if(req->getBody()=="")
    {
        ret["response"] = "Não Há informações no body";
    } 
    else
    {        
        ret = dao.updateVendor(vendor);
    }

    auto resp=HttpResponse::newHttpJsonResponse(ret);
    callback(resp);
}

//===============================delete vendor method==========================
void Service::deleteVendor(const HttpRequestPtr &req,
                std::function<void (const HttpResponsePtr &)> &&callback) const
{
    std::string response = "";
    Json::Value ret, vendor;
    Json::Reader reader; 
    Dao dao;

    reader.parse(std::string{req->getBody()}, vendor);
    
    if(req->getBody()=="")
    {
        ret["response"] = "Não Há informações no body";
    } 
    else
    {
        ret = dao.deleteVendor(vendor);
    }
            
    auto resp=HttpResponse::newHttpJsonResponse(ret);
    callback(resp);
}

//===============================get all users method==========================
void Service::getUsers(const HttpRequestPtr &req,
            std::function<void (const HttpResponsePtr &)> &&callback) const
{
    LOG_DEBUG<<"Get All Users -> Method = getUsers";
    Dao dao;
    Json::Value ret;

    ret = dao.getUsers();
        
    auto resp=HttpResponse::newHttpJsonResponse(ret);
    callback(resp);
}

//===============================get all vendors method========================
void Service::getVendors(const HttpRequestPtr &req,
            std::function<void (const HttpResponsePtr &)> &&callback) const
{
    Json::Value ret;
    Dao dao;
    ret = dao.getVendors();
        
    auto resp=HttpResponse::newHttpJsonResponse(ret);
    callback(resp);
}

void Service::searchUsers(const HttpRequestPtr &req,
                            std::function<void (const HttpResponsePtr &)> &&callback) const
{
    Json::Value response, request;
    Json::Reader reader;
    Dao dao;

    reader.parse(std::string{req->getBody()}, request);

    if(req->getBody()=="")
    {
        response["response"] = "Não há informações Json";
    }
    else
    {
        response = dao.searchUsers(request);
    }    

    auto resp = HttpResponse::newHttpJsonResponse(response);
    callback(resp);
}

void Service::searchVendors(const HttpRequestPtr &req,
                std::function<void (const HttpResponsePtr &)> &&callback) const
{
    Json::Value response, request;
    Json::Reader reader;
    Dao dao;

    reader.parse(std::string{req->getBody()}, request);

    if(req->getBody()=="")
    {
        response["response"] = "Não há informações Json";
    }
    else
    {
        response = dao.searchVendors(request);
    }    

    auto resp = HttpResponse::newHttpJsonResponse(response);
    callback(resp);
}

void Service::tests(const HttpRequestPtr &req,
                std::function<void (const HttpResponsePtr &)> &&callback) const
{
    LOG_DEBUG;
    Json::Reader reader;
    Json::Value temp, response;
    
    reader.parse(std::string{req->getBody()}, temp);

    validateRequest(req);
    std::string secret = "";

    time_t t = time(0);
    std::string sTime = std::to_string(t);
    std::string user_token = "alvino.barboza@youcast.tv.br";
    // sms 

    if(temp["type"].asString() == "sms")
    {
        secret = "cm3yaca6xr37xp5d0b10vw6d8yhcwd9zk1b7o0be";
    }
    else if( temp["type"].asString() == "tvn")
    {
        secret = "q2r4lgtefdz7t3obtyxw2h5crgfi877xtcbkuizo";
    }
    else
    {
        secret = "crzahutejhdwbxxf8n79g9t9ugr4dyt0sh8zxwnk";
    }

    std::string tokenUser {"91a2962738f56e145792a5fedb0f7b0fedff04bf"}, user {"alvino.barboza:"};
    std::string teste {sha1(user+tokenUser)};
    
    std::string token =  sha1(sTime+user_token+secret);

    response["teste"] = teste;
    response["sms"] = user_token+":"+sTime+":"+token;
            
    // LOG_DEBUG;
    auto resp = HttpResponse::newHttpJsonResponse(response);
    callback(resp);
}