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
    LOG_DEBUG<<"User updated -> "<<username<<" Method = update";
    
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
    LOG_DEBUG<<"User deleted -> "<<id<<" Method = delete";
    
    std::string response = "";
    Json::Value ret;

    auto clientPtr = drogon::app().getDbClient();
    try
    {
        auto r = clientPtr->execSqlSync("DELETE FROM sms.user WHERE CAST(id_user AS CHAR) = $1 ;",
                                        id);        
        response = "Usuário deletado com sucesso!";
    }
    catch (const drogon::orm::DrogonDbException &e)
    {
        LOG_DEBUG << "catch:" << e.base().what();
        response = "Erro ao deletar, verifique os logs do servidor!";
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
    
    std::string response = "", count = "";
    Json::Value ret, obj;

    auto clientPtr = drogon::app().getDbClient();
    try
    {
        auto r = clientPtr->execSqlSync("SELECT id_user, ds_user_name, ds_vendor_name FROM sms.user, sms.vendor WHERE fk_vendor = id_vendor;");
        
        int i = 0;
        for (auto const &row : r)
        {
            obj["UserID"] = row["id_user"].as<std::string>();
            obj["UserName"] = row["ds_user_name"].as<std::string>();
            obj["Vendor"] = row["ds_vendor_name"].as<std::string>();

            count = std::to_string(i);
            ret[count] = obj;
            i++;
        }
        response = "Usuários listados com sucesso!";
        
    }
    catch (const drogon::orm::DrogonDbException &e)
    {
        LOG_DEBUG << "catch:" << e.base().what();
        response = "Erro ao listar, verifique os logs do servidor!";
    }

    ret["Response"] = response;
        
    auto resp=HttpResponse::newHttpJsonResponse(ret);
    callback(resp);
}

//===============================get all vendors method===============================
void Service::getVendors(const HttpRequestPtr &req,
            std::function<void (const HttpResponsePtr &)> &&callback) const
{
    LOG_DEBUG<<"Get All Vendors -> Method = getVendors";
    
    std::string response = "", count = "";
    Json::Value ret, obj;

    auto clientPtr = drogon::app().getDbClient();
    try
    {
        auto r = clientPtr->execSqlSync("SELECT * FROM sms.vendor;");
        
        int i = 0;
        for (auto const &row : r)
        {
            obj["VendorID"] = row["id_vendor"].as<std::string>();
            obj["VendorName"] = row["ds_vendor_name"].as<std::string>();
            obj["VendorUrl"] = row["ds_vendor_url"].as<std::string>();

            count = std::to_string(i);
            ret[count] = obj;
            i++;
        }
        response = "Vendors listados com sucesso!";
        
    }
    catch (const drogon::orm::DrogonDbException &e)
    {
        LOG_DEBUG << "catch:" << e.base().what();
        response = "Erro ao listar, verifique os logs do servidor!";
    }

    ret["Response"] = response;
        
    auto resp=HttpResponse::newHttpJsonResponse(ret);
    callback(resp);
}

void Service::getUserSMS(const HttpRequestPtr &req,
                            std::function<void (const HttpResponsePtr &)> &&callback,
                            const std::string &id)const
{

    Json::Value ret;

    int count = 0;
    auto client = HttpClient::newHttpClient("http://sms.youcast.tv.br");
    auto requestH = HttpRequest::newHttpRequest();
    requestH->setMethod(drogon::Post);
    requestH->setPath("/api/customer/getData");
    requestH->addHeader("Authorization","alvino.barboza@youcast.tv.br:1595625451:dd441f04c06c9bd718c8f1815304705db0348be9");
    requestH->setBody("{\"data\":{\"viewers_id\": 02}}");

    for (int i = 0; i < 10; ++i)
    {
        client->sendRequest(
            requestH,
            [&count,&ret](ReqResult result, const HttpResponsePtr &response) {
                std::cout << "receive response!" << std::endl;
                // auto headers=response.
                ++count;
                auto retrieve = response->getJsonObject();

                //ret = retrieve ;
                // auto cookies = response->cookies();
                // for (auto const &cookie : cookies)
                // {
                //     std::cout << cookie.first << "="
                //                 << cookie.second.value()
                //                 << ":domain=" << cookie.second.domain()
                //                 << std::endl;
                // }
               // std::cout << "count=" << retrieve << std::endl;
            });
    }    

    LOG_DEBUG<<"Get All SMS Users -> Method = getVendors";

    auto resp=HttpResponse::newHttpJsonResponse(ret);
    callback(resp);
}