#ifndef TORCHINFER_H
#define TORCHINFER_H

#include <torch/script.h>
#include <cstdint>
#include <string>
#include <vector>
#include <map>

class TorchInfer {
public:
    // 构造函数：加载模型
    explicit TorchInfer(const std::string& model_path);
    // 预测接口
    std::string predict(const std::string& text);

private:
    // 文本预处理：分词、转换为tensor等
    std::tuple<torch::Tensor, torch::Tensor> preprocess(const std::string& text);
    // 中文BERT分词（简化版，实际需要与Python端一致）
    std::vector<std::string> tokenize(const std::string& text);
    // 创建attention mask：根据实际序列长度生成mask
    torch::Tensor create_attention_mask(int seq_len);
    // 获取词汇表中token对应的id
    int64_t get_token_id(const std::string& token);
    // 加载词汇表文件到vocab中
    void load_vocab(const std::string& path);

private:
    // 词汇表映射：token -> id
    std::unordered_map<std::string, int64_t> vocab;
    // 加载的LibTorch模型（BERT + 分类器）
    torch::jit::script::Module model;
    // 统一的输入序列长度，需与Python训练脚本保持一致
    int pad_size_ = 200;
};

#endif // TORCHINFER_H