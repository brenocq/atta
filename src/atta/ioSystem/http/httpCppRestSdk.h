//--------------------------------------------------
// Atta IO System
// httpCppRestSdk.h
// Date: 2022-02-10
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_IO_SYSTEM_HTTP_HTTP_CPPRESTSDK_H
#define ATTA_IO_SYSTEM_HTTP_HTTP_CPPRESTSDK_H
#ifdef ATTA_CPPRESTSDK_SUPPORT 
#include <atta/ioSystem/http/http.h>

namespace atta::io
{
    class HttpCppRestSdk final : public Http
    {
    public:
        HttpCppRestSdk(Http::CreateInfo info);
        ~HttpCppRestSdk();

        Http::Content get(Http::Content content) override;
        Http::Content post(Http::Content content) override;
        Http::Content put(Http::Content content) override;
    };
}

#endif// ATTA_CPPRESTSDK_SUPPORT 
#endif// ATTA_IO_SYSTEM_HTTP_HTTP_CPPRESTSDK_H
