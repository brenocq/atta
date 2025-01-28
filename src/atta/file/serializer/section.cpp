//--------------------------------------------------
// Atta File Module
// section.cpp
// Date: 2022-04-22
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/file/serializer/section.h>

namespace atta::file {

//---------- Section ----------//

std::map<std::string, SectionData>& Section::map() { return _map; }

const std::map<std::string, SectionData>& Section::map() const { return _map; }

SectionData& Section::operator[](std::string key) { return map()[key]; }

bool Section::contains(std::string key) { return map().find(key) != map().end(); }

void Section::insertFromString(const std::string& string) {
    // Check if the input string is empty or contains only whitespace
    if (string.find_first_not_of(" \t") == std::string::npos)
        return;

    auto delimiterPos = string.find('=');
    if (delimiterPos != std::string::npos) {
        std::string key = string.substr(0, delimiterPos);
        std::string value = string.substr(delimiterPos + 1);

        // Trim whitespace around key and value
        key.erase(0, key.find_first_not_of(" \t"));
        key.erase(key.find_last_not_of(" \t") + 1);
        value.erase(0, value.find_first_not_of(" \t"));
        value.erase(value.find_last_not_of(" \t") + 1);

        if (key.empty() || value.empty())
            return;

        // Add to the map
        SectionData data;
        data.setStr(value);
        _map[key] = std::move(data);
    }
}

std::string Section::toString() const {
    std::ostringstream ss;
    ss << "[" << _name << "]" << std::endl;
    for (const auto& [key, value] : _map) {
        ss << key << " = " << value.getStr() << std::endl;
    }
    return ss.str();
}

} // namespace atta::file
