#ifndef TORCHINFER_H
#define TORCHINFER_H

#include <torch/script.h>
#include <unordered_map>
#include <cstdint>
#include <string>
#include <vector>
#include <queue>
#include <map>
#include <condition_variable>
#include <thread>
#include <future>
#include <atomic>
#include <mutex>

class TorchInfer {
public:
    // 构造函数：加载模型
    explicit TorchInfer(const std::string& model_path, size_t num_threads = std::thread::hardware_concurrency());
    ~TorchInfer();
    
    // 单文本预测
    std::string predict(const std::string& text);
    // 批量预测
    std::future<double> batch_predict(const std::vector<std::string>& texts);
    // 统计信息
    std::pair<int, int> get_statistical_info() const {
        return {total_processed.load(), positive_count.load()};
    }

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

    // 工作线程函数
    void worker_thread();
    // 处理单个文本并更新统计信息
    void process_single_text(const std::string& text);

private:
    // 词汇表映射：token -> id
    std::unordered_map<std::string, int64_t> vocab;
    // 加载的LibTorch模型（BERT + 分类器）
    torch::jit::script::Module model;
    // 统一的输入序列长度，需与Python训练脚本保持一致
    int pad_size = 200;

    // 线程池
    std::vector<std::thread> workers;
    // 任务队列
    std::queue<std::function<void()>> tasks;
    // 队列锁 防止竞争
    std::mutex queue_mutex;
    std::condition_variable condition;
    bool stop = false;

    // 已经处理的总数，积极的个数
    std::atomic<int> total_processed{0};
    std::atomic<int> positive_count{0};
    // 异步结果传递，存储最终计算的批处理好评率
    std::promise<double> batch_promise;
    std::mutex promise_mutex;
    // 记录当前批次处理的文本总数
    int current_batch_size = 0;
    // 确保只有一个线程设置最终结果
    std::mutex batch_mutex;

};

#endif // TORCHINFER_H