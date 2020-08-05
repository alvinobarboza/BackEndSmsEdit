#include "api_call_Service.h"
using namespace api::call;

void Service::login(const HttpRequestPtr &req,
           std::function<void (const HttpResponsePtr &)> &&callback,
           std::string &&userId,
           const std::string &password)
{
    
    LOG_DEBUG<<"Tentativa de login -> "<<userId<<" Method = login";
    
    std::string vendor = "", url = "", userToken ="", token = "";
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
            userToken = row["ds_vendor_user_token"].as<std::string>();
            token = row["ds_vendor_token"].as<std::string>();
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

    ret["User_Token"] = userToken;
    ret["token"] = token;
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

    if(username == "" || password == "" || vendor == "")
    {
        response = "Algum campo está vazio";
        ret["Username"] = username;
        ret["Password"] = password;
        ret["Vendor"] = vendor;
    }
    else
    {
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

    if(id == "" || username == "" || password == "" || vendor == "")
    {
        response = "Algum campo está vazio";
        ret["Id"] = id;
        ret["Username"] = username;
        ret["Password"] = password;
        ret["Vendor"] = vendor;
    }
    else
    {
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

    if(id == "")
    {
        response = "Compo ID vazio!";
    }
    else
    {        
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
    }

    ret["Response"] = response;
        
    auto resp=HttpResponse::newHttpJsonResponse(ret);
    callback(resp);
}

//===============================save vendor method===============================
void Service::saveVendor(const HttpRequestPtr &req,
            std::function<void (const HttpResponsePtr &)> &&callback,
            const std::string &vendorname,
            const std::string &url,
            const std::string &usertoken,
            const std::string &token) const
{
    LOG_DEBUG<<"Vendor saved -> "<<vendorname<<" Method = save";
    
    std::string response = "";
    Json::Value ret;

    auto clientPtr = drogon::app().getDbClient();

    if(vendorname == "" || url == "" || usertoken == "" || token == "")
    {
        response = "Algum campo vazio!";
        ret["Vendor_Name"] = vendorname;
        ret["URL"] = url;
        ret["UserToken"] = usertoken;
        ret["Token"] = token;
    }
    else
    {        
        try
        {
            auto r = clientPtr->execSqlSync("INSERT INTO sms.vendor(ds_vendor_name, ds_vendor_url, ds_vendor_user_token, ds_vendor_token) VALUES ($1, $2, $3, $4);",
                                            vendorname, url, usertoken, token);        
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
            std::function<void (const HttpResponsePtr &)> &&callback,
            const std::string &id,
            const std::string &vendorname,
            const std::string &url,
            const std::string &usertoken,
            const std::string &token) const
{
    LOG_DEBUG<<"Vendor updated -> "<<vendorname<<" Method = update";
    
    std::string response = "";
    Json::Value ret;

    auto clientPtr = drogon::app().getDbClient();

    if(id == "" || vendorname == "" || url == "" || usertoken == "" || token == "")
    {
        response = "Algum campo vazio!";
        ret["Vendor_Name"] = vendorname;
        ret["URL"] = url;
        ret["UserToken"] = usertoken;
        ret["Token"] = token;
    }
    else
    {
        try
        {
            auto r = clientPtr->execSqlSync("UPDATE sms.vendor SET ds_vendor_name=$1, ds_vendor_url=$2, ds_vendor_user_token=$3, ds_vendor_token=$4 WHERE CAST(id_vendor AS CHAR) = $5 ;",
                                            vendorname, url, usertoken, token, id);        
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
                std::function<void (const HttpResponsePtr &)> &&callback,
                const std::string &id) const
{
    LOG_DEBUG<<"Vendor deleted -> "<<id<<" Method = delete";
    
    std::string response = "";
    Json::Value ret;

    auto clientPtr = drogon::app().getDbClient();

    
    if(id == "")
    {
        response = "Compo ID vazio!";
    }
    else
    {        
        try
        {
            auto r = clientPtr->execSqlSync("DELETE FROM sms.vendor WHERE CAST(id_vendor AS CHAR) = $1 ;",
                                            id);        
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
            obj["User_token"] = row["ds_vendor_user_token"].as<std::string>();
            obj["Token"] = row["ds_vendor_token"].as<std::string>();

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
//=====================================test get API =============================
void Service::getUserSMS(const HttpRequestPtr &req,
                             std::function<void (const HttpResponsePtr &)> &&callback,
                             const std::string &id)const
{
    LOG_DEBUG << "get SMS";
    static Json::Value json;
    static bool check = true;
    
    int count = 0;
    auto client = HttpClient::newHttpClient("https://sms.youcast.tv.br");
    auto requestH = HttpRequest::newHttpRequest();
    requestH->setMethod(drogon::Post);
    requestH->setPath("/api/customer/getData");
    requestH->addHeader("Authorization","");
    requestH->addHeader("Content-Type","application/json");
    requestH->setBody("{'data':{'viewers_id': 02}}");

    //for (int i = 0; i < 2; ++i)
    //{
    //}    
    client->sendRequest(
        requestH,
        [=,&count](ReqResult result, const HttpResponsePtr &response){
            std::cout << "receive response!" << std::endl;
            // auto headers=response.
            ++count;

            json = *response->getJsonObject();
            
            std::cout << "Json received" << std::endl;
          
            std::cout<<json<<"Response"<<std::endl;
        });

    while(check)
    {
        std::cout << count++ << "Count \n";
        if(json != nullptr)
            check = false;

    }
         
    auto resp=HttpResponse::newHttpJsonResponse(json);
    callback(resp);
    std::cout<<json<<"Response worked!"<< std::endl;
    
       
    
}