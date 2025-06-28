// #pragma once
// #include <drogon/HttpController.h>
// #include "../torch_infer/torch_infer.h"
// #include <drogon/orm/DbClient.h>
// #include <chrono>
// #include <ctime>

// using namespace drogon;
// using namespace drogon::orm;

// class CrawlerController : public drogon::HttpController<CrawlerController> {
// public:
//     METHOD_LIST_BEGIN
//         ADD_METHOD_TO(CrawlerController::showCrawlerPage, "/crawler", Get);
//         ADD_METHOD_TO(CrawlerController::crawlAndAnalyse, "/crawler/crawlandanalyse", Post);
//     METHOD_LIST_END

//     CrawlerController() : infer_("/home/jkz/project/new_sentiment_analyse_system/project_with_mysql/sa_models/bert.pt") {
//         db_ = drogon::app().getDbClient();
//     }

//     void showCrawlerPage(const HttpRequestPtr& req,
//                         std::function<void(const HttpResponsePtr&)>&& callback);
//     void crawlAndAnalyse(const HttpRequestPtr& req,
//                         std::function<void(const HttpResponsePtr&)>&& callback);

// private:
//     TorchInfer infer_;
//     DbClientPtr db_;
    
//     // 执行爬虫脚本
//     bool runCrawlerScript(const std::string& movie_id);
    
//     // 检查是否需要更新数据
//     std::pair<bool, double> checkMovieRecord(const std::string& movie_id);
    
//     // 获取评论数据
//     std::vector<std::string> getCommentsForMovie(const std::string& movie_id);
    
//     // 更新电影记录
//     void updateMovieRecord(const std::string& movie_id, double positive_ratio);
// };


#pragma once
#include <drogon/HttpController.h>
#include "../torch_infer/torch_infer.h"
#include <drogon/orm/DbClient.h>
#include <chrono>
#include <ctime>
#include <fstream>
#include <filesystem>  // 添加文件系统支持

using namespace drogon;
using namespace drogon::orm;

class CrawlerController : public drogon::HttpController<CrawlerController> {
public:
    METHOD_LIST_BEGIN
        ADD_METHOD_TO(CrawlerController::showCrawlerPage, "/crawler", Get);
        ADD_METHOD_TO(CrawlerController::crawlAndAnalyse, "/crawler/crawlandanalyse", Post);
    METHOD_LIST_END

    CrawlerController() 
        : infer_("/home/jkz/project/new_sentiment_analyse_system/project_with_mysql/sa_models/bert.pt"),
          data_dir_("/home/jkz/project/new_sentiment_analyse_system/project_with_mysql/crawler_data/")  // 设置数据目录
    {
        db_ = drogon::app().getDbClient();
        
        // 确保数据目录存在
        std::filesystem::create_directories(data_dir_);
    }

    void showCrawlerPage(const HttpRequestPtr& req,
                        std::function<void(const HttpResponsePtr&)>&& callback);
    void crawlAndAnalyse(const HttpRequestPtr& req,
                        std::function<void(const HttpResponsePtr&)>&& callback);

private:
    TorchInfer infer_;
    DbClientPtr db_;
    std::string data_dir_;  // 存储txt文件的目录
    
    // 执行爬虫脚本
    bool runCrawlerScript(const std::string& movie_id);
    
    // 检查是否需要更新数据
    std::pair<bool, double> checkMovieRecord(const std::string& movie_id);
    
    // 从txt文件获取评论数据
    std::vector<std::string> getCommentsFromFile(const std::string& movie_id);
    
    // 更新电影记录
    void updateMovieRecord(const std::string& movie_id, double positive_ratio);
};