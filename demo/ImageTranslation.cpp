#include <iostream>
#include <hv/hlog.h>
#include <TongYiOpenAi.hpp>

static void mylogger(int loglevel, const char* buf, int len) {
    stdout_logger(loglevel, buf, len);
     file_logger(loglevel, buf, len);
}

int main() {

#if _WIN32
   system("chcp 65001");
#endif // _WIN32

    hlog_set_file("Log");
    hlog_set_format(DEFAULT_LOG_FORMAT);
    hlog_set_handler(mylogger);
    hlog_set_level(LOG_LEVEL_DEBUG);


    TongYiOpenAi::__init("https://dashscope.aliyuncs.com/api/v1/services/aigc/image2image/image-synthesis", 
        "sk-c43463e3702d452fa7d873efb7808eae");

    // auto json = TongYiOpenAi::imageTranslation().create(R"(
    //     {
    //         "model": "qwen-mt-image",
    //         "input": {
    //             "image_url": "https://help-static-aliyun-doc.aliyuncs.com/file-manage-files/zh-CN/20250916/ordhsk/1.webp",
    //             "source_lang": "zh",
    //             "target_lang": "en",
    //             "ext": {
    //                 "config": {
    //                     "imageSegment": false
    //                 }
    //             }
    //         }
    //     }
    //     )"_json);
    // hlogd("image translation: %s", json.dump().c_str());

    // auto json = TongYiOpenAi::imageTranslation().queryTask("d427f90e-95c1-4fad-9e3e-fc8453630b4e");
    // hlogd("image translation query task:\n%s\n", json.dump().c_str());

    TongYiOpenAi::download("https://dashscope-a717.oss-cn-beijing.aliyuncs.com/1d/45/20260304/5d2be93d/6630c3fc-052b-4950-8538-baa51f085d70.jpg?Expires=1772693695&OSSAccessKeyId=LTAI5tPxpiCM2hjmWrFXrym1&Signature=AZ80eadQadSIC2xzJEZNxwZ%2B%2BBU%3D", "1.jpg");

    
    
    return 0;
}