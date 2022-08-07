//--------------------------------------------------
// Atta IO Module
// httpWeb.cpp
// Date: 2022-06-07
// By Breno Cunha Queiroz
//--------------------------------------------------
// The .cpp file with the http implementation is chosen at compile time

#include <atta/io/http/http.h>

namespace atta::io
{
    const std::string Http::implType = "web";
    const bool Http::supported = false;

    Http::Response Http::request(const Http::Request& req, Http::RequestType reqType)
    {
        return {};
    }
}
