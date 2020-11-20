#include "Sms_user_DAO.h"

Json::Value SmsDAO::saveUser(Json::Value &request)
{
    LOG_DEBUG;
    int response;
    Json::Value temp; 
    LOG_DEBUG;
    
    auto clientPtr = drogon::app().getDbClient();
    try
    {
        
       
        auto r = clientPtr->execSqlSync("INSERT INTO sms.customer(ds_customer_name, ds_customer_lastname, ds_customer_login, ds_customer_profile_name, dt_customer_birthdate, ds_customer_sms_pair_id, ds_contact_email, ds_contact_phone1, ds_contact_phone2, ds_customer_sms_pair_id) "
                                        "VALUES ($1, $2, $3, $4, to_date($5, 'DDMMYYYY'), $6, $7, $8, $9, $10);",
                                        request["name"].asString(),
                                        request["lastname"].asString(),
                                        request["login"].asString(),
                                        request["profilename"].asString(),
                                        request["birthdate"].asString(),
                                        request["idsms"].asString(),                                            
                                        request["email"].asString(),
                                        request["tel1"].asString(),
                                        request["tel2"].asString()
                                        request["smsID"].asString());
        response = 1;
        LOG_DEBUG;
       
    }
    catch (const drogon::orm::DrogonDbException &e)
    {
        LOG_DEBUG << "catch:" << e.base().what();
        response = 0;
    }

    LOG_DEBUG;
    temp["response"] = response;
    return temp;
}

Json::Value SmsDAO::updateUser(Json::Value &request)
{
    LOG_DEBUG;
    std::string response;
    Json::Value temp; 
    
    if(isEmpty(request))
    {
        temp["response"] = "Algum campo vazio";
        temp["requestSent"] = request;
        return temp;
    }
    else
    {
        auto clientPtr = drogon::app().getDbClient();
        try
        {

            auto r = clientPtr->execSqlSync("UPDATE sms.customer SET "
                                            "ds_customer_name = CASE WHEN $2 = '' THEN ds_customer_name ELSE $2 END, "
                                            "ds_customer_lastname = CASE WHEN $3 = '' THEN ds_customer_lastname ELSE $3 END, "
                                            "ds_customer_login = CASE WHEN $4 = '' THEN ds_customer_login ELSE $4 END, "
                                            "ds_customer_profile_name = CASE WHEN $5 = '' THEN ds_customer_profile_name ELSE $5 END, "
                                            "dt_customer_birthdate = CASE WHEN $6 = '' THEN dt_customer_birthdate ELSE to_date($6, 'DDMMYYYY') END "
                                            "WHERE CAST(id_customer AS CHAR(10)) = $1;",
                                            request["id"].asString(),
                                            request["name"].asString(),
                                            request["lastname"].asString(),
                                            request["login"].asString(),
                                            request["profilename"].asString(),
                                            request["birthdate"].asString());
            response = "Customer atualizado com sucesso!";

            //Begin----------------------save contacts---------------------------
            std::cout << r.affectedRows() << std::endl;
            if(r.affectedRows() > 0)
            {                
                auto r1 = clientPtr->execSqlSync("UPDATE sms.contact SET "
                                                "ds_contact_email = CASE WHEN $1 = '' THEN ds_contact_email ELSE $1 END, "
                                                "ds_contact_phone1 = CASE WHEN $2 = '' THEN ds_contact_phone1 ELSE $1 END, "
                                                "ds_contact_phone2 = CASE WHEN $3 = '' THEN ds_contact_phone2 ELSE $1 END "
                                                "WHERE id_contact = (SELECT fk_contact FROM sms.customer WHERE CAST(id_customer AS CHAR(10)) = $4);",
                                                request["email"].asString(),
                                                request["tel1"].asString(),
                                                request["tel2"].asString(),
                                                request["id"].asString());
            }
            //End----------------------------------------------------------------
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

Json::Value SmsDAO::getUsers()
{
    LOG_DEBUG;
    Json::Value temp;
    std::string response;

    auto clientPtr = drogon::app().getDbClient();
    try
    {
        auto r = clientPtr->execSqlSync("SELECT "
                                        "id_customer, "
                                        "ds_customer_name, "
                                        "ds_customer_lastname, "
                                        "ds_customer_login, "
                                        "ds_customer_profile_name, "
                                        "to_char(dt_customer_birthdate, 'DD-MM-YYYY') dt_customer_birthdate, "
                                        "ds_contact_email, "
                                        "ds_contact_phone1, "
                                        "ds_contact_phone2 "
                                        "FROM sms.customer ");
        //LOG_DEBUG;
        int i = 0;
        std::string count;
        for (auto const &row : r)
        {

            Json::Value obj;
            
            obj["userid"]               = row["id_customer"].as<std::string>();
            obj["name"]                 = row["ds_customer_name"].as<std::string>();
            obj["lastname"]             = row["ds_customer_lastname"].as<std::string>();
            obj["login"]                = row["ds_customer_login"].as<std::string>();
            obj["profilename"]          = row["ds_customer_profile_name"].as<std::string>();            
            obj["birthdate"]            = row["dt_customer_birthdate"].as<std::string>();
            obj["email"]                = row["ds_contact_email"].as<std::string>();
            obj["tel1"]                 = row["ds_contact_phone1"].as<std::string>();
            obj["tel2"]                 = row["ds_contact_phone2"].as<std::string>();


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

Json::Value SmsDAO::getUserById(std::string &id)
{
    LOG_DEBUG;
    Json::Value temp;
    std::string response;

    auto clientPtr = drogon::app().getDbClient();
    try
    {
        auto r = clientPtr->execSqlSync("SELECT "
                                        "id_customer, "
                                        "ds_customer_name, "
                                        "ds_customer_lastname, "
                                        "ds_customer_login, "
                                        "ds_customer_profile_name, "
                                        "to_char(dt_customer_birthdate, 'DD-MM-YYYY') dt_customer_birthdate, "
                                        "ds_contact_email, "
                                        "ds_contact_phone1, "
                                        "ds_contact_phone2 "
                                        "FROM sms.customer "
                                        "WHERE CAST(id_customer AS CHAR(10)) = $1;",
                                        id);
        //LOG_DEBUG;
        int i = 0;
        std::string count;
        for (auto const &row : r)
        {

            Json::Value obj;
            
            obj["userid"]               = row["id_customer"].as<std::string>();
            obj["name"]                 = row["ds_customer_name"].as<std::string>();
            obj["lastname"]             = row["ds_customer_lastname"].as<std::string>();
            obj["login"]                = row["ds_customer_login"].as<std::string>();
            obj["profilename"]          = row["ds_customer_profile_name"].as<std::string>();            
            obj["birthdate"]            = row["dt_customer_birthdate"].as<std::string>();
            obj["email"]                = row["ds_contact_email"].as<std::string>();
            obj["tel1"]                 = row["ds_contact_phone1"].as<std::string>();
            obj["tel2"]                 = row["ds_contact_phone2"].as<std::string>();


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

Json::Value SmsDAO::getUserByMotvId(std::string &id)
{
    LOG_DEBUG;
    Json::Value temp;
    std::string response;

    auto clientPtr = drogon::app().getDbClient();
    try
    {
        auto r = clientPtr->execSqlSync("SELECT "
                                        "id_customer, "
                                        "ds_customer_name, "
                                        "ds_customer_lastname, "
                                        "ds_customer_login, "
                                        "ds_customer_profile_name, "
                                        "to_char(dt_customer_birthdate, 'DD-MM-YYYY') dt_customer_birthdate, "
                                        "ds_contact_email, "
                                        "ds_contact_phone1, "
                                        "ds_contact_phone2, "
                                        "ds_customer_sms_pair_id "
                                        "FROM sms.customer "
                                        "WHERE CAST(ds_customer_sms_pair_id AS CHAR(10)) = $1;",
                                        id);
        //LOG_DEBUG;
        int i = 0;
        std::string count;
        for (auto const &row : r)
        {
            Json::Value obj;
            
            obj["userid"]               = row["id_customer"].as<std::string>();
            obj["userSMSid"]            = row["ds_customer_sms_pair_id"].as<std::string>();
            obj["name"]                 = row["ds_customer_name"].as<std::string>();
            obj["lastname"]             = row["ds_customer_lastname"].as<std::string>();
            obj["login"]                = row["ds_customer_login"].as<std::string>();
            obj["profilename"]          = row["ds_customer_profile_name"].as<std::string>();            
            obj["birthdate"]            = row["dt_customer_birthdate"].as<std::string>();
            obj["email"]                = row["ds_contact_email"].as<std::string>();
            obj["tel1"]                 = row["ds_contact_phone1"].as<std::string>();
            obj["tel2"]                 = row["ds_contact_phone2"].as<std::string>();

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

Json::Value SmsDAO::searchUsers(Json::Value &request)
{
    LOG_DEBUG;
    Json::Value temp;
    std::string response;

    auto clientPtr = drogon::app().getDbClient();
    try
    {
        auto r = clientPtr->execSqlSync("SELECT "
                                        "id_customer, "
                                        "ds_customer_name, "
                                        "ds_customer_lastname, "
                                        "ds_customer_login, "
                                        "ds_customer_profile_name, "
                                        "to_char(dt_customer_birthdate, 'DD-MM-YYYY') dt_customer_birthdate, "
                                        "ds_contact_email, "
                                        "ds_contact_phone1, "
                                        "ds_contact_phone2 "
                                        "FROM sms.customer WHERE ds_customer_name ilike '%"+request["search"].asString()+"%' "
                                        "or ds_customer_lastname ilike '%"+request["search"].asString()+"%' "
                                        "or ds_customer_profile_name ilike '%"+request["search"].asString()+"%';");
        
         //LOG_DEBUG;
        int i = 0;
        std::string count;
        for (auto const &row : r)
        {

            Json::Value obj;
            
            obj["userid"]               = row["id_customer"].as<std::string>();
            obj["name"]                 = row["ds_customer_name"].as<std::string>();
            obj["lastname"]             = row["ds_customer_lastname"].as<std::string>();
            obj["login"]                = row["ds_customer_login"].as<std::string>();
            obj["profilename"]          = row["ds_customer_profile_name"].as<std::string>();            
            obj["birthdate"]            = row["dt_customer_birthdate"].as<std::string>();
            obj["email"]                = row["ds_contact_email"].as<std::string>();
            obj["tel1"]                 = row["ds_contact_phone1"].as<std::string>();
            obj["tel2"]                 = row["ds_contact_phone2"].as<std::string>();


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

bool isEmpty(Json::Value &value)
{
    bool validation = false;
    if(value["name"].asString() == ""|| 
        value["lastname"].asString() == ""|| 
        value["login"].asString() == ""||
        value["profilename"].asString() == ""||
        value["birthdate"].asString() == "")
        {
            validation = true;
        }
    return validation;
}