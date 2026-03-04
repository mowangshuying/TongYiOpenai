#ifndef TongYiOpenapi_hpp
#define TongYiOpenapi_hpp

#include <hv/hlog.h>
#include <hv/json.hpp>
#include <string>
#include <cstdlib>
#include <vector>
#include <list>
#include <hv/requests.h>

//// chat:
//// openai chat:       https://dashscope.aliyuncs.com/compatible-mode/v1/chat/completions               
//// OpenAi Responses:  https://dashscope.aliyuncs.com/api/v2/apps/protocols/compatible-mode/v1/responses
//// DashScope:         https://dashscope.aliyuncs.com/api/v1/services/aigc/text-generation/generation

//// image generation:  
//// 文生图:    https://dashscope.aliyuncs.com/api/v1/services/aigc/multimodal-generation/generation
//// 图像编辑:  https://dashscope.aliyuncs.com/api/v1/services/aigc/multimodal-generation/generation
//// 图像翻译:  https://dashscope.aliyuncs.com/api/v1/services/aigc/image2image/image-synthesis

//// 专项模型
//// Qwen-MT: 
////    sdk:    https://dashscope.aliyuncs.com/compatible-mode/v1
////    http:   https://dashscope.aliyuncs.com/compatible-mode/v1/chat/completions 


//// chat:    qwen3-max, qwen3.5-plus, qwen-plus, qwen3.5-flash, qwen-flash, qwen-turbo
//// 文生图：  qwen-image-max, qwen-image-plus, qwen-image
//// 图像编辑：qwen-image-edit-max, qwen-image-edit-plus, qwen-image-edit
//// 图像翻译：qwen-mt-image
//// 专项模型: qwen-mt-plus(翻译模型)

/// using area;
using Json = nlohmann::json;

/// struct;
namespace TongYiOpenAi {
namespace __detail {
    
class TongYiOpenAi;



/// 文本补全或者聊天补全
class CategoryCompletion {
    public:
        CategoryCompletion(TongYiOpenAi &openai) : m_openAi{openai}
        {

        }

        // void setHttpUrl(std::string url)
        // {
        //     m_httpUrl = url;
        // }

        /// 直接填写sdk的url;
        Json create(Json input);
    protected:
        TongYiOpenAi& m_openAi;
        // std::string m_httpUrl = "https://dashscope.aliyuncs.com/compatible-mode/v1/chat/completions";
};

/// 文生图
class CategoryImageGeneration {
    public:
        CategoryImageGeneration(TongYiOpenAi &openai) : m_openAi{openai} {}

        Json create(Json input);

        /// 模型概览
        std::list<std::string> list();


    protected:
        TongYiOpenAi& m_openAi;
};

/// 图像编辑
class CategoryImageEdit {
    public:
        CategoryImageEdit(TongYiOpenAi &openai) : m_openAi{openai} {}
        Json create(Json input);
        /// 模型概览
        std::list<std::string> list();
    protected:
        TongYiOpenAi& m_openAi;
};

/// 图像翻译
class CategoryImageTranslation {
    public:
        CategoryImageTranslation(TongYiOpenAi &openai) : m_openAi{openai} {}

        Json create(Json input);
        
        /// 模型概览
        std::list<std::string> list();
        
        Json queryTask(std::string taskId);

    protected:
        TongYiOpenAi& m_openAi;
};

/// 模型部署相关接口
class CategoryModelDeployments {
public:
    CategoryModelDeployments(TongYiOpenAi& openai) : m_openai(openai) {}

    Json list();

    Json list(int pageNo, int pageSize);
protected:
    TongYiOpenAi& m_openai;
};

/// 更多
class CategoryMore {
public:
    CategoryMore(TongYiOpenAi& openai) : m_openai(openai) {}
    // 创建零时key;
	Json makeTempToken(int expire_in_seconds);
protected:
    TongYiOpenAi& m_openai;
};


class TongYiOpenAi {
    public:
         // https://bailian.console.aliyun.com/cn-beijing/?spm=5176.12818093_47.overview_recent.1.3ad216d0Zif3ri&tab=doc#/doc/?type=model&url=2840914
    public:
        TongYiOpenAi() {
        }
        TongYiOpenAi(std::string httpUrl, std::string token) {
            m_token = token;
            m_httpUrl = httpUrl;
            // hlogi("TongYiOpenapi::TongYiOpenapi()");
        }

        TongYiOpenAi(const TongYiOpenAi&) = delete;

        ~TongYiOpenAi() {
            // hlogi("TongYiOpenapi::~TongYiOpenapi()");
        }

        void __init(){
            hlogi("TongYiOpenapi::__init()");
            // __DASHSCOPE_API_KEY = getEnv("DASHSCOPE_API_KEY");
        }

        std::string Token() {
            return m_token;
        }

        void setToken(std::string token) {
            m_token = token;
        }

        std::string getHttpUrl() {
            return m_httpUrl;
        }

        void setHttpUrl(std::string httpUrl) {
            m_httpUrl = httpUrl;
        }


        void sayHello() {
            hlogi("TongYiOpenapi::sayHello()");
        }

        // get env by k;
        std::string getEnv(std::string k) {
            // hlogi("TongYiOpenapi::getEnv()"); 
            const char* v = std::getenv(k.c_str());
            if (v == NULL) {
                return "";
            }

            return v;
        }
        
        //// simple http request;
        // curl -X POST https://dashscope.aliyuncs.com/compatible-mode/v1/chat/completions \
        // -H "Authorization: Bearer $DASHSCOPE_API_KEY" \
        // -H "Content-Type: application/json" \
        // -d '{
        //     "model": "qwen-plus",
        //     "messages": [
        //         {
        //             "role": "system",
        //             "content": "You are a helpful assistant."
        //         },
        //         {
        //             "role": "user",
        //             "content": "你是谁？"
        //         }
        //     ]
        // }'
        /// post;
        Json post(Json input)
        {
			return post(m_httpUrl, "application/json", input);
        }


        Json post(std::string httpurl, std::string contentType, Json input)
        {
            return post(httpurl, Json{}, contentType, input);
        }

        Json post(std::string httpurl,  std::string contentType, Json headers, Json input)
        {
             /// 判断流式和非流式输出;
            http_headers httpheaders;
            httpheaders["Authorization"] = std::string("Bearer ") + m_token;
            for (auto header : headers.items())
            {
                httpheaders[header.key()] = header.value();
            }

            if (contentType != "")
            {
                httpheaders["Content-Type"] = contentType;
            }

            /// 判断input中是否含有"stream"字段;
            
            bool bStream = false;
            if (input.contains("stream"))
            {
                bStream = input["stream"];
            }

            // headers["Content-Type"] = "application/json";
            requests::Response  resp = requests::post(httpurl.c_str(), input.dump(), httpheaders);
            //// resp is null
            if (resp == NULL) {
                hloge("TongYiOpenapi::post() - http error");
                return Json{};
            }

            if (resp->status_code != 200) {
                hloge("TongYiOpenapi::post() - http error: %d", resp->status_code);
            }

            std::string resptext = resp->body;

            Json json{};
            if (!bStream)
            {
                if (isJson(resptext)) {
                    json = Json::parse(resptext);
                }
            }
            
            if (bStream)
            {
                std::list<std::string> lines;
                for (size_t i = 0; i < resptext.size(); i++) {
                    if (resptext[i] == '\n' && resptext[i + 1] == '\n') {
                        //// 丢弃前面6个字符"data: "
                        lines.push_back(resptext.substr(6, i - 6));
                        resptext = resptext.substr(i + 2);
                        i = 0;
                    }
                }

                //return lines;
                json =  Json(lines);
            }

            return json;

        }

        /// get;
        Json get(std::string httpurl)
        {
            http_headers headers;
            headers["Authorization"] = std::string("Bearer ") + m_token;
            requests::Response  resp = requests::get(httpurl.c_str(), headers);

            if (resp == NULL) {
                hloge("TongYiOpenapi::get() - http error");
                return Json{};
            }

            if (resp->status_code != 200) {
                hloge("TongYiOpenapi::get() - http error: %d", resp->status_code);
            }

            std::string resptext = resp->body;
            Json json{};
            if (isJson(resptext)) {
                json = Json::parse(resptext);
            }
            else {
                hloge("TongYiOpenapi::get() - json parse error");
            }
            return json;
        }

        /// download file;
        size_t download(std::string httpurl, std::string path)
        {
             return requests::downloadFile(httpurl.c_str(), path.c_str());
        }

        /// isJson
        bool isJson(const std::string &data){
            bool rc = true;
            try {
                auto json = Json::parse(data); // throws if no json 
            }
            catch (std::exception &){
                rc = false;
            }
            return(rc);
        }
    public:
        CategoryCompletion completion{*this};
        CategoryImageGeneration imageGeneration{*this};
        CategoryImageEdit imageEdit{*this};
        CategoryImageTranslation imageTranslation{*this};
        CategoryModelDeployments modelDeployments{*this};
        CategoryMore more{*this};
    protected:
        std::string m_token;
        std::string m_httpUrl;

        /// 内置httpUrl;

}; /// class TongYiOpenAi;



Json CategoryCompletion::create(Json input)
{
    return m_openAi.post(input);
}

Json CategoryImageGeneration::create(Json input)
{
    return m_openAi.post(input);
}

std::list<std::string> CategoryImageGeneration::list() {
    std::list<std::string> models;
    models.push_back("qwen-image-max");
    models.push_back("qwen-image-plus");
    models.push_back("qwen-image");
    return models;
}

Json CategoryImageEdit::create(Json input)
{
    return m_openAi.post(input);
}

std::list<std::string> CategoryImageEdit::list() {
    std::list<std::string> models;
    models.push_back("qwen-image-edit-max");
    models.push_back("qwen-image-edit-plus");
    models.push_back("qwen-image-edit");
    return models;
}

Json CategoryImageTranslation::create(Json input)
{
    Json j;
    j["X-DashScope-Async"] = "enable";
    return m_openAi.post(m_openAi.getHttpUrl(), "application/json", j, input);
}

Json CategoryImageTranslation::queryTask(std::string taskId)
{
    return m_openAi.get("https://dashscope.aliyuncs.com/api/v1/tasks/" + taskId);
}

std::list<std::string> CategoryImageTranslation::list() {
    std::list<std::string> models;
    models.push_back("qwen-mt-image");
    return models;
}

//curl - X POST "https://dashscope.aliyuncs.com/api/v1/tokens?expire_in_seconds=1800" \
//- H "Authorization: Bearer $DASHSCOPE_API_KEY"
//临时 API Key 默认有效期为60秒，支持设置超时时间范围为[1, 1800]秒。
Json CategoryMore::makeTempToken(int expire_in_seconds)
{
    std::string httpurl = "https://dashscope.aliyuncs.com/api/v1/tokens?expire_in_seconds=" + std::to_string(expire_in_seconds);
    //return m_openai.get(httpurl);
    return m_openai.post(httpurl, "", Json{});
}

Json CategoryModelDeployments::list()
{
    // https://dashscope.aliyuncs.com/api/v1/deployments/models
    return m_openai.get("https://dashscope.aliyuncs.com/api/v1/deployments/models");
}

Json CategoryModelDeployments::list(int pageNo, int pageSize)
{
    // https://dashscope.aliyuncs.com/api/v1/deployments/models?pageNo=1&pageSize=10
    std::string httpurl = "https://dashscope.aliyuncs.com/api/v1/deployments/models?pageNo=" + std::to_string(pageNo) + "&pageSize=" + std::to_string(pageSize);
    return m_openai.get(httpurl);
}


TongYiOpenAi& instance() {
   static TongYiOpenAi _instance;
   return _instance;
}

void __init(std::string httpurl, std::string token)
{
    instance().setHttpUrl(httpurl);
    instance().setToken(token);
}

void __initByEnv()
{
    std::string httpurl = instance().getEnv("TongYiOpenAiHttpUrl");
    std::string token = instance().getEnv("TongYiOpenAiToken");
    __init(httpurl, token);
}

/// post;
inline Json post(Json input) {
    return instance().post(input);
}

/// get;
inline Json get(Json input) {
    return instance().get(input);
}

/// download;
inline size_t download(std::string httpurl, std::string path) {
    return instance().download(httpurl, path);
}

inline CategoryCompletion& completion() {
    return instance().completion;
}

inline CategoryImageGeneration& imageGeneration() {
    return instance().imageGeneration;
}

inline CategoryImageEdit& imageEdit() {
    return instance().imageEdit;
}

inline CategoryImageTranslation& imageTranslation() {
    return instance().imageTranslation;
}

inline CategoryModelDeployments& modelDeployments() {
    return instance().modelDeployments;
}

inline CategoryMore& more() {
    return instance().more;
}

} // namespace __detail

using __detail::TongYiOpenAi;
using __detail::instance;
using __detail::__init;
using __detail::__initByEnv;
using __detail::post;
using __detail::get;
using __detail::download;

using __detail::completion;
using __detail::imageGeneration;
using __detail::imageTranslation;
using __detail::imageEdit;
using __detail::modelDeployments;
using __detail::more;

} // namespace TongYiOpenAi

#endif