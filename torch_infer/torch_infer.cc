#include "torch_infer.h"
#include <stdexcept>
#include <algorithm>

// 构造函数：加载模型
TorchInfer::TorchInfer(const std::string& model_path) {
    load_vocab("/home/jkz/project/new_sentiment_analyse_system/project_with_mysql/torch_infer/vocab.txt");
    try {
        // 加载TorchScript模型
        // model = torch::jit::load(model_path);
        model = torch::jit::load(model_path, torch::kCPU);
        model.eval();
    } catch (const c10::Error& e) {
        throw std::runtime_error("Failed to load model: " + std::string(e.what()));
    }
}

std::string TorchInfer::predict(const std::string& text) {
    auto [input_ids, attention_mask] = preprocess(text);
    auto outputs = model.forward({input_ids, attention_mask}).toTensor();
    
    float logit_0 = outputs[0][0].item<float>();
    float logit_1 = outputs[0][1].item<float>();
    
    // 根据Python测试结果调整方向
    return (logit_1 > logit_0) ? "积极" : "消极"; 
}

std::tuple<torch::Tensor, torch::Tensor> TorchInfer::preprocess(const std::string& text) {
    // 1. 分词
    auto tokens = tokenize(text);
    tokens.insert(tokens.begin(), "[CLS]");
    // 2. 转换为ID
    std::vector<int64_t> token_ids;
    for (const auto& token : tokens) {
        token_ids.push_back(get_token_id(token));
    }
    // 3. Padding/截断
    if (token_ids.size() > pad_size_) {
        token_ids.resize(pad_size_);
    } else {
        token_ids.resize(pad_size_, 0);
    }
    // 4. 创建attention mask
    std::vector<int64_t> mask(pad_size_, 0);
    int real_len = std::min(static_cast<int>(tokens.size()), pad_size_);
    std::fill(mask.begin(), mask.begin() + real_len, 1);
    // 5. 转换为Tensor
    auto options = torch::TensorOptions().dtype(torch::kInt64);
    return {
        torch::from_blob(token_ids.data(), {1, pad_size_}, options).clone(),
        torch::from_blob(mask.data(), {1, pad_size_}, options).clone()
    };
}

// 创建attention mask
torch::Tensor TorchInfer::create_attention_mask(int seq_len) {
    std::vector<int64_t> mask(pad_size_, 0);
    int real_len = std::min(seq_len, pad_size_);
    std::fill(mask.begin(), mask.begin() + real_len, 1);
    
    auto options = torch::TensorOptions().dtype(torch::kInt64);
    return torch::from_blob(mask.data(), {1, pad_size_}, options).clone();
}

std::vector<std::string> TorchInfer::tokenize(const std::string& text) {
    // 简单 UTF-8 单字分词，适用于中文字符按字切割
    std::vector<std::string> tokens;
    size_t i = 0;
    while (i < text.size()) {
        uint8_t c = text[i];
        size_t len = 1;
        if ((c & 0xF8) == 0xF0) len = 4;       // 四字节 UTF-8
        else if ((c & 0xF0) == 0xE0) len = 3;  // 三字节
        else if ((c & 0xE0) == 0xC0) len = 2;  // 两字节
        // else 默认 len=1（ASCII）

        tokens.push_back(text.substr(i, len));
        i += len;
    }
    return tokens;
}

void TorchInfer::load_vocab(const std::string& path) {
    std::ifstream file(path);
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open vocab file: " + path);
    }

    std::string line;
    int64_t index = 0;
    while (std::getline(file, line)) {
        // 去除换行符、前后空格
        line.erase(0, line.find_first_not_of(" \t\r\n"));
        line.erase(line.find_last_not_of(" \t\r\n") + 1);
        if (!line.empty()) {
            vocab[line] = index++;
        }
    }
}

int64_t TorchInfer::get_token_id(const std::string& token) {
    auto it = vocab.find(token);
    if (it != vocab.end()) {
        return it->second;
    }
    auto unk_it = vocab.find("[UNK]");
    if (unk_it != vocab.end()) {
        return unk_it->second;
    }
    return 0; // 若连 [UNK] 都找不到，返回 0 是兜底策略
}
