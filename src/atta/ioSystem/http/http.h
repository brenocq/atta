//--------------------------------------------------
// Atta IO System
// http.h
// Date: 2022-02-10
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_IO_SYSTEM_HTTP_HTTP_H
#define ATTA_IO_SYSTEM_HTTP_HTTP_H
#include <atta/core/stringId.h>

namespace atta::io
{
    class Http
    {
    public:
        struct CreateInfo 
        {
            std::string url = "";
            StringId debugName = StringId("Unnamed io::Http");
        };

        Http(CreateInfo info);
        virtual ~Http() = default;

        struct Content
        {
            std::vector<std::pair<std::string, std::string>> headers;
            std::vector<std::pair<std::string, std::string>> params;
            std::string body;
        };

        virtual Content get(Content content) = 0;
        virtual Content post(Content content) = 0;
        virtual Content put(Content content) = 0;

    protected:
        std::string _url;
        StringId _debugName;
    };
}

#endif// ATTA_IO_SYSTEM_HTTP_HTTP_H
