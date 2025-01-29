//--------------------------------------------------
// Atta File Module
// section.cpp
// Date: 2022-04-22
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/file/serializer/section.h>

namespace atta::file {

//---------- Section ----------//

const std::string& Section::getName() const { return _name; }

bool Section::empty() const { return _map.empty(); }

size_t Section::size() const { return _map.size(); }

bool Section::contains(std::string key) const { return _map.find(key) != _map.end(); }

SectionData& Section::operator[](std::string key) {
    if (!contains(key))
        _keys.push_back(key);
    return _map[key];
}

const SectionData& Section::operator[](std::string key) const { return _map.at(key); }

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
        _keys.push_back(key);
        _map[key] = std::move(data);
    }
}

std::string Section::toString() const {
    std::ostringstream ss;
    ss << "[" << _name << "]" << std::endl;
    for (const std::string& key : _keys) {
        ss << key << " = " << _map.at(key).getStr() << std::endl;
    }
    return ss.str();
}

} // namespace atta::file
