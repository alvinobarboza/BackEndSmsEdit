#include "api_call_Service.h"
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

//===============================save method===============================
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

//===============================update method===============================
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

//===============================delete method===============================
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

//===============================save vendor method===============================
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

//===============================update vendor method===============================
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

//===============================delete vendor method===============================
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

//===============================get all users method===============================
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

//===============================get all vendors method===============================
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