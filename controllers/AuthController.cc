// controllers/AuthController.cc
#include "AuthController.h"
#include "../models/User.h"
#include <drogon/utils/Utilities.h>
#include <drogon/HttpResponse.h>
#include <drogon/HttpViewData.h>

using namespace drogon;

// 修改loginPage方法
void AuthController::loginPage(const HttpRequestPtr &req,
                             std::function<void(const HttpResponsePtr &)> &&callback) {
    auto resp = HttpResponse::newFileResponse("/home/jkz/project/new_sentiment_analyse_system/project_with_mysql/frontend/auth/login.html");
    callback(resp);
}

// 修改registerPage方法
void AuthController::registerPage(const HttpRequestPtr &req,
                                std::function<void(const HttpResponsePtr &)> &&callback) {
    auto resp = HttpResponse::newFileResponse("/home/jkz/project/new_sentiment_analyse_system/project_with_mysql/frontend/auth/register.html");
    callback(resp);
}

void AuthController::login(const HttpRequestPtr &req, 
                          std::function<void(const HttpResponsePtr &)> &&callback) {
    auto json = req->getJsonObject();
    if (!json) {
        auto resp = HttpResponse::newHttpResponse();
        resp->setStatusCode(k400BadRequest);
        resp->setBody("Invalid request");
        callback(resp);
        return;
    }

    std::string username = (*json)["username"].asString();
    std::string password = (*json)["password"].asString();

    auto client = drogon::app().getDbClient();
    User::findUser(client, username,
        [=](const Result &result) {
            if (result.empty()) {
                Json::Value ret;
                ret["error"] = "User not found";
                auto resp = HttpResponse::newHttpJsonResponse(ret);
                resp->setStatusCode(k401Unauthorized);
                callback(resp);
                return;
            }

            auto user = result[0];
            std::string storedPassword = user["password"].as<std::string>();
            
            // 在实际应用中，这里应该使用密码哈希比较
            if (password == storedPassword) {
                Json::Value ret;
                ret["message"] = "Login successful";
                ret["user"] = User::toJson(user);
                
                auto resp = HttpResponse::newHttpJsonResponse(ret);
                
                // 创建会话
                req->session()->insert("username", username);
                req->session()->insert("user_id", user["id"].as<std::string>());
                
                callback(resp);
            } else {
                Json::Value ret;
                ret["error"] = "Invalid password";
                auto resp = HttpResponse::newHttpJsonResponse(ret);
                resp->setStatusCode(k401Unauthorized);
                callback(resp);
            }
        },
        [=](const DrogonDbException &e) {
            Json::Value ret;
            ret["error"] = "Database error";
            auto resp = HttpResponse::newHttpJsonResponse(ret);
            resp->setStatusCode(k500InternalServerError);
            callback(resp);
        });
}

void AuthController::registerUser(const HttpRequestPtr &req, 
                                std::function<void(const HttpResponsePtr &)> &&callback) {
    LOG_DEBUG << "Register request received";
    
    auto json = req->getJsonObject();
    if (!json) {
        LOG_ERROR << "Invalid JSON request";
        auto resp = HttpResponse::newHttpResponse();
        resp->setStatusCode(k400BadRequest);
        resp->setBody("Invalid request");
        callback(resp);
        return;
    }

    LOG_DEBUG << "Request JSON: " << json->toStyledString();
    
    std::string username = (*json)["username"].asString();
    std::string password = (*json)["password"].asString();
    std::string email = (*json)["email"].asString();

    LOG_DEBUG << "Registering user: " << username << ", " << email;

    auto client = drogon::app().getDbClient();
    User::findUser(client, username,
        [=](const Result &result) {
            if (!result.empty()) {
                Json::Value ret;
                ret["error"] = "Username already exists";
                auto resp = HttpResponse::newHttpJsonResponse(ret);
                resp->setStatusCode(k400BadRequest);
                callback(resp);
                return;
            }

            // 在实际应用中，这里应该对密码进行哈希处理
            User::createUser(client, username, password, email,
                [=](const Result &result) {
                    Json::Value ret;
                    ret["message"] = "Registration successful";
                    auto resp = HttpResponse::newHttpJsonResponse(ret);
                    callback(resp);
                },
                [=](const DrogonDbException &e) {
                    Json::Value ret;
                    ret["error"] = "Database error";
                    auto resp = HttpResponse::newHttpJsonResponse(ret);
                    resp->setStatusCode(k500InternalServerError);
                    callback(resp);
                });
        },
        [=](const DrogonDbException &e) {
            Json::Value ret;
            ret["error"] = "Database error";
            auto resp = HttpResponse::newHttpJsonResponse(ret);
            resp->setStatusCode(k500InternalServerError);
            callback(resp);
        });
}

void AuthController::logout(const HttpRequestPtr &req, 
                          std::function<void(const HttpResponsePtr &)> &&callback) {
    req->session()->erase("username");
    req->session()->erase("user_id");
    
    auto resp = HttpResponse::newRedirectionResponse("/login");
    callback(resp);
}