#include "Validation.hpp"

bool validateRequest(const drogon::HttpRequestPtr &req)
{
    Dao dao;
    std::string header {req->getHeader("Authentication")};
    std::string user {header, 0 , header.find(":")};
    std::string token {header, header.find(":")+1, header.size()};

    std::string compareToken = dao.getUserToken(user);

    if(!compareToken.compare(""))
        return false;
    
    std::string tokenHolder {user + ":" + compareToken};
    if(!token.compare(sha1(tokenHolder)))
    {
        LOG_DEBUG;
        return true;
    }
    else
    {
        LOG_DEBUG;
        return false;        
    }    
}