#include "BatchSentimentController.h"
#include <drogon/HttpResponse.h>
#include <drogon/HttpViewData.h>
#include <drogon/utils/Utilities.h>
#include <fstream>
#include <sstream>
#include <json/json.h>
#include <algorithm>

using namespace drogon;

void BatchController::showBatchPage(const HttpRequestPtr &req,
                                  std::function<void(const HttpResponsePtr &)> &&callback) {
    auto session = req->session();
    if (!session || !session->find("username")) {
        auto resp = HttpResponse::newRedirectionResponse("/login");
        callback(resp);
        return;
    }
    
    auto resp = HttpResponse::newFileResponse(
        "/home/jkz/project/new_sentiment_analyse_system/project_with_mysql/frontend/batch.html"
    );
    callback(resp);
}

void BatchController::analyseBatch(const HttpRequestPtr &req,
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
    
    std::vector<std::string> texts;
    
    // 检查是否是multipart请求
    if (req->contentType() == drogon::ContentType::CT_MULTIPART_FORM_DATA) {
        MultiPartParser fileUpload;
        if (fileUpload.parse(req) == 0) {
            auto files = fileUpload.getFiles();
            if (!files.empty()) {
                auto &file = files[0];
                std::string fileContent(file.fileContent().data(), file.fileContent().size());
                std::string fileName = file.getFileName();
                
                // 检查文件扩展名
                if (hasTxtExtension(fileName)) {
                    texts = parseTextFile(fileContent);
                } else {
                    Json::Value ret;
                    ret["error"] = "Unsupported file type";
                    auto resp = HttpResponse::newHttpJsonResponse(ret);
                    callback(resp);
                    return;
                }
            }
        }
    } 
    // 处理JSON请求
    else if (req->contentType() == drogon::ContentType::CT_APPLICATION_JSON) {
        auto json = req->getJsonObject();
        if (json && json->isMember("texts")) {
            std::string textContent = (*json)["texts"].asString();
            std::istringstream iss(textContent);
            std::string line;
            while (std::getline(iss, line)) {
                if (!line.empty()) {
                    texts.push_back(line);
                }
            }
        }
    }
    
    if (texts.empty()) {
        Json::Value ret;
        ret["error"] = "No valid input provided";
        auto resp = HttpResponse::newHttpJsonResponse(ret);
        callback(resp);
        return;
    }
    
    // 调用批量预测
    try {
        TorchInfer infer("/home/jkz/project/new_sentiment_analyse_system/project_with_mysql/sa_models/bert.pt");
        auto future = infer.batch_predict(texts);
        future.wait();
        double positive_ratio = future.get();
        
        Json::Value ret;
        ret["positive_ratio"] = positive_ratio;
        ret["total"] = static_cast<int>(texts.size());
        ret["positive_count"] = static_cast<int>(positive_ratio * texts.size());
        auto resp = HttpResponse::newHttpJsonResponse(ret);
        callback(resp);
    } catch (const std::exception &e) {
        Json::Value ret;
        ret["error"] = "Prediction failed: " + std::string(e.what());
        auto resp = HttpResponse::newHttpJsonResponse(ret);
        resp->setStatusCode(k500InternalServerError);
        callback(resp);
    }
}

std::vector<std::string> BatchController::parseTextFile(const std::string &fileContent) {
    std::vector<std::string> texts;
    std::istringstream iss(fileContent);
    std::string line;
    while (std::getline(iss, line)) {
        if (!line.empty()) {
            texts.push_back(line);
        }
    }
    return texts;
}

bool BatchController::hasTxtExtension(const std::string& fileName) {
    if (fileName.size() < 4) return false;
    return fileName.substr(fileName.size() - 4) == ".txt";
}