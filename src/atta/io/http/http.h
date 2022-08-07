//--------------------------------------------------
// Atta IO Module
// http.h
// Date: 2022-02-10
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_IO_HTTP_HTTP_H
#define ATTA_IO_HTTP_HTTP_H
#include <atta/core/stringId.h>

namespace atta::io
{
    class Http
    {
    public:
        Http(std::string url): _url(url) {}
        ~Http() = default;

        struct Request
        {
            std::map<std::string, std::string> headers;
            std::map<std::string, std::string> params;
            std::string body;
        };

        struct Response
        {
            unsigned statusCode;
            std::map<std::string, std::string> headers;
            std::string body;
        };

        enum RequestType
        {
            POST,
            GET,
            PUT
        };

        Response get(const Request& req = {}) { return request(req, GET); }
        Response post(const Request& req = {}) { return request(req, POST); }
        Response put(const Request& req = {}) { return request(req, PUT); }
        Response request(const Request& req, RequestType reqType);
        
        static const std::string implType;// Which implementation is being used
        static const bool supported;// If the http is implemented for this build

    private:
        std::string _url;
    };
}

#endif// ATTA_IO_HTTP_HTTP_H
