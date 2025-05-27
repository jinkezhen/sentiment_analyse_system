// models/User.cc
#include "User.h"

void User::createUser(const DbClientPtr &client, 
                     const std::string &username,
                     const std::string &password,
                     const std::string &email,
                     std::function<void(const Result &)> &&callback,
                     std::function<void(const DrogonDbException &)> &&exceptCallback) {
    *client << "INSERT INTO users (username, password, email) VALUES (?, ?, ?)"
            << username
            << password
            << email
            >> std::move(callback)
            >> std::move(exceptCallback);
}

void User::findUser(const DbClientPtr &client,
                   const std::string &username,
                   std::function<void(const Result &)> &&callback,
                   std::function<void(const DrogonDbException &)> &&exceptCallback) {
    *client << "SELECT * FROM users WHERE username = ?"
            << username
            >> std::move(callback)
            >> std::move(exceptCallback);
}

Json::Value User::toJson(const Row &row) {
    Json::Value user;
    user["id"] = row["id"].as<int>();
    user["username"] = row["username"].as<std::string>();
    user["email"] = row["email"].as<std::string>();
    user["created_at"] = row["created_at"].as<std::string>();
    return user;
}