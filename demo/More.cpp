#include <iostream>
#include <hv/hlog.h>
#include <TongYiOpenAi.hpp>


static void mylogger(int loglevel, const char* buf, int len) {
    stdout_logger(loglevel, buf, len);
     file_logger(loglevel, buf, len);
}

int main() {
    // std::cout << "Hello TongYiOpenai!" << std::endl;
#if _WIN32
   // 使用编码格式 utf-8
   system("chcp 65001");
#endif // _WIN32

    hlog_set_file("Log");
    hlog_set_format(DEFAULT_LOG_FORMAT);
    hlog_set_handler(mylogger);
    hlog_set_level(LOG_LEVEL_DEBUG);


    TongYiOpenAi::__init("https://dashscope.aliyuncs.com/compatible-mode/v1/chat/completions", 
        "sk-c43463e3702d452fa7d873efb7808eae");

    auto json = TongYiOpenAi::more().makeTempToken(60 * 10);
    hlogd("makeTempToken: %s", json.dump().c_str());

    
    
    return 0;
}