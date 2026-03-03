#include <iostream>
#include <hv/hlog.h>
#include "TongYiOpenAi.hpp"


static void mylogger(int loglevel, const char* buf, int len) {
    stdout_logger(loglevel, buf, len);
    // file_logger(loglevel, buf, len);
}

int main() {
    // std::cout << "Hello TongYiOpenai!" << std::endl;
#if _WIN32
   // 使用编码格式 utf-8
   system("chcp 65001");
#endif // _WIN32

    hlog_set_file("logs/Log");
    hlog_set_format(DEFAULT_LOG_FORMAT);
    hlog_set_handler(mylogger);
    hlog_set_level(LOG_LEVEL_DEBUG);


    //hlogd("Hello TongYiOpenai!");
    //TongYiOpenAi::TongYiOpenAi tyOpenAi;

    // test getEnv
    // std::string path = tyOpenAi.getEnv("Path");
    // hlogd("Path: %s", path.c_str());

    //std::string specPath = tyOpenAi.getEnv("SpecPath");
    //hlogd("SpecPath: %s", specPath.c_str());

    TongYiOpenAi::__init("https://dashscope.aliyuncs.com/compatible-mode/v1/chat/completions", 
        "sk-c43463e3702d452fa7d873efb7808eae");
    
    // auto completion = TongYiOpenAi::completion().create(R"(
    //                 {
    //                     "model": "qwen3.5-plus",
    //                     "messages": [
    //                         {
    //                             "role": "system",
    //                             "content": "You are a helpful assistant."
    //                         },
    //                         {
    //                             "role": "user",
    //                             "content": "你好呀!"
    //                         }
    //                     ]
    //                 }
    //                 )"_json);

    // std::string dump = completion.dump();
    // hlogd("completion: %s", dump.c_str());

    auto json = TongYiOpenAi::modelDeployments().list();
    hlogd("modelDeployments: %s", json.dump().c_str());

    return 0;
}