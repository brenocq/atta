//--------------------------------------------------
// Atta Resource Module
// resource.cpp
// Date: 2021-09-07
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/resource/resource.h>

namespace atta::resource {

Resource::Resource(const fs::path filename) : _id(StringId(filename.string())), _filename(filename), _data(nullptr) {}

} // namespace atta::resource
