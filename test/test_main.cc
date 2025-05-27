#define DROGON_TEST_MAIN
#include <drogon/drogon_test.h>
#include <drogon/drogon.h>
#include "torch_infer/torch_infer.h"

DROGON_TEST(BasicTest)
{
    // Add your tests here
}

int main(int argc, char** argv) 
{
    // using namespace drogon;

    // std::promise<void> p1;
    // std::future<void> f1 = p1.get_future();

    // // Start the main loop on another thread
    // std::thread thr([&]() {
    //     // Queues the promise to be fulfilled after starting the loop
    //     app().getLoop()->queueInLoop([&p1]() { p1.set_value(); });
    //     app().run();
    // });

    // // The future is only satisfied after the event loop started
    // f1.get();
    // int status = test::run(argc, argv);

    // // Ask the event loop to shutdown and wait
    // app().getLoop()->queueInLoop([]() { app().quit(); });
    // thr.join();
    // return status;
    try {
        // 1. 初始化
        TorchInfer infer("/home/jkz/project/new_sentiment_analyse_system/new_project/sa_models/bert.pt");
        
        // 2. 预测
        std::string text = "你们就买吧，一买一个不吱声";
        std::string result = infer.predict(text);
        
        std::cout << "文本: " << text << "\n预测结果: " << result << std::endl;
        
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}
