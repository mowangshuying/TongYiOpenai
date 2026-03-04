#include <iostream>
#include <hv/hlog.h>
#include <TongYiOpenAi.hpp>

// ref:https://bailian.console.aliyun.com/cn-beijing/?spm=5176.29597918.J_SEsSjsNv72yRuRFS2VknO.2.6a8f133cWAuOBo&tab=api#/api/?type=model&url=2975126
static void mylogger(int loglevel, const char* buf, int len) {
    stdout_logger(loglevel, buf, len);
     file_logger(loglevel, buf, len);
}

////----------------------- 请求参数 ------------------------------
//// 请求头
//// Content-Type string （必选）application/json
//// Authorization string（必选）请求身份认证。接口使用阿里云百炼API-Key进行身份认证。示例值：Bearer sk-xxxx。

//// 请求体
//// model string （必选）
//// input object （必选）
////       messages array （必选） 请求内容数组。当前仅支持单轮对话，数组内有且只有一个元素。
////            role string （必选）消息的角色。此参数必须设置为user。
////            content array （必选）
////                text string （必选） // 正向提示词用于描述您期望生成的图像内容、风格和构图。
                                        // 支持中英文，长度不超过800个字符，每个汉字、字母、数字或符号计为一个字符，超过部分会自动截断。
                                        // 示例值：一只坐着的橘黄色的猫，表情愉悦，活泼可爱，逼真准确。
                                        // 注意：仅支持传入一个text，不传或传入多个将报错。
//// parameters object （可选）
////    negative_prompt string  // 反向提示词，用于描述不希望在图像中出现的内容，对画面进行限制。
                                // 支持中英文，长度不超过500个字符，超出部分将自动截断。
                                // 示例值：低分辨率，低画质，肢体畸形，手指畸形，画面过饱和，蜡像感，人脸无细节，过度光滑，画面具有AI感。构图混乱。文字模糊，扭曲。
//// size string （可选）   // 输出图像的分辨率，格式为宽*高。
////                        // qwen-image-2.0系列模型：输出图像总像素需在512*512至2048*2048之间，默认分辨率为1024*1024
////                        // qwen-image-max、qwen-image-plus系列模型：默认分辨率为1664*928。可选的分辨率及其对应的图像宽高比例为：
                            // 1664*928（默认值）：16:9; 1472*1104：4:3 ; 1328*1328：1:1; 1104*1472：3:4; 928*1664：9:16;
//// n integer （可选） // 输出图像的数量，默认值为1。
////                   // 对于qwen-image-2.0系列模型，可选择输出1-6张图片。
////                   // 对于qwen-image-max、qwen-image-plus系列模型，此参数固定为1，设置其他值将导致报错。

//// prompt_extend bool （可选） // 是否开启 Prompt（提示词）智能改写功能。开启后模型将对正向提示词进行优化与润色。此功能不会修改反向提示词。
///                             // true：默认值，开启智能改写。如果希望图像内容更多样化，由模型补充细节，建议开启此选项。
///                             // false：关闭智能改写。如果图像细节更可控，建议关闭此选项，并参考文生图Prompt指南进行优化，

//// watermark bool （可选）// 是否在图像右下角添加 "Qwen-Image" 水印。默认值为 false。水印样式如下：
///  seed integer （可选）  // 随机数种子，取值范围[0,2147483647]。使用相同的seed参数值可使生成内容保持相对稳定。
////                       // 若不提供，算法将自动使用随机数种子。
////

//// ------------------------ 响应参数 ------------------------------ 
//// output object 任务输出信息。模型生成的输出内容。此数组仅包含一个元素。
////    choices array 模型生成的输出内容。此数组仅包含一个元素。
////        finish_reason string 任务停止原因，自然停止时为stop。
////        message object
////            role string
////            content array
////                image string 生成图像的 URL，图像格式为PNG。链接有效期为24小时，请及时下载并保存图像
////    task_metric object
////        TOTAL integer 总的任务数。
////        SUCCESS integer 任务状态为成功的任务数。
////        FAILED integer 任务状态为失败的任务数。
//// usage object 输出信息统计。只对成功的结果计数。
////    /// image_count integer 模型生成图像的数量，当前固定为1。
////    /// width integer 模型生成图像的宽度（像素）。
////    /// height integer 模型生成图像的高度（像素）。
//// request_id string 请求唯一标识。可用于请求明细溯源和问题排查。
//// code string 请求失败的错误码。请求成功时不会返回此参数，详情请参见错误信息。
//// message string 请求失败的详细信息。请求成功时不会返回此参数，详情请参见错误信息。
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


    TongYiOpenAi::__init("ttps://dashscope.aliyuncs.com/api/v1/services/aigc/multimodal-generation/generation", 
        "sk-c43463e3702d452fa7d873efb7808eae");

    auto json = TongYiOpenAi::imageGeneration().create(R"(
        {
            "model": "qwen-image-plus",
            "input": {
            "messages": [
                            {
                                "role": "user",
                                "content": [
                                    {
                                        "text": "一副典雅庄重的对联悬挂于厅堂之中，房间是个安静古典的中式布置，桌子上放着一些青花瓷，对联上左书“义本生知人机同道善思新”，右书“通云赋智乾坤启数高志远”， 横批“智启千问”，字体飘逸，在中间挂着一幅中国风的画作，内容是岳阳楼。"
                                    }
                                ]
                            }
                        ]
                    },
            "parameters": {
                "negative_prompt": "低分辨率，低画质，肢体畸形，手指畸形，画面过饱和，蜡像感，人脸无细节，过度光滑，画面具有AI感。构图混乱。文字模糊，扭曲。",
                "prompt_extend": true,
                "watermark": false,
                "size": "1024*1024"
            }
        }
        )"_json);
    hlogd("image generation: %s", json.dump().c_str());

    
    
    return 0;
}