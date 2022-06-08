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
        ~Http();

        struct Content
        {
            std::vector<std::pair<std::string, std::string>> headers;
            std::vector<std::pair<std::string, std::string>> params;
            std::string body;
        };

        Content get(Content content);
        Content post(Content content);
        Content put(Content content);
        
        static std::string getImplType() { return _implType; }

    private:
        std::string _url;
        StringId _debugName;
        static const std::string _implType;// Which implementation is being used
    };
}

#endif// ATTA_IO_SYSTEM_HTTP_HTTP_H
