#pragma once
#include <drogon/HttpController.h>

using namespace drogon;

class AuthController : public drogon::HttpController<AuthController> {
public:
    METHOD_LIST_BEGIN
        ADD_METHOD_TO(AuthController::loginPage, "/login", Get);
        ADD_METHOD_TO(AuthController::registerPage, "/register", Get);
        ADD_METHOD_TO(AuthController::login, "/api/login", Post);
        ADD_METHOD_TO(AuthController::registerUser, "/api/register", Post);
        ADD_METHOD_TO(AuthController::logout, "/logout", Get);
    METHOD_LIST_END

    void loginPage(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback);
    void registerPage(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback);
    void login(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback);
    void registerUser(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback);
    void logout(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback);
};