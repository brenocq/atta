//--------------------------------------------------
// Atta IO System
// httpCppRestSdk.cpp
// Date: 2022-02-10
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/ioSystem/http/httpCppRestSdk.h>
#ifdef ATTA_CPPRESTSDK_SUPPORT 

#include <cpprest/http_client.h>
#include <cpprest/json.h>
#include <cpprest/filestream.h>
using namespace utility;                    // Common utilities like string conversions
using namespace web;                        // Common features like URIs.
using namespace web::http;                  // Common HTTP functionality
using namespace web::http::client;          // HTTP client features
using namespace concurrency::streams;       // Asynchronous streams
using namespace web::json;

namespace atta::io
{
    HttpCppRestSdk::HttpCppRestSdk(Http::CreateInfo info):
        Http(info)
    {

    }

    HttpCppRestSdk::~HttpCppRestSdk()
    {

    }

    Http::Content HttpCppRestSdk::get(Http::Content content)
    {
        //static std::shared_ptr<http_client> client = std::make_shared<http_client>("https://raw.githubusercontent.com/brenocq-atta/projects/main/projects.json");
        return {};
    }

    Http::Content HttpCppRestSdk::post(Http::Content content)
    {
        return {};
    }

    Http::Content HttpCppRestSdk::put(Http::Content content)
    {
        return {};
    }
}
#endif// ATTA_CPPRESTSDK_SUPPORT 
