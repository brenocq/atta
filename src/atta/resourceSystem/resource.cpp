//--------------------------------------------------
// Atta Resource System
// resource.cpp
// Date: 2021-09-07
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/resourceSystem/resource.h>

namespace atta
{
    Resource::Resource(const fs::path filename):
        _filename(filename), _id(StringId(filename.string())), _data(nullptr)
    {

    }
}
