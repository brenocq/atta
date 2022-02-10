//--------------------------------------------------
// Atta IO System
// http.cpp
// Date: 2022-02-10
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/ioSystem/http/http.h>

namespace atta::io
{
    Http::Http(Http::CreateInfo info):
        _url(info.url), _debugName(info.debugName)
    {

    }
}
