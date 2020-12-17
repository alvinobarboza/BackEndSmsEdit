#include "SystemDao.hpp"

Json::Value Dao::login(Json::Value &request)
{
    LOG_DEBUG;
    Json::Value temp;
    int profile = 2;
    std::string password {sha1(request["password"].asString())};
    
    auto clientPtr = drogon::app().getDbClient();
    try
    {
        auto r = clientPtr->execSqlSync("select * from sms.user, sms.vendor where ds_user_name=$1 and ds_user_password=$2 and fk_vendor = id_vendor",
                                        request["login"].asString(), password);

        Json::Value obj;
        for (auto const &row : r)
        {
            obj[0]["vendor"] = row["ds_vendor_name"].as<std::string>();           
            profile = row["ds_user_profile"].as<int>();
            obj[0]["token"] = row["ds_user_token"].as<std::string>();
            obj[0]["name"] = row["ds_profile_name"].as<std::string>();
            obj[0]["id"] = row["id_user"].as<std::string>();        
        }
        if(obj[0]["vendor"].asString() == "" && profile == 2){
            temp["status"] = 14001;
            temp["response"] = Json::arrayValue;
        }else
        {
            obj[0]["profile"] = profile;
            temp["status"] = 1;
            temp["response"] = obj;
        }
    }
    catch (const drogon::orm::DrogonDbException &e)
    {
        LOG_DEBUG << "catch:" << e.base().what();
        temp["status"] = 0;
        temp["response"] = Json::arrayValue;
    }
    return temp;
}

Json::Value Dao::saveUser(Json::Value &request)
{
    LOG_DEBUG;
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
            temp["response"] = Json::arrayValue;
            temp["status"] = 6;
            return temp;
        }
        else
        {
            if(checkExistingUser(request))
            {
                temp["response"] = Json::arrayValue;
                temp["status"] = 14000;
                return temp;
            }

            auto clientPtr = drogon::app().getDbClient();
            try
            {
                time_t t = time(0);
                std::string sTime {std::to_string(t)};
                std::string token {sTime+":"+request["username"].asString()};

                auto r = clientPtr->execSqlSync("INSERT INTO sms.user(ds_user_name, ds_user_password, fk_vendor, ds_user_profile, ds_profile_name, ds_user_token) VALUES ($1, $2, (select id_vendor from sms.vendor where upper(ds_vendor_name) = upper($3)),$4, $5, $6);",
                                                request["username"].asString(), 
                                                sha1(request["password"].asString()), 
                                                request["vendor"].asString(),
                                                request["profile"].asInt(),
                                                request["name"].asString(),
                                                sha1(token));
                if(r.affectedRows())
                {
                    temp["response"] = Json::arrayValue;
                    temp["status"] = 1;
                }
                else
                {
                    temp["response"] = Json::arrayValue;
                    temp["status"] = 0;
                }
            }
            catch (const drogon::orm::DrogonDbException &e)
            {
                LOG_DEBUG << "catch:" << e.base().what();
                temp["response"] = Json::arrayValue;
                temp["status"] = 0;
            }
        }
        return temp;
    }
    else
    {
        temp["response"] = Json::arrayValue;
        temp["status"] = 6;
        return temp;
    }    
}

Json::Value Dao::updateUser(Json::Value &request)
{    
    LOG_DEBUG;
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
            temp["response"] = Json::arrayValue;
            temp["status"] = 6;
            return temp;
        }
        else
        {
            if(checkExistingUser(request))
            {
                temp["response"] = Json::arrayValue;
                temp["status"] = 14000;
                return temp;
            }
            auto clientPtr = drogon::app().getDbClient();
            try
            {
                auto r = clientPtr->execSqlSync("UPDATE sms.user "
												"SET ds_user_name= CASE WHEN $1 = '' THEN ds_user_name ELSE $1 END, "
												"	 ds_user_password=CASE WHEN $2 = 'da39a3ee5e6b4b0d3255bfef95601890afd80709' THEN ds_user_password ELSE $2 END, "
												"	 fk_vendor = CASE WHEN $3 = '' THEN fk_vendor ELSE (select id_vendor from sms.vendor where upper(ds_vendor_name) = upper($3)) END, "
												"	 ds_user_profile=$4, "
												"	 ds_profile_name=CASE WHEN $6 = '' THEN ds_profile_name ELSE $6 END "
												"WHERE CAST(id_user AS CHAR(10)) = $5;",
                                                request["username"].asString(), 
                                                sha1(request["password"].asString()), 
                                                request["vendor"].asString(),
                                                request["profile"].asInt(), 
                                                request["id"].asString(),
                                                request["name"].asString());        
                if(r.affectedRows())
                {
                    temp["response"] = Json::arrayValue;
                    temp["status"] = 1;
                }
                else
                {
                    temp["response"] = Json::arrayValue;
                    temp["status"] = 0;
                }
            }
            catch (const drogon::orm::DrogonDbException &e)
            {
                LOG_DEBUG << "catch:" << e.base().what();
                temp["response"] = Json::arrayValue;
                temp["status"] = 0;
            }
        }
        return temp;
    }
    else
    {
        temp["response"] = Json::arrayValue;
        temp["status"] = 6;
        return temp;
    } 
}

Json::Value Dao::deleteUser(Json::Value &request)
{
    LOG_DEBUG;
    Json::Value temp;
    
    if(request["id"].asString() == "")
    {
        temp["response"] = Json::arrayValue;
        temp["status"] = 6;
    }
    else
    {        
        auto clientPtr = drogon::app().getDbClient();
        try
        {
            auto r = clientPtr->execSqlSync("DELETE FROM sms.user WHERE CAST(id_user AS VARCHAR) = $1 ;",
                                            request["id"].asString());        
            if(r.affectedRows())
            {
                temp["response"] = Json::arrayValue;
                temp["status"] = 1;
            }
            else
            {
                temp["response"] = Json::arrayValue;
                temp["status"] = 0;
            }
        }
        catch (const drogon::orm::DrogonDbException &e)
        {
            LOG_DEBUG << "catch:" << e.base().what();
            temp["response"] = Json::arrayValue;
            temp["status"] = 0;
        }
    }
    return temp;
}

Json::Value Dao::saveVendor(Json::Value &request)
{
    LOG_DEBUG;
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
            temp["response"] = Json::arrayValue;
            temp["status"] = 6;
            return temp;
        }
        else
        {
            if(checkExistingVendor(request))
            {
                temp["response"] = Json::arrayValue;
                temp["status"] = 14000;
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
                if(r.affectedRows())
                {
                    temp["response"] = Json::arrayValue;
                    temp["status"] = 1;
                }
                else
                {
                    temp["response"] = Json::arrayValue;
                    temp["status"] = 0;
                }
            }
            catch (const drogon::orm::DrogonDbException &e)
            {
                LOG_DEBUG << "catch:" << e.base().what();
                temp["response"] = Json::arrayValue;
                temp["status"] = 0;
            }  
        }
        return temp;
    }
    else
    {
        temp["response"] = Json::arrayValue;
        temp["status"] = 6;
        return temp;
    }
}

Json::Value Dao::updateVendor(Json::Value &request)
{
    LOG_DEBUG;
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
            temp["response"] = Json::arrayValue;
            temp["status"] = 6;
            return temp;
        }
        else
        {
            if(checkExistingVendor(request))
            {
                temp["response"] = Json::arrayValue;
                temp["status"] = 14000;
                return temp;
            }
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
                if(r.affectedRows())
                {
                    temp["response"] = Json::arrayValue;
                    temp["status"] = 1;
                }
                else
                {
                    temp["response"] = Json::arrayValue;
                    temp["status"] = 0;
                }
            }
            catch (const drogon::orm::DrogonDbException &e)
            {
                LOG_DEBUG << "catch:" << e.base().what();
                temp["response"] = Json::arrayValue;
                temp["status"] = 0;
            }  
        }
        return temp;
    }
    else
    {
        temp["response"] = Json::arrayValue;
        temp["status"] = 6;
        return temp;
    }
}

Json::Value Dao::deleteVendor(Json::Value &request)
{
    LOG_DEBUG;
    Json::Value temp;

    if(request["id"].asString() == "")
    {
        temp["response"] = Json::arrayValue;
        temp["status"] = 6;
    }
    else
    {        
        auto clientPtr = drogon::app().getDbClient();
        try
        {
            auto r = clientPtr->execSqlSync("DELETE FROM sms.vendor WHERE CAST(id_vendor AS VARCHAR) = $1 ;",
                                            request["id"].asString());        
            if(r.affectedRows())
            {
                temp["response"] = Json::arrayValue;
                temp["status"] = 1;
            }
            else
            {
                temp["response"] = Json::arrayValue;
                temp["status"] = 0;
            }
        }
        catch (const drogon::orm::DrogonDbException &e)
        {
            LOG_DEBUG << "catch:" << e.base().what();
            temp["response"] = Json::arrayValue;
            temp["status"] = 0;
        }
    }
    return temp;
}

Json::Value Dao::getUsers()
{
    LOG_DEBUG;
    Json::Value temp;
    int response {0};

    auto clientPtr = drogon::app().getDbClient();
    try
    {
        auto r = clientPtr->execSqlSync("SELECT * FROM sms.user, sms.vendor WHERE fk_vendor = id_vendor;");
        
        int i { 0 };
        Json::Value obj;
        for (auto const &row : r)
        {
            obj[i]["UserID"] = row["id_user"].as<std::string>();
            obj[i]["UserName"] = row["ds_user_name"].as<std::string>();
            obj[i]["Vendor"] = row["ds_vendor_name"].as<std::string>();
            obj[i]["Profile"] = row["ds_user_profile"].as<int>();
            obj[i]["Name"] = row["ds_profile_name"].as<std::string>();
            
            i++;
            response = 1;
        }
        temp["response"] = obj;       
    }
    catch (const drogon::orm::DrogonDbException &e)
    {
        LOG_DEBUG << "catch:" << e.base().what();
    }

    temp["status"] = response;

    return temp;
}

Json::Value Dao::getVendors()
{
    LOG_DEBUG;
    Json::Value temp, obj;

    auto clientPtr = drogon::app().getDbClient();
    try
    {
        auto r = clientPtr->execSqlSync("SELECT * FROM sms.vendor;");        
        int i {0};
        for (auto const &row : r)
        {
            obj[i]["VendorID"] = row["id_vendor"].as<std::string>();
            obj[i]["VendorName"] = row["ds_vendor_name"].as<std::string>();
            obj[i]["Secret"] = row["ds_vendor_token"].as<std::string>();
            obj[i]["UserSecret"] = row["ds_vendor_user_token"].as<std::string>();
            obj[i]["URL"] = row["ds_vendor_url"].as<std::string>();
                       
            i++;
        }
        temp["response"] = obj;
        temp["status"] = 1;
    }
    catch (const drogon::orm::DrogonDbException &e)
    {
        LOG_DEBUG << "catch:" << e.base().what();        
        temp["response"] = Json::arrayValue;;
        temp["status"] = 0;
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
        int i {0};
        
        for (auto const &row : r)
        {
            obj[i]["idUser"] = row["id_user"].as<std::string>();
            obj[i]["userName"] = row["ds_user_name"].as<std::string>();
            obj[i]["profile"] = row["ds_user_profile"].as<int>();
            obj[i]["name"] = row["ds_profile_name"].as<std::string>();
            obj[i]["vendor"] = row["ds_vendor_name"].as<std::string>();
           
            i++;
        }         
        if(r.affectedRows())
        {
            temp["response"] = obj;
            temp["status"] = 1;
        }
        else
        {
            temp["response"] = Json::arrayValue;
            temp["status"] = 1;
        }
    }
    catch (const drogon::orm::DrogonDbException &e)
    {
        LOG_DEBUG << "catch:" << e.base().what();
        temp["response"] = Json::arrayValue;
        temp["status"] = 0;
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
        int i {0};
        
        for (auto const &row : r)
        {
            obj[i]["idVendor"] = row["id_vendor"].as<std::string>();
            obj[i]["vendorName"] = row["ds_vendor_name"].as<std::string>();
            obj[i]["url"] = row["ds_vendor_url"].as<std::string>();
            obj[i]["userToken"] = row["ds_vendor_user_token"].as<std::string>();
           
            i++;
        }          
        if(r.affectedRows())
        {
            temp["response"] = obj;
            temp["status"] = 1;
        }
        else
        {
            temp["response"] = Json::arrayValue;
            temp["status"] = 1;
        }
    }
    catch (const drogon::orm::DrogonDbException &e)
    {
        LOG_DEBUG << "catch:" << e.base().what();
        temp["response"] = Json::arrayValue;
        temp["status"] = 0;
    }
    return temp;
}

std::string Dao::getUserToken(std::string &user)
{
    std::string token {""};

    auto clientPtr = drogon::app().getDbClient();
    try
    {
        auto r = clientPtr->execSqlSync("select ds_user_token from sms.user where ds_user_name=$1", user);        
        int i = 0;        
        for (auto const &row : r)
        {
            token = row["ds_user_token"].as<std::string>();
        }       
    }
    catch (const drogon::orm::DrogonDbException &e)
    {
        LOG_DEBUG << "catch:" << e.base().what();
    }

    return token;
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
