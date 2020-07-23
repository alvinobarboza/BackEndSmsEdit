#include "api_call_Service.h"
using namespace api::call;

void Service::login(const HttpRequestPtr &req,
           std::function<void (const HttpResponsePtr &)> &&callback,
           std::string &&userId,
           const std::string &password)
{
    
    LOG_DEBUG<<"Tentativa de login -> "<<userId<<" Method = login";
    
    std::string vendor = "", url = "";
    Json::Value ret;

    auto clientPtr = drogon::app().getDbClient();
    try
    {
        auto r = clientPtr->execSqlSync("select * from sms.user, sms.vendor where ds_user_name=$1 and ds_user_password=$2 and fk_vendor = id_vendor",
                                        userId, password);
        for (auto const &row : r)
        {
            vendor = row["ds_vendor_name"].as<std::string>();
            url = row["ds_vendor_url"].as<std::string>();
        }

        if(vendor == "" && url == ""){
            vendor = "Usuário ou senha incorreta!";
        }
    }
    catch (const drogon::orm::DrogonDbException &e)
    {
        LOG_DEBUG << "catch:" << e.base().what();
        vendor = "Erro ao logar, verifique os logs do servidor!";
    }

    ret["Vendor"] = vendor;
    ret["URL"] = url;
    
    //ret["token"]=drogon::utils::getUuid();
    auto resp=HttpResponse::newHttpJsonResponse(ret);
    callback(resp);
}

void Service::saveUser(const HttpRequestPtr &req,
            std::function<void (const HttpResponsePtr &)> &&callback,
            const std::string &id,
            const std::string &username,
            const std::string &password,
            const std::string &vendor) const
{
    LOG_DEBUG<<"Save method "<<username;

    Json::Value ret;
    ret["result"]="ok";
    ret["user_name"]="Jack";
    ret["user_id"]=username;
    ret["gender"]=1;
    auto resp=HttpResponse::newHttpJsonResponse(ret);
    callback(resp);
}

void Service::updateUser(const HttpRequestPtr &req,
            std::function<void (const HttpResponsePtr &)> &&callback,
            const std::string &id,
            const std::string &username,
            const std::string &password,
            const std::string &vendor) const
{
    
}

void Service::deleteUser(const HttpRequestPtr &req,
                std::function<void (const HttpResponsePtr &)> &&callback,
                const std::string &id) const
{

}

void Service::getUsers(const HttpRequestPtr &req,
            std::function<void (const HttpResponsePtr &)> &&callback) const
{

}

void Service::getVendors(const HttpRequestPtr &req,
            std::function<void (const HttpResponsePtr &)> &&callback) const
{

}