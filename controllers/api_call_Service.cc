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
        ret["response"] = Json::arrayValue;
        ret["status"] = 7;
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
        ret["response"] = Json::arrayValue;
        ret["status"] = 7;
    }
    else if(validateRequest(req))
    {        
        ret = dao.saveUser(user);
    }
    else
    {
        ret["response"] = Json::arrayValue;
        ret["status"] = 20;
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
        ret["response"] = Json::arrayValue;
        ret["status"] = 7;
    }
    else if(validateRequest(req))
    {
        ret = dao.updateUser(user);
    } 
    else
    {
        ret["response"] = Json::arrayValue;
        ret["status"] = 20;
    }
        
    auto resp=HttpResponse::newHttpJsonResponse(ret);
    callback(resp);
}

//===============================delete method=================================
void Service::deleteUser(const HttpRequestPtr &req,
                std::function<void (const HttpResponsePtr &)> &&callback) const
{
    Json::Value ret, user;
    Json::Reader reader; 
    Dao dao;

    reader.parse(std::string{req->getBody()}, user);
    
    if(req->getBody()=="")
    {
        ret["response"] = Json::arrayValue;
        ret["status"] = 7;
    } 
    else if(validateRequest(req))
    {
        ret = dao.deleteUser(user);
    }
    else
    {
        ret["response"] = Json::arrayValue;
        ret["status"] = 20;
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
        ret["response"] = Json::arrayValue;
        ret["status"] = 7;
    } 
    else if(validateRequest(req))
    {
        ret = dao.saveVendor(vendor);
    }
    else
    {
        ret["response"] = Json::arrayValue;
        ret["status"] = 20;
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
        ret["response"] = Json::arrayValue;
        ret["status"] = 7;
    } 
    else if(validateRequest(req))
    {
        ret = dao.updateVendor(vendor);
    }
    else
    {
        ret["response"] = Json::arrayValue;
        ret["status"] = 20;
    }

    auto resp=HttpResponse::newHttpJsonResponse(ret);
    callback(resp);
}

//===============================delete vendor method==========================
void Service::deleteVendor(const HttpRequestPtr &req,
                std::function<void (const HttpResponsePtr &)> &&callback) const
{
    Json::Value ret, vendor;
    Json::Reader reader; 
    Dao dao;

    reader.parse(std::string{req->getBody()}, vendor);
    
    if(req->getBody()=="")
    {
        ret["response"] = Json::arrayValue;
        ret["status"] = 7;
    } 
    else if(validateRequest(req))
    {
        ret = dao.deleteVendor(vendor);
    }
    else
    {
        ret["response"] = Json::arrayValue;
        ret["status"] = 20;
    }
            
    auto resp=HttpResponse::newHttpJsonResponse(ret);
    callback(resp);
}

//===============================get all users method==========================
void Service::getUsers(const HttpRequestPtr &req,
            std::function<void (const HttpResponsePtr &)> &&callback) const
{
    LOG_DEBUG;
    Dao dao;
    Json::Value ret;
    
    if (validateRequest(req))
    {
        ret = dao.getUsers();        
    }
    else
    {
        ret["response"] = Json::arrayValue;
        ret["status"] = 20;
    }
    
        
    auto resp=HttpResponse::newHttpJsonResponse(ret);
    callback(resp);
}

//===============================get all vendors method========================
void Service::getVendors(const HttpRequestPtr &req,
            std::function<void (const HttpResponsePtr &)> &&callback) const
{
    Json::Value ret;
    Dao dao;

    if (validateRequest(req))
    {
        ret = dao.getVendors();        
    }
    else
    {
        ret["response"] = Json::arrayValue;
        ret["status"] = 20;
    }
        
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
        response["response"] = Json::arrayValue;
        response["status"] = 7;
    } 
    else if(validateRequest(req))
    {
        response = dao.searchUsers(request);
    }
    else
    {
        response["response"] = Json::arrayValue;
        response["status"] = 20;
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
        response["response"] = Json::arrayValue;
        response["status"] = 7;
    } 
    else if(validateRequest(req))
    {
        response = dao.searchVendors(request);
    }
    else
    {
        response["response"] = Json::arrayValue;
        response["status"] = 20;
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
    std::string secret = temp["secret"].asString();
    std::string user_token = temp["user"].asString();

    time_t t = time(0);
    std::string sTime = std::to_string(t);
        
    std::string token =  sha1(sTime+user_token+secret);
    
    response["token"] = user_token+":"+sTime+":"+token;
            
    // LOG_DEBUG;
    auto resp = HttpResponse::newHttpJsonResponse(response);
    callback(resp);
}