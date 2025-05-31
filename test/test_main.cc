#define DROGON_TEST_MAIN
#include <drogon/drogon_test.h>
#include <drogon/drogon.h>
#include "torch_infer/torch_infer.h"
#include <future>

DROGON_TEST(BasicTest)
{
    // Add your tests here
}

int main(int argc, char** argv) 
{
    try {
        // 1. 初始化
        TorchInfer infer("/home/jkz/project/new_sentiment_analyse_system/project_with_mysql/sa_models/bert.pt");
        
        // 2. 预测
        // std::string text = "电影情节非常的老套了，没新鲜感";
        // std::string result = infer.predict(text);
        
        // std::cout << "文本: " << text << "\n预测结果: " << result << std::endl;
        // std::vector<std::string> texts = {
        //     "这个东西非常不好用",
        //     "这个东西非常好用",
        //     "这个质量挺不错的",
        //     "这个东西让人买了还想买",
        //     "推荐大家购买，东西质量不错",
        //     "这个政策我觉得是利民的",
        //     "这个政策的开展，大大提升了城市居民的出行便利程度",
        //     "这个衣服我才穿了10天就坏了，质量可想而知",
        //     "空调制冷不错，前台服务也还可以",
        //     "电影情节非常的老套了，没有一点新鲜感"
        // };
        // std::future<double> future = infer.batch_predict(texts);
        // std::cout << "好评率" << future.get();
        // auto statistical_infos = infer.get_statistical_info();
        // std::cout << "总共个数：" << statistical_infos.first << "  " << "好评个数：" << statistical_infos.second;

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}
