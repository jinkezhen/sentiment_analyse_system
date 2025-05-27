#pragma once
#include <drogon/HttpController.h>
#include "../torch_infer/torch_infer.h"

using namespace drogon;

class SentimentController : public drogon::HttpController<SentimentController> {
public:
    METHOD_LIST_BEGIN
        ADD_METHOD_TO(SentimentController::analyze, "/api/analyze", drogon::Post);
        ADD_METHOD_TO(SentimentController::sentimentPage, "/sentiment", drogon::Get);
        ADD_METHOD_TO(SentimentController::resultPage, "/result", drogon::Get); // 新增
    METHOD_LIST_END
    
    SentimentController() : analyzer("/home/jkz/project/new_sentiment_analyse_system/project_with_mysql/sa_models/bert.pt") {}
    
    void analyze(const drogon::HttpRequestPtr &req,
                 std::function<void(const drogon::HttpResponsePtr &)> &&callback);
    
    void sentimentPage(const drogon::HttpRequestPtr &req,
                      std::function<void(const drogon::HttpResponsePtr &)> &&callback);
    
    void resultPage(const drogon::HttpRequestPtr &req,
                   std::function<void(const drogon::HttpResponsePtr &)> &&callback); // 新增
private:
    TorchInfer analyzer;
};