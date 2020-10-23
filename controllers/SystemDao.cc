#include "SystemDao.h"

Json::Value Dao::login(Json::Value &request)
{
    LOG_DEBUG;
    Json::Value temp;
    int profile = 2;
    
    auto clientPtr = drogon::app().getDbClient();
    try
    {
        auto r = clientPtr->execSqlSync("select * from sms.user, sms.vendor where ds_user_name=$1 and ds_user_password=$2 and fk_vendor = id_vendor",
                                        request["login"].asString(),request["password"].asString());
        for (auto const &row : r)
        {
            temp["vendor"] = row["ds_vendor_name"].as<std::string>();           
            profile = row["ds_user_profile"].as<int>();
            temp["name"] = row["ds_profile_name"].as<std::string>();
            temp["id"] = row["id_user"].as<std::string>();        
        }
        if(temp["vendor"].asString() == "" && profile == 2){
            temp["Error"] = "Usuário ou senha incorreta!";
        }else
        {
            temp["profile"] = profile;
        }
    }
    catch (const drogon::orm::DrogonDbException &e)
    {
        LOG_DEBUG << "catch:" << e.base().what();
        temp["Error"] = "Erro ao logar, verifique os logs do servidor!";
    }
    return temp;
}

Json::Value Dao::saveUser(Json::Value &request)
{
    LOG_DEBUG;
    std::string response;
    Json::Value temp;
    std::vector<std::string> keyNames;
    int i = 0;

    keyNames = request.getMemberNames();
    //Check if json has all key values expected in this case must have all five values
    for(std::string key : keyNames)
    {
        if(key == "username" || key == "password" || key == "vendor" || key == "profile" || key == "name")
        {
            i++;
        }
    }
    if(i==5)
    {        
        if(request["username"].asString() == ""|| 
            request["password"].asString() == ""|| 
            request["vendor"].asString() == ""||
            request["name"].asString() == "")
        {
            temp["response"] = "Algum campo vazio";
            temp["requestSent"] = request;
            return temp;
        }
        else
        {
            if(checkExistingUser(request))
            {
                temp["response"] = "Usuário já existe!";
                return temp;
            }

            auto clientPtr = drogon::app().getDbClient();
            try
            {
                auto r = clientPtr->execSqlSync("INSERT INTO sms.user(ds_user_name, ds_user_password, fk_vendor, ds_user_profile, ds_profile_name) VALUES ($1, $2, (select id_vendor from sms.vendor where upper(ds_vendor_name) = upper($3)),$4, $5);",
                                                request["username"].asString(), 
                                                request["password"].asString(), 
                                                request["vendor"].asString(),
                                                request["profile"].asInt(),
                                                request["name"].asString());
                response = "Usuário criado com sucesso!";
            }
            catch (const drogon::orm::DrogonDbException &e)
            {
                LOG_DEBUG << "catch:" << e.base().what();
                response = "Erro ao atualizar, verifique os logs do servidor!";
            }  
        }        
        temp["response"] = response;
        return temp;
    }
    else
    {
        temp["response"] = "Algum campo não enviado";
        temp["requestSent"] = request;
        return temp;
    }    
}

Json::Value Dao::updateUser(Json::Value &request)
{    
    LOG_DEBUG;
    std::string response;
    Json::Value temp;
    std::vector<std::string> keyNames;
    int i = 0;

    keyNames = request.getMemberNames();
    //Check if json has all key values expected in this case must have all five values
    for(std::string key : keyNames)
    {
        if(key == "id" || key == "username" || key == "password" || key == "vendor" || key == "profile" || key == "name")
        {
            i++;
        }
    }
    if(i==6)
    {        
        if(request["id"].asString() == "")
        {
            temp["response"] = "ID não informado";
            return temp;
        }
        else
        {
            auto clientPtr = drogon::app().getDbClient();
            try
            {
                auto r = clientPtr->execSqlSync("UPDATE sms.user "
												"SET ds_user_name= CASE WHEN $1 = '' THEN ds_user_name ELSE $1 END, "
												"	 ds_user_password=CASE WHEN $2 = '' THEN ds_user_password ELSE $2 END, "
												"	 fk_vendor = CASE WHEN $3 = '' THEN fk_vendor ELSE (select id_vendor from sms.vendor where upper(ds_vendor_name) = upper($3)) END, "
												"	 ds_user_profile=$4, "
												"	 ds_profile_name=CASE WHEN $6 = '' THEN ds_profile_name ELSE $6 END "
												"WHERE CAST(id_user AS CHAR(10)) = $5;",
                                                request["username"].asString(), 
                                                request["password"].asString(), 
                                                request["vendor"].asString(),
                                                request["profile"].asInt(), 
                                                request["id"].asString(),
                                                request["name"].asString());        
                response = "Usuário atualizado com sucesso!";
            }
            catch (const drogon::orm::DrogonDbException &e)
            {
                LOG_DEBUG << "catch:" << e.base().what();
                response = "Erro ao atualizar, verifique os logs do servidor!";
            }
        }
        
        temp["response"] = response;

        return temp;
    }
    else
    {
        temp["response"] = "Algum campo não enviado";
        temp["requestSent"] = request;
        return temp;
    } 
}

Json::Value Dao::deleteUser(Json::Value &request)
{
    LOG_DEBUG;
    Json::Value temp;
    std::string response;
    
    if(request["id"].asString() == "")
    {
        response = "Compo ID vazio!";
    }
    else
    {        
        auto clientPtr = drogon::app().getDbClient();
        try
        {
            auto r = clientPtr->execSqlSync("DELETE FROM sms.user WHERE CAST(id_user AS VARCHAR) = $1 ;",
                                            request["id"].asString());        
            response = "Usuário deletado com sucesso!";
        }
        catch (const drogon::orm::DrogonDbException &e)
        {
            LOG_DEBUG << "catch:" << e.base().what();
            response = "Erro ao deletar, verifique os logs do servidor!";
        }
    }
    temp["response"] = response;
    return temp;
}

Json::Value Dao::saveVendor(Json::Value &request)
{
    LOG_DEBUG;
    std::string response;
    Json::Value temp;
    std::vector<std::string> keyNames;
    int i = 0;

    keyNames = request.getMemberNames();
    //Check if json has all key values expected in this case must have all five values
    for(std::string key : keyNames)
    {
        if(key == "vendorname" || key == "url" || key == "usertoken" || key == "token")
        {
            i++;
        }
    }
    if(i==4)
    {        
        if(request["vendorname"].asString() == ""|| 
            request["url"].asString() == ""|| 
            request["usertoken"].asString() == ""||
            request["token"].asString() == "")
        {
            temp["response"] = "Algum campo vazio";
            temp["requestSent"] = request;
            return temp;
        }
        else
        {
            if(checkExistingVendor(request))
            {
                temp["response"] = "Vendor já existe!";
                return temp;
            }

            auto clientPtr = drogon::app().getDbClient();
            try
            {
                auto r = clientPtr->execSqlSync("INSERT INTO sms.vendor(ds_vendor_name, ds_vendor_url, ds_vendor_user_token, ds_vendor_token) VALUES ($1, $2, $3, $4);",
                                                request["vendorname"].asString(), 
                                                request["url"].asString(), 
                                                request["usertoken"].asString(),
                                                request["token"].asString());
                response = "Vendor criado com sucesso!";
            }
            catch (const drogon::orm::DrogonDbException &e)
            {
                LOG_DEBUG << "catch:" << e.base().what();
                response = "Erro ao atualizar, verifique os logs do servidor!";
            }  
        }
        
        temp["response"] = response;

        return temp;
    }
    else
    {
        temp["response"] = "Algum campo não enviado";
        temp["requestSent"] = request;
        return temp;
    }
}

Json::Value Dao::updateVendor(Json::Value &request)
{
    LOG_DEBUG;
    std::string response;
    Json::Value temp;
    std::vector<std::string> keyNames;
    int i = 0;

    keyNames = request.getMemberNames();
    //Check if json has all key values expected in this case must have all five values
    for(std::string key : keyNames)
    {
        if(key == "id" || key == "vendorname" || key == "url" || key == "usertoken" || key == "token")
        {
            i++;
        }
    }
    if(i==5)
    {        
        if(request["id"].asString() == "")
        {
            temp["response"] = "ID não informado";
            return temp;
        }
        else
        {
            auto clientPtr = drogon::app().getDbClient();
            try
            {
                auto r = clientPtr->execSqlSync("UPDATE sms.vendor "
                                                "SET ds_vendor_name= CASE WHEN $1 = '' THEN ds_vendor_name ELSE $1 END, "
                                                "   ds_vendor_url = CASE WHEN $2 = '' THEN ds_vendor_url ELSE $2 END, "
                                                "   ds_vendor_user_token = CASE WHEN $3 = '' THEN ds_vendor_user_token ELSE $3 END, "
                                                "   ds_vendor_token = CASE WHEN $4 = '' THEN ds_vendor_token ELSE $4 END "
                                                "WHERE CAST(id_vendor AS CHAR(10)) = $5 ;",
                                                request["vendorname"].asString(), 
                                                request["url"].asString(), 
                                                request["usertoken"].asString(),
                                                request["token"].asString(),
                                                request["id"].asString());
                response = "Vendor atualizado com sucesso!";
            }
            catch (const drogon::orm::DrogonDbException &e)
            {
                LOG_DEBUG << "catch:" << e.base().what();
                response = "Erro ao atualizar, verifique os logs do servidor!";
            }  
        }
        
        temp["response"] = response;

        return temp;
    }
    else
    {
        temp["response"] = "Algum campo não enviado";
        temp["requestSent"] = request;
        return temp;
    }
}

Json::Value Dao::deleteVendor(Json::Value &request)
{
    LOG_DEBUG;
    Json::Value temp;
    std::string response;
    
    if(request["id"].asString() == "")
    {
        response = "Compo ID vazio!";
    }
    else
    {        
        auto clientPtr = drogon::app().getDbClient();
        try
        {
            auto r = clientPtr->execSqlSync("DELETE FROM sms.vendor WHERE CAST(id_vendor AS VARCHAR) = $1 ;",
                                            request["id"].asString());        
            response = "Vendor deletado com sucesso!";
            std::cout << request << std::endl;
        }
        catch (const drogon::orm::DrogonDbException &e)
        {
            LOG_DEBUG << "catch:" << e.base().what();
            response = "Erro ao deletar, verifique os logs do servidor!";
        }
    }
    temp["response"] = response;
    return temp;
}

Json::Value Dao::getUsers()
{
    LOG_DEBUG;
    Json::Value temp;
    std::string response;

    auto clientPtr = drogon::app().getDbClient();
    try
    {
        auto r = clientPtr->execSqlSync("SELECT * FROM sms.user, sms.vendor WHERE fk_vendor = id_vendor;");
        
        int i = 0;
        std::string count;
        for (auto const &row : r)
        {
            Json::Value obj;
            obj["UserID"] = row["id_user"].as<std::string>();
            obj["UserName"] = row["ds_user_name"].as<std::string>();
            obj["Vendor"] = row["ds_vendor_name"].as<std::string>();
            obj["Profile"] = row["ds_user_profile"].as<int>();
            obj["Name"] = row["ds_profile_name"].as<std::string>();
            
            count = std::to_string(i);
            temp[count] = obj;
            i++;
        }
        response = "Usuários listados com sucesso!";
        
    }
    catch (const drogon::orm::DrogonDbException &e)
    {
        LOG_DEBUG << "catch:" << e.base().what();
        response = "Erro ao listar, verifique os logs do servidor!";
    }

    temp["response"] = response;

    return temp;
}

Json::Value Dao::getVendors()
{
    LOG_DEBUG;
    std::string response = "", count = "";
    Json::Value temp, obj;

    auto clientPtr = drogon::app().getDbClient();
    try
    {
        auto r = clientPtr->execSqlSync("SELECT * FROM sms.vendor;");        
        int i = 0;
        for (auto const &row : r)
        {
            obj["VendorID"] = row["id_vendor"].as<std::string>();
            obj["VendorName"] = row["ds_vendor_name"].as<std::string>();
            obj["Secret"] = row["ds_vendor_token"].as<std::string>();
            obj["UserSecret"] = row["ds_vendor_user_token"].as<std::string>();
            obj["URL"] = row["ds_vendor_url"].as<std::string>();
            
            count = std::to_string(i);
            temp[count] = obj;
            i++;
        }
    }
    catch (const drogon::orm::DrogonDbException &e)
    {
        LOG_DEBUG << "catch:" << e.base().what();
    }
    return temp;
}

Json::Value Dao::searchUsers(Json::Value &request)
{
    LOG_DEBUG;
    Json::Value temp, obj;
    
    auto clientPtr = drogon::app().getDbClient();
    try
    {
        auto r = clientPtr->execSqlSync("select id_user, ds_user_name, ds_user_profile, ds_profile_name, ds_vendor_name from sms.user, sms.vendor where (upper(ds_profile_name) like upper('%"+request["search"].asString()+"%') or upper(ds_user_name) like upper('%"+request["search"].asString()+"%') or upper(ds_vendor_name) like upper('%"+request["search"].asString()+"%')) and fk_vendor = id_vendor;");        
        int i = 0;
        
        for (auto const &row : r)
        {
            obj["idUser"] = row["id_user"].as<std::string>();
            obj["userName"] = row["ds_user_name"].as<std::string>();
            obj["profile"] = row["ds_user_profile"].as<int>();
            obj["name"] = row["ds_profile_name"].as<std::string>();
            obj["vendor"] = row["ds_vendor_name"].as<std::string>();
           
            temp[std::to_string(i)] = obj;
            i++;
        }       
    }
    catch (const drogon::orm::DrogonDbException &e)
    {
        LOG_DEBUG << "catch:" << e.base().what();
        temp["response"] = "Erro ao listar, verifique os logs do servidor!";
    }
    
    return temp;
}

Json::Value Dao::searchVendors(Json::Value &request)
{
    LOG_DEBUG;
    Json::Value temp, obj;
    
    auto clientPtr = drogon::app().getDbClient();
    try
    {
        auto r = clientPtr->execSqlSync("select * from sms.vendor where (upper(ds_vendor_name) like upper('%"+request["search"].asString()+"%') or upper(ds_vendor_user_token) like upper('%"+request["search"].asString()+"%'))");        
        int i = 0;
        
        for (auto const &row : r)
        {
            obj["idVendor"] = row["id_vendor"].as<std::string>();
            obj["vendorName"] = row["ds_vendor_name"].as<std::string>();
            obj["url"] = row["ds_vendor_url"].as<std::string>();
            obj["userToken"] = row["ds_vendor_user_token"].as<std::string>();
           
            temp[std::to_string(i)] = obj;
            i++;
        }       
    }
    catch (const drogon::orm::DrogonDbException &e)
    {
        LOG_DEBUG << "catch:" << e.base().what();
        temp["response"] = "Erro ao listar, verifique os logs do servidor!";
    }
    return temp;
}

bool Dao::checkExistingUser(Json::Value &user)
{
    LOG_DEBUG;
    bool check = false;
    auto clientPtr = drogon::app().getDbClient();
    try
    {
        auto r = clientPtr->execSqlSync("select ds_user_name from sms.user where ds_user_name=$1", user["username"].asString());        
        int i = 0;        
        for (auto const &row : r)
        {
            if(row["ds_user_name"].as<std::string>() != "")
                check = true;
        }       
    }
    catch (const drogon::orm::DrogonDbException &e)
    {
        LOG_DEBUG << "catch:" << e.base().what();
    }
    return check;
}

bool Dao::checkExistingVendor(Json::Value &vendor)
{
    LOG_DEBUG;
    bool check = false;
    auto clientPtr = drogon::app().getDbClient();
    try
    {
        auto r = clientPtr->execSqlSync("select ds_vendor_name from sms.vendor where upper(ds_vendor_name)=upper($1)", vendor["vendorname"].asString());        
        int i = 0;        
        for (auto const &row : r)
        {
            if(row["ds_vendor_name"].as<std::string>() != "")
                check = true;
        }       
    }
    catch (const drogon::orm::DrogonDbException &e)
    {
        LOG_DEBUG << "catch:" << e.base().what();
    }
    return check;
}