// controllers/MainController.cc
#include "MainController.h"
#include <drogon/HttpResponse.h>
#include <drogon/HttpViewData.h>

using namespace drogon;

void MainController::index(const HttpRequestPtr &req, 
                          std::function<void(const HttpResponsePtr &)> &&callback) {
    auto resp = HttpResponse::newRedirectionResponse("/dashboard");
    callback(resp);
}

void MainController::dashboard(const HttpRequestPtr &req,
                             std::function<void(const HttpResponsePtr &)> &&callback) {
    auto session = req->session();
    if (!session || !session->find("username")) { // 添加session存在性检查
        auto resp = HttpResponse::newRedirectionResponse("/login");
        callback(resp);
        return;
    }
    
    try {
        HttpViewData data;
        data.insert("username", session->get<std::string>("username"));
        // 使用静态文件响应
        auto resp = HttpResponse::newFileResponse(
            "/home/jkz/project/new_sentiment_analyse_system/project_with_mysql/frontend/dashboard.html"
        );
        callback(resp);
    } catch (const std::exception &e) {
        LOG_ERROR << "Dashboard error: " << e.what();
        auto resp = HttpResponse::newRedirectionResponse("/login");
        callback(resp);
    }
}

void MainController::getUserInfo(const HttpRequestPtr &req,
                               std::function<void(const HttpResponsePtr &)> &&callback) {
    auto session = req->session();
    if (!session || !session->find("username")) {
        Json::Value ret;
        ret["error"] = "Not logged in";
        auto resp = HttpResponse::newHttpJsonResponse(ret);
        resp->setStatusCode(k401Unauthorized);
        callback(resp);
        return;
    }
    
    Json::Value ret;
    ret["username"] = session->get<std::string>("username");
    auto resp = HttpResponse::newHttpJsonResponse(ret);
    callback(resp);
}
