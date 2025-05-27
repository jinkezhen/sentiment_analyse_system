// controllers/MainController.h
#pragma once

#include <drogon/HttpSimpleController.h>
#include <drogon/HttpController.h>

using namespace drogon;

class MainController : public drogon::HttpController<MainController> {
public:
    METHOD_LIST_BEGIN
        ADD_METHOD_TO(MainController::index, "/", Get);
        ADD_METHOD_TO(MainController::dashboard, "/dashboard", Get);
        ADD_METHOD_TO(MainController::getUserInfo, "/api/userinfo", Get);
    METHOD_LIST_END

    void index(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback);
    void dashboard(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback);
    void getUserInfo(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback);
};