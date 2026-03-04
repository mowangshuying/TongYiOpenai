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


    TongYiOpenAi::__init("https://dashscope.aliyuncs.com/api/v1/services/aigc/multimodal-generation/generation", 
        "sk-c43463e3702d452fa7d873efb7808eae");

    auto json = TongYiOpenAi::imageEdit().create(R"(
        {
            "model": "qwen-image-2.0-pro",
            "input": {
            "messages": [
                            {
                                "role": "user",
                                "content": [
                                    {
                                        "image": "https://help-static-aliyun-doc.aliyuncs.com/file-manage-files/zh-CN/20250925/fpakfo/image36.webp"
                                    },
                                    {
                                        "text": "生成一张符合深度图的图像，遵循以下描述：一辆红色的破旧的自行车停在一条泥泞的小路上，背景是茂密的原始森林"
                                    }
                                ]
                            }
                        ]
            },
            "parameters": {
                "n": 2,
                "negative_prompt": " ",
                "prompt_extend": true,
                "watermark": false,
                "size": "1536*1024"
            }
        }
        )"_json);
    hlogd("image generation: %s", json.dump().c_str());

    
    
    return 0;
}