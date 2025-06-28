// #include "CrawlerController.h"
// #include <drogon/HttpResponse.h>
// #include <drogon/HttpViewData.h>
// #include <drogon/utils/Utilities.h>
// #include <fstream>
// #include <sstream>
// #include <json/json.h>
// #include <algorithm> 
// #include <chrono>
// #include <ctime>
// #include <sys/wait.h>
// #include <unistd.h>

// using namespace drogon;
// using namespace drogon::orm;

// void CrawlerController::showCrawlerPage(const HttpRequestPtr& req,
//                                         std::function<void(const HttpResponsePtr&)>&& callback) {
//     auto session = req->session();
//     if (!session || !session->find("username")) {
//         auto resp = HttpResponse::newRedirectionResponse("/login");
//         callback(resp);
//         return;
//     }          
//     auto resp = HttpResponse::newFileResponse(
//         "/home/jkz/project/new_sentiment_analyse_system/project_with_mysql/frontend/crawler.html"
//     );
//     callback(resp);                             
// }

// bool CrawlerController::runCrawlerScript(const std::string& movie_id) {
//     pid_t pid = fork();
//     if (pid == 0) { // 子进程
//         // 构造命令
//         std::string command = "python3 /home/jkz/project/new_sentiment_analyse_system/project_with_mysql/scripts/DoubanCommentCrawler.py " + movie_id;
        
//         // 执行命令
//         execl("/bin/sh", "sh", "-c", command.c_str(), (char *)0);
//         exit(127); // 如果execl失败
//     } else if (pid < 0) { // fork失败
//         return false;
//     } else { // 父进程
//         int status;
//         waitpid(pid, &status, 0);
//         return WIFEXITED(status) && WEXITSTATUS(status) == 0;
//     }
// }

// std::pair<bool, double> CrawlerController::checkMovieRecord(const std::string& movie_id) {
//     try {
//         auto result = db_->execSqlSync("SELECT rating_ratio, created_at FROM movies WHERE movie_id = ?", movie_id);
//         if (!result.empty()) {
//             auto row = result[0];
//             double rating_ratio = row["rating_ratio"].as<double>();
//             std::string created_at_str = row["created_at"].as<std::string>();

//             std::tm created_tm = {};
//             std::istringstream ss(created_at_str);
//             ss >> std::get_time(&created_tm, "%Y-%m-%d %H:%M:%S");
//             if (ss.fail()) {
//                 LOG_ERROR << "Failed to parse created_at: " << created_at_str;
//                 return {false, 0.0};
//             }

//             // 计算时间差
//             auto now = std::chrono::system_clock::now();
//             auto created_time = std::chrono::system_clock::from_time_t(std::mktime(&created_tm));
//             auto diff = std::chrono::duration_cast<std::chrono::hours>(now - created_time).count();

//             // 检查是否在 7 天内 (7*24 = 168 小时)
//             if (diff < 168) {
//                 return {true, rating_ratio};
//             }
//         }
//         return {false, 0.0};
//     } catch (const DrogonDbException &e) {
//         LOG_ERROR << "Database error: " << e.base().what();
//         return {false, 0.0};
//     }
// }


// std::vector<std::string> CrawlerController::getCommentsForMovie(const std::string& movie_id) {
//     std::vector<std::string> comments;
//     try {
//         auto result = db_->execSqlSync("SELECT content FROM comments WHERE movie_id = ?", movie_id);
//         for (const auto &row : result) {
//             comments.push_back(row["content"].as<std::string>());
//         }
//     } catch (const DrogonDbException &e) {
//         LOG_ERROR << "Database error: " << e.base().what();
//     }
//     return comments;
// }

// void CrawlerController::updateMovieRecord(const std::string& movie_id, double positive_ratio) {
//     try {
//         db_->execSqlSync(
//             "INSERT INTO movies (movie_id, rating_ratio, created_at) "
//             "VALUES (?, ?, NOW()) "
//             "ON DUPLICATE KEY UPDATE rating_ratio = ?, created_at = NOW()",
//             movie_id, positive_ratio, positive_ratio
//         );
//     } catch (const DrogonDbException &e) {
//         LOG_ERROR << "Database error: " << e.base().what();
//     }
// }

// void CrawlerController::crawlAndAnalyse(const HttpRequestPtr& req,
//                                        std::function<void(const HttpResponsePtr&)>&& callback) {
//     auto session = req->session();
//     if (!session || !session->find("username")) {
//         Json::Value ret;
//         ret["error"] = "Not logged in";
//         auto resp = HttpResponse::newHttpJsonResponse(ret);
//         resp->setStatusCode(k401Unauthorized);
//         callback(resp);
//         return;
//     }
    
//     // 解析请求参数
//     auto json = req->getJsonObject();
//     if (!json || !json->isMember("movie_id")) {
//         Json::Value ret;
//         ret["error"] = "Missing movie_id";
//         auto resp = HttpResponse::newHttpJsonResponse(ret);
//         resp->setStatusCode(k400BadRequest);
//         callback(resp);
//         return;
//     }
    
//     std::string movie_id = (*json)["movie_id"].asString();
//     Json::Value response;
    
//     // 1. 检查数据库中是否已有记录且未过期
//     auto [hasRecord, rating_ratio] = checkMovieRecord(movie_id);
//     if (hasRecord) {
//         // 返回现有数据
//         auto comments = getCommentsForMovie(movie_id);
//         int total_count = comments.size();
//         int positive_count = static_cast<int>(rating_ratio * total_count);
        
//         response["status"] = "cached";
//         response["rating_ratio"] = rating_ratio;
//         response["total_count"] = total_count;
//         response["positive_count"] = positive_count;
        
//         auto resp = HttpResponse::newHttpJsonResponse(response);
//         callback(resp);
//         return;
//     }
    
//     // 2. 如果记录过期或不存在，删除旧评论
//     if (hasRecord) {
//         try {
//             db_->execSqlSync("DELETE FROM comments WHERE movie_id = ?", movie_id);
//         } catch (const DrogonDbException &e) {
//             LOG_ERROR << "Database error: " << e.base().what();
//         }
//     }
    
//     // 3. 执行爬虫脚本
//     if (!runCrawlerScript(movie_id)) {
//         response["error"] = "Failed to run crawler script";
//         auto resp = HttpResponse::newHttpJsonResponse(response);
//         resp->setStatusCode(k500InternalServerError);
//         callback(resp);
//         return;
//     }
    
//     // 4. 获取评论并进行分析
//     auto comments = getCommentsForMovie(movie_id);
//     if (comments.empty()) {
//         response["error"] = "No comments found after crawling";
//         auto resp = HttpResponse::newHttpJsonResponse(response);
//         resp->setStatusCode(k500InternalServerError);
//         callback(resp);
//         return;
//     }

//     try {
//         auto future = infer_.batch_predict(comments);
//         future.wait();
//         double positive_ratio = future.get();
//         int total_count = comments.size();
//         int positive_count = static_cast<int>(positive_ratio * total_count);
        
//         // 5. 更新数据库
//         updateMovieRecord(movie_id, positive_ratio);
        
//         // 6. 返回结果
//         response["status"] = "success";
//         response["rating_ratio"] = positive_ratio;
//         response["total_count"] = total_count;
//         response["positive_count"] = positive_count;
        
//         auto resp = HttpResponse::newHttpJsonResponse(response);
//         callback(resp);
//     } catch (const std::exception &e) {
//         response["error"] = "Analysis failed: " + std::string(e.what());
//         auto resp = HttpResponse::newHttpJsonResponse(response);
//         resp->setStatusCode(k500InternalServerError);
//         callback(resp);
//     }
// }


#include "CrawlerController.h"
#include <drogon/HttpResponse.h>
#include <drogon/HttpViewData.h>
#include <drogon/utils/Utilities.h>
#include <fstream>
#include <sstream>
#include <json/json.h>
#include <algorithm> 
#include <chrono>
#include <ctime>
#include <sys/wait.h>
#include <unistd.h>

using namespace drogon;
using namespace drogon::orm;

void CrawlerController::showCrawlerPage(const HttpRequestPtr& req,
                                        std::function<void(const HttpResponsePtr&)>&& callback) {
    auto session = req->session();
    if (!session || !session->find("username")) {
        auto resp = HttpResponse::newRedirectionResponse("/login");
        callback(resp);
        return;
    }          
    auto resp = HttpResponse::newFileResponse(
        "/home/jkz/project/new_sentiment_analyse_system/project_with_mysql/frontend/crawler.html"
    );
    callback(resp);                             
}

bool CrawlerController::runCrawlerScript(const std::string& movie_id) {
    pid_t pid = fork();
    if (pid == 0) { // 子进程
        // 确保输出目录存在
        std::filesystem::create_directories(data_dir_);
        
        // 构造命令，添加输出目录参数
        std::string command = "python3 /home/jkz/project/new_sentiment_analyse_system/project_with_mysql/scripts/DoubanCommentCrawler.py " + 
                              movie_id;
        
        // 执行命令
        execl("/bin/sh", "sh", "-c", command.c_str(), (char *)0);
        exit(127); // 如果execl失败
    } else if (pid < 0) { // fork失败
        return false;
    } else { // 父进程
        int status;
        waitpid(pid, &status, 0);
        return WIFEXITED(status) && WEXITSTATUS(status) == 0;
    }
}

std::pair<bool, double> CrawlerController::checkMovieRecord(const std::string& movie_id) {
    try {
        auto result = db_->execSqlSync("SELECT rating_ratio, created_at FROM movies WHERE movie_id = ?", movie_id);
        if (!result.empty()) {
            auto row = result[0];
            double rating_ratio = row["rating_ratio"].as<double>();
            std::string created_at_str = row["created_at"].as<std::string>();

            std::tm created_tm = {};
            std::istringstream ss(created_at_str);
            ss >> std::get_time(&created_tm, "%Y-%m-%d %H:%M:%S");
            if (ss.fail()) {
                LOG_ERROR << "Failed to parse created_at: " << created_at_str;
                return {false, 0.0};
            }

            // 计算时间差
            auto now = std::chrono::system_clock::now();
            auto created_time = std::chrono::system_clock::from_time_t(std::mktime(&created_tm));
            auto diff = std::chrono::duration_cast<std::chrono::hours>(now - created_time).count();

            // 检查是否在 7 天内 (7*24 = 168 小时)
            if (diff < 168) {
                return {true, rating_ratio};
            }
        }
        return {false, 0.0};
    } catch (const DrogonDbException &e) {
        LOG_ERROR << "Database error: " << e.base().what();
        return {false, 0.0};
    }
}

std::vector<std::string> CrawlerController::getCommentsFromFile(const std::string& movie_id) {
    std::vector<std::string> comments;
    std::string file_path = data_dir_ + movie_id + ".txt";
    
    // 检查文件是否存在
    if (!std::filesystem::exists(file_path)) {
        LOG_ERROR << "Comment file not found: " << file_path;
        return comments;
    }
    
    try {
        std::ifstream file(file_path);
        if (!file.is_open()) {
            LOG_ERROR << "Failed to open comment file: " << file_path;
            return comments;
        }
        
        std::string line;
        while (std::getline(file, line)) {
            // 跳过空行
            if (!line.empty()) {
                comments.push_back(line);
            }
        }
        
        LOG_INFO << "Loaded " << comments.size() << " comments from " << file_path;
        return comments;
    } catch (const std::exception &e) {
        LOG_ERROR << "Error reading comment file: " << e.what();
        return comments;
    }
}

void CrawlerController::updateMovieRecord(const std::string& movie_id, double positive_ratio) {
    try {
        db_->execSqlSync(
            "INSERT INTO movies (movie_id, rating_ratio, created_at) "
            "VALUES (?, ?, NOW()) "
            "ON DUPLICATE KEY UPDATE rating_ratio = ?, created_at = NOW()",
            movie_id, positive_ratio, positive_ratio
        );
    } catch (const DrogonDbException &e) {
        LOG_ERROR << "Database error: " << e.base().what();
    }
}

void CrawlerController::crawlAndAnalyse(const HttpRequestPtr& req,
                                       std::function<void(const HttpResponsePtr&)>&& callback) {
    auto session = req->session();
    if (!session || !session->find("username")) {
        Json::Value ret;
        ret["error"] = "Not logged in";
        auto resp = HttpResponse::newHttpJsonResponse(ret);
        resp->setStatusCode(k401Unauthorized);
        callback(resp);
        return;
    }
    
    // 解析请求参数
    auto json = req->getJsonObject();
    if (!json || !json->isMember("movie_id")) {
        Json::Value ret;
        ret["error"] = "Missing movie_id";
        auto resp = HttpResponse::newHttpJsonResponse(ret);
        resp->setStatusCode(k400BadRequest);
        callback(resp);
        return;
    }
    
    std::string movie_id = (*json)["movie_id"].asString();
    Json::Value response;
    
    // 1. 检查数据库中是否已有记录且未过期
    auto [hasRecord, rating_ratio] = checkMovieRecord(movie_id);
    if (hasRecord) {
        // 尝试从文件获取评论（如果文件存在）
        auto comments = getCommentsFromFile(movie_id);
        int total_count = comments.size();
        
        if (total_count == 0) {
            // 如果文件不存在或为空，使用数据库中的评分
            try {
                auto result = db_->execSqlSync("SELECT COUNT(*) AS count FROM comments WHERE movie_id = ?", movie_id);
                if (!result.empty()) {
                    total_count = result[0]["count"].as<size_t>();
                }
            } catch (...) {
                // 忽略错误
            }
        }
        
        int positive_count = static_cast<int>(rating_ratio * total_count);
        
        response["status"] = "cached";
        response["rating_ratio"] = rating_ratio;
        response["total_count"] = total_count;
        response["positive_count"] = positive_count;
        
        auto resp = HttpResponse::newHttpJsonResponse(response);
        callback(resp);
        return;
    }
    
    // 2. 执行爬虫脚本
    if (!runCrawlerScript(movie_id)) {
        response["error"] = "Failed to run crawler script";
        auto resp = HttpResponse::newHttpJsonResponse(response);
        resp->setStatusCode(k500InternalServerError);
        callback(resp);
        return;
    }
    
    // 3. 从txt文件获取评论数据
    auto comments = getCommentsFromFile(movie_id);
    if (comments.empty()) {
        response["error"] = "No comments found after crawling";
        auto resp = HttpResponse::newHttpJsonResponse(response);
        resp->setStatusCode(k500InternalServerError);
        callback(resp);
        return;
    }

    try {
        auto future = infer_.batch_predict(comments);
        future.wait();
        double positive_ratio = future.get();
        int total_count = comments.size();
        int positive_count = static_cast<int>(positive_ratio * total_count);
        
        // 4. 更新数据库（只保存分析结果）
        updateMovieRecord(movie_id, positive_ratio);
        
        // 5. 返回结果
        response["status"] = "success";
        response["rating_ratio"] = positive_ratio;
        response["total_count"] = total_count;
        response["positive_count"] = positive_count;
        
        auto resp = HttpResponse::newHttpJsonResponse(response);
        callback(resp);
    } catch (const std::exception &e) {
        response["error"] = "Analysis failed: " + std::string(e.what());
        auto resp = HttpResponse::newHttpJsonResponse(response);
        resp->setStatusCode(k500InternalServerError);
        callback(resp);
    }
}