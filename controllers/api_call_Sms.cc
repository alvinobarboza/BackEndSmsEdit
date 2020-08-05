#include "api_call_Sms.h"
using namespace api::call;
//add definition of your processing function here
//=====================================test get API =============================
void Sms::getUserSMS(const HttpRequestPtr &req,
                             std::function<void (const HttpResponsePtr &)> &&callback,
                             const std::string &search)const
{
    LOG_DEBUG << "get SMS";
    static bool check = true;
    Json::Value json, dataJ, searchJ, wild_search;

    wild_search["wild_search"] = search;
    searchJ["search"] = wild_search;
    dataJ["data"]=searchJ;

    
    //requestJson = dataJ;

    int count = 0;
    auto client = HttpClient::newHttpClient("https://sms.youcast.tv.br");
    auto requestH = HttpRequest::newHttpRequest();
    requestH->setMethod(drogon::Post);
    requestH->setPath("/api/customer/findCustomerForSales");
    requestH->addHeader("Authorization","alvino.barboza@youcast.tv.br:1596657548:9bb9bb1d669c22d5cc4525ca1add9d5ffc6db678");
  //  requestH->setContentTypeCode(CT_APPLICATION_JSON);
    requestH->newHttpJsonRequest(dataJ);

        
    auto a = client->sendRequest(requestH);

    json = *a.second->getJsonObject();

    
    
    std::cout << dataJ <<" end Json"<< sha1("teste") <<std::endl;

    auto resp=HttpResponse::newHttpJsonResponse(json);
    callback(resp);
    
}