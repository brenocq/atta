//--------------------------------------------------
// Atta IO System
// httpNull.cpp
// Date: 2022-06-07
// By Breno Cunha Queiroz
//--------------------------------------------------
// The .cpp file with the http implementation is chosen at compile time

#include <atta/ioSystem/http/http.h>

namespace atta::io
{
    const std::string Http::_implType = "null";

    Http::Http(Http::CreateInfo info)
    {

    }

    Http::~Http()
    {

    }

    Http::Content Http::get(Http::Content content)
    {

    }
    
    Http::Content Http::post(Http::Content content)
    {

    }

    Http::Content Http::put(Http::Content content)
    {

    }
}
