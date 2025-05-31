#pragma once

#include <drogon/HttpSimpleController.h>
#include <drogon/HttpController.h>
#include "../torch_infer/torch_infer.h"

using namespace drogon;

class BatchController : public drogon::HttpController<BatchController> {
public:
    METHOD_LIST_BEGIN
        ADD_METHOD_TO(BatchController::showBatchPage, "/batch", Get);
        ADD_METHOD_TO(BatchController::analyseBatch, "/batch/analyse", Post);
    METHOD_LIST_END

    void showBatchPage(const HttpRequestPtr &req, 
                      std::function<void(const HttpResponsePtr &)> &&callback);
    
    void analyseBatch(const HttpRequestPtr &req,
                     std::function<void(const HttpResponsePtr &)> &&callback);

private:
    // 解析上传的文本文件
    std::vector<std::string> parseTextFile(const std::string &fileContent);
    // 检查文件扩展名
    bool hasTxtExtension(const std::string& fileName);
};