//--------------------------------------------------
// Atta File Module
// section.cpp
// Date: 2022-04-22
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/file/serializer/section.h>

namespace atta::file {

//---------- Section ----------//
std::string Section::toString() const {
    std::ostringstream ss;
    ss << "[" << _name << "]" << std::endl;
    for (const auto& [key, value] : _map) {
        ss << key << " = " << value.getStr() << std::endl;
    }
    return ss.str();
}

std::map<std::string, SectionData>& Section::map() { return _map; }

const std::map<std::string, SectionData>& Section::map() const { return _map; }

SectionData& Section::operator[](std::string key) { return map()[key]; }

bool Section::contains(std::string key) { return map().find(key) != map().end(); }

} // namespace atta::file
