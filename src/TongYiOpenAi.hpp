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

//// Session;
// class Session {
// protected:
// };
class TongYiOpenAi {
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
        //   void sayHello()
        //   {
        //     HttpRequest request;
        //     request.method = HTTP_POST;
        //     request.url = "https://dashscope.aliyuncs.com/compatible-mode/v1/chat/completions";
        //     request.headers["Authorization"] = string("Bearer ") + string("sk-c43463e3702d452fa7d873efb7808eae");
        //     request.headers["Content-Type"] = "application/json";
        //     hv::Json data;
        //     data["model"] = "qwen-plus";
        //     data["messages"] = "[{\"role\": \"system\", \"content\": \"You are a helpful assistant.\"}, {\"role\": \"user\", \"content\": \"你是谁？\"}]}]";
        //     hv::HttpClient client;
        //     hv::HttpResponse response;
        //     client.send(&request, &response);
        //   }

        /// post;
        Json post(Json input)
        {
			return postByHttpUrl(m_httpUrl, input);
        }

        Json postByHttpUrl(std::string httpurl, Json input)
        {
            http_headers headers;
            headers["Authorization"] = std::string("Bearer ") + m_token;
            headers["Content-Type"] = "application/json";
            requests::Response  resp = requests::post(httpurl.c_str(), input.dump(), headers);

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
            if (isJson(resptext)) {
                json = Json::parse(resptext);
            }
            else {
                hloge("TongYiOpenapi::post() - json parse error");
            }
            return json;
        }

        /// get;
        Json get(std::string httpurl)
        {
            http_headers headers;
            headers["Authorization"] = std::string("Bearer ") + m_token;
            headers["Content-Type"] = "application/json";
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
        CategoryModelDeployments modelDeployments{*this};
    protected:
        std::string m_token;
        std::string m_httpUrl;

}; /// class TongYiOpenAi;



Json CategoryCompletion::create(Json input)
{
    return m_openAi.post(input);
}

//curl - X POST "https://dashscope.aliyuncs.com/api/v1/tokens?expire_in_seconds=1800" \
//- H "Authorization: Bearer $DASHSCOPE_API_KEY"
Json CategoryMore::makeTempToken(int expire_in_seconds)
{
    std::string httpurl = "https://dashscope.aliyuncs.com/api/v1/services/aigc/auth/temp-token?expire_in_seconds=" + std::to_string(expire_in_seconds);
    //return m_openai.get(httpurl);
	return m_openai.postByHttpUrl(httpurl, Json{});
}

/// not class method;
TongYiOpenAi& instance() {
   static TongYiOpenAi _instance;
   return _instance;
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

inline CategoryCompletion& completion() {
    return instance().completion;
}

inline CategoryModelDeployments& modelDeployments() {
    return instance().modelDeployments;
}

} // namespace __detail

using __detail::TongYiOpenAi;
using __detail::instance;
using __detail::__init;
using __detail::__initByEnv;
using __detail::post;

using __detail::completion;
using __detail::modelDeployments;

} // namespace TongYiOpenAi

#endif