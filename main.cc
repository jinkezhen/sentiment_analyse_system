#include <drogon/drogon.h>
#include "controllers/MainController.h"
#include "controllers/SentimentController.h"
#include "controllers/AuthController.h"

int main() {
    drogon::app().setLogLevel(trantor::Logger::kTrace);
    drogon::app().enableSession(3600);
    drogon::app().loadConfigFile("/home/jkz/project/new_sentiment_analyse_system/project_with_mysql/config.json");
    
    drogon::app().registerHandler("/",
        [](const HttpRequestPtr &,
           std::function<void(const HttpResponsePtr &)> &&callback) {
            auto resp = HttpResponse::newRedirectionResponse("/login");
            callback(resp);
        });
    
    drogon::app().run();
    return 0;
}