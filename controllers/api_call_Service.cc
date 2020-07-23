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

//===============================save method===============================
void Service::saveUser(const HttpRequestPtr &req,
            std::function<void (const HttpResponsePtr &)> &&callback,
            const std::string &username,
            const std::string &password,
            const std::string &vendor) const
{
    LOG_DEBUG<<"User saved -> "<<username<<" Method = save";
    
    std::string response = "";
    Json::Value ret;

    auto clientPtr = drogon::app().getDbClient();
    try
    {
        auto r = clientPtr->execSqlSync("INSERT INTO sms.user(ds_user_name, ds_user_password, fk_vendor) VALUES ($1, $2, (select id_vendor from sms.vendor where upper(ds_vendor_name) = upper($3)));",
                                        username, password, vendor);
        
        response = "Usuário salvo com sucesso!";
    }
    catch (const drogon::orm::DrogonDbException &e)
    {
        LOG_DEBUG << "catch:" << e.base().what();
        response = "Erro ao salvar, verifique os logs do servidor!";
    }

    ret["Response"] = response;
    ret["Test"] = ret;

    auto resp=HttpResponse::newHttpJsonResponse(ret);
    callback(resp);
}

//===============================update method===============================
void Service::updateUser(const HttpRequestPtr &req,
            std::function<void (const HttpResponsePtr &)> &&callback,
            const std::string &id,
            const std::string &username,
            const std::string &password,
            const std::string &vendor) const
{
    LOG_DEBUG<<"User updated -> "<<username<<" Method = save";
    
    std::string response = "";
    Json::Value ret;

    auto clientPtr = drogon::app().getDbClient();
    try
    {
        auto r = clientPtr->execSqlSync("UPDATE sms.user SET ds_user_name=$1, ds_user_password=$2, fk_vendor = (select id_vendor from sms.vendor where upper(ds_vendor_name) = upper($3)) WHERE CAST(id_user AS CHAR) = $4 ;",
                                        username, password, vendor, id);
        
        response = "Usuário atualizado com sucesso!";
    }
    catch (const drogon::orm::DrogonDbException &e)
    {
        LOG_DEBUG << "catch:" << e.base().what();
        response = "Erro ao atualizar, verifique os logs do servidor!";
    }

    ret["Response"] = response;
        
    auto resp=HttpResponse::newHttpJsonResponse(ret);
    callback(resp);
}

//===============================delete method===============================
void Service::deleteUser(const HttpRequestPtr &req,
                std::function<void (const HttpResponsePtr &)> &&callback,
                const std::string &id) const
{

}

//===============================get all users method===============================
void Service::getUsers(const HttpRequestPtr &req,
            std::function<void (const HttpResponsePtr &)> &&callback) const
{

}

//===============================saget all vendors method===============================
void Service::getVendors(const HttpRequestPtr &req,
            std::function<void (const HttpResponsePtr &)> &&callback) const
{

}