// models/User.h
#pragma once

#include <drogon/orm/Result.h>
#include <drogon/orm/Row.h>
#include <drogon/orm/Field.h>
#include <drogon/orm/DbClient.h>
#include <drogon/HttpRequest.h>
#include <json/json.h>

using namespace drogon::orm;

class User {
public:
    static void createUser(const DbClientPtr &client, 
                         const std::string &username,
                         const std::string &password,
                         const std::string &email,
                         std::function<void(const drogon::orm::Result &)> &&callback,
                         std::function<void(const drogon::orm::DrogonDbException &)> &&exceptCallback);
    
    static void findUser(const DbClientPtr &client,
                        const std::string &username,
                        std::function<void(const drogon::orm::Result &)> &&callback,
                        std::function<void(const drogon::orm::DrogonDbException &)> &&exceptCallback);
    
    static Json::Value toJson(const Row &row);
};