#include "api_call_Service.h"
using namespace api::call;


//================================simple login method==========================
void Service::login(const HttpRequestPtr &req,
           std::function<void (const HttpResponsePtr &)> &&callback)
{
    std::string vendor = ""; 
    Json::Value ret, loginData;
    Json::Reader reader; 
    Dao dao;

    reader.parse(std::string{req->getBody()}, loginData);
    
    if(req->getBody()=="")
    {
        ret["response"] = "Não Há informações no body";
    }   

    LOG_DEBUG<<"Tentativa de login -> "<<loginData["login"].asString();

    ret = dao.login(loginData);
    
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

    ret = dao.saveUser(user);

    auto resp=HttpResponse::newHttpJsonResponse(ret);
    callback(resp);
}

//===============================update method===============================
void Service::updateUser(const HttpRequestPtr &req,
            std::function<void (const HttpResponsePtr &)> &&callback) const
{
    Json::Value ret, user;
    std::string response = "";
    Json::Reader reader; 

    reader.parse(std::string{req->getBody()}, user);
    
    if(req->getBody()=="")
    {
        ret["response"] = "Não Há informações no body";
    } 

    LOG_DEBUG<<"User updated -> "<<user["username"].asString()<<" Method = update";
    
    auto clientPtr = drogon::app().getDbClient();

    if(user["id"].asString() == "" || user["username"].asString() == "" || user["password"].asString() == "" || user["vendor"].asString() == "")
    {
        response = "Algum campo está vazio";
        ret["Id"] = user["id"];
        ret["Username"] = user["username"];
        ret["Password"] = user["password"];
        ret["Vendor"] = user["vendor"];
    }
    else
    {
        try
        {
            auto r = clientPtr->execSqlSync("UPDATE sms.user SET ds_user_name=$1, ds_user_password=$2, fk_vendor = (select id_vendor from sms.vendor where upper(ds_vendor_name) = upper($3)), ds_user_profile=$4 WHERE CAST(id_user AS CHAR) = $5 ;",
                                            user["username"].asString(), 
                                            user["password"].asString(), 
                                            user["vendor"].asString(),
                                            user["profile"].asInt(), 
                                            user["id"].asString());        
            response = "Usuário atualizado com sucesso!";
        }
        catch (const drogon::orm::DrogonDbException &e)
        {
            LOG_DEBUG << "catch:" << e.base().what();
            response = "Erro ao atualizar, verifique os logs do servidor!";
        }
    }

    ret["Response"] = response;
        
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

    reader.parse(std::string{req->getBody()}, user);
    
    if(req->getBody()=="")
    {
        ret["response"] = "Não Há informações no body";
    } 
    
    LOG_DEBUG<<"User deleted -> "<<user["id"].asString()<<" Method = delete";

    auto clientPtr = drogon::app().getDbClient();

    if(user["id"].asString() == "")
    {
        response = "Compo ID vazio!";
    }
    else
    {        
        try
        {
            auto r = clientPtr->execSqlSync("DELETE FROM sms.user WHERE CAST(id_user AS CHAR) = $1 ;",
                                            user["id"].asString());        
            response = "Usuário deletado com sucesso!";
        }
        catch (const drogon::orm::DrogonDbException &e)
        {
            LOG_DEBUG << "catch:" << e.base().what();
            response = "Erro ao deletar, verifique os logs do servidor!";
        }
    }

    ret["Response"] = response;
        
    auto resp=HttpResponse::newHttpJsonResponse(ret);
    callback(resp);
}

//===============================save vendor method===============================
void Service::saveVendor(const HttpRequestPtr &req,
            std::function<void (const HttpResponsePtr &)> &&callback) const
{
    
    std::string response = "";
    Json::Value ret, vendor;
    Json::Reader reader; 

    reader.parse(std::string{req->getBody()}, vendor);
    
    if(req->getBody()=="")
    {
        ret["response"] = "Não Há informações no body";
    } 

    LOG_DEBUG<<"Vendor saved -> "<<vendor["vendorname"].asString()<<" Method = save";

    auto clientPtr = drogon::app().getDbClient();

    if(vendor["vendorname"].asString() == "" || vendor["url"].asString() == "" || vendor["usertoken"].asString() == "" || vendor["token"].asString() == "")
    {
        response = "Algum campo vazio!";
        ret["Vendor_Name"] = vendor["vendorname"];
        ret["URL"] = vendor["url"];
        ret["UserToken"] = vendor["usertoken"];
        ret["Token"] = vendor["token"];
    }
    else
    {        
        try
        {
            auto r = clientPtr->execSqlSync("INSERT INTO sms.vendor(ds_vendor_name, ds_vendor_url, ds_vendor_user_token, ds_vendor_token) VALUES ($1, $2, $3, $4);",
                                            vendor["vendorname"].asString(), vendor["url"].asString(), vendor["usertoken"].asString(), vendor["token"].asString());        
            response = "Vendor salvo com sucesso!";
        }
        catch (const drogon::orm::DrogonDbException &e)
        {
            LOG_DEBUG << "catch:" << e.base().what();
            response = "Erro ao salvar, verifique os logs do servidor!";
        }
    }

    ret["Response"] = response;

    auto resp=HttpResponse::newHttpJsonResponse(ret);
    callback(resp);
}

//===============================update vendor method===============================
void Service::updateVendor(const HttpRequestPtr &req,
            std::function<void (const HttpResponsePtr &)> &&callback) const
{
    
    std::string response = "";
    Json::Value ret, vendor;
    Json::Reader reader; 

    reader.parse(std::string{req->getBody()}, vendor);

    std::cout<<req->getBody()<<std::endl;
    
    if(req->getBody()=="")
    {
        ret["response"] = "Não Há informações no body";
    } 

    LOG_DEBUG<<"Vendor updated -> "<<vendor["vendorname"].asString()<<" Method = update";


    auto clientPtr = drogon::app().getDbClient();

    if(vendor["id"].asString() == "" || vendor["vendorname"].asString() == "" || vendor["url"].asString() == "" || vendor["usertoken"].asString() == "" || vendor["token"].asString() == "")
    {
        std::cout<<"second if"<<std::endl;
        response = "Algum campo vazio!";
        ret["ID"] = vendor["id"];
        ret["Vendor_Name"] = vendor["vendorname"];
        ret["URL"] = vendor["url"];
        ret["UserToken"] = vendor["usertoken"];
        ret["Token"] = vendor["token"];
    }
    else
    {
        try
        {
            std::cout<<"before sql"<<std::endl;
            auto r = clientPtr->execSqlSync("UPDATE sms.vendor SET ds_vendor_name=$1, ds_vendor_url=$2, ds_vendor_user_token=$3, ds_vendor_token=$4 WHERE CAST(id_vendor AS CHAR) = $5 ;",
                                            vendor["vendorname"].asString(), vendor["url"].asString(), vendor["usertoken"].asString(), vendor["token"].asString(), vendor["id"].asString());        
            response = "Vendor atualizado com sucesso!";
        }
        catch (const drogon::orm::DrogonDbException &e)
        {
            LOG_DEBUG << "catch:" << e.base().what();
            response = "Erro ao atualizar, verifique os logs do servidor!";
        }
    }

    ret["Response"] = response;
        
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

    reader.parse(std::string{req->getBody()}, vendor);
    
    if(req->getBody()=="")
    {
        ret["response"] = "Não Há informações no body";
    } 

    LOG_DEBUG<<"Vendor deleted -> "<<vendor["id"].asString()<<" Method = delete";

    auto clientPtr = drogon::app().getDbClient();

    if(vendor["id"].asString() == "")
    {
        response = "Compo ID vazio!";
    }
    else
    {        
        try
        {
            auto r = clientPtr->execSqlSync("DELETE FROM sms.vendor WHERE CAST(id_vendor AS CHAR) = $1 ;",vendor["id"].asString());        
            response = "Vendor deletado com sucesso!";
        }
        catch (const drogon::orm::DrogonDbException &e)
        {
            LOG_DEBUG << "catch:" << e.base().what();
            response = "Erro ao deletar, verifique os logs do servidor!";
        }
    }

    ret["Response"] = response;
        
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
    LOG_DEBUG<<"Get All Vendors -> Method = getVendors";
    
    Json::Value ret;
    Dao dao;
    ret = dao.getVendors();
        
    auto resp=HttpResponse::newHttpJsonResponse(ret);
    callback(resp);
}

void Service::searchUsers(const HttpRequestPtr &req,
                            std::function<void (const HttpResponsePtr &)> &&callback) const
{

}

void Service::searchVendors(const HttpRequestPtr &req,
                std::function<void (const HttpResponsePtr &)> &&callback) const
{

}