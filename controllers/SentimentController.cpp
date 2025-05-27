#include "SentimentController.h"
#include <drogon/HttpResponse.h>
#include <json/json.h>

using namespace drogon;

void SentimentController::analyze(const HttpRequestPtr &req,
                                  std::function<void(const HttpResponsePtr &)> &&callback) {
    auto json = req->getJsonObject();
    if (!json || !json->isMember("text")) {
        auto resp = HttpResponse::newHttpResponse();
        resp->setStatusCode(k400BadRequest);
        callback(resp);
        return;
    }
    
    std::string text = (*json)["text"].asString();
    std::string sentiment = analyzer.predict(text);
    
    Json::Value ret;
    ret["sentiment"] = sentiment;
    auto resp = HttpResponse::newHttpJsonResponse(ret);
    callback(resp);
}

void SentimentController::sentimentPage(const HttpRequestPtr &req,
                                      std::function<void(const HttpResponsePtr &)> &&callback) {
    auto session = req->session();
    if (!session || !session->find("username")) {
        auto resp = HttpResponse::newRedirectionResponse("/login");
        callback(resp);
        return;
    }
    
    auto resp = HttpResponse::newFileResponse(
        "/home/jkz/project/new_sentiment_analyse_system/project_with_mysql/frontend/sentiment.html");
    callback(resp);
}


void SentimentController::resultPage(const HttpRequestPtr &req,
                                   std::function<void(const HttpResponsePtr &)> &&callback) {
    auto session = req->session();
    if (!session || !session->find("username")) {
        auto resp = HttpResponse::newRedirectionResponse("/login");
        callback(resp);
        return;
    }
    
    auto resp = HttpResponse::newFileResponse(
        "/home/jkz/project/new_sentiment_analyse_system/project_with_mysql/frontend/result.html");
    callback(resp);
}