//--------------------------------------------------
// Atta File Module
// serializer.cpp
// Date: 2025-01-28
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/file/serializer/serializer.h>

namespace atta::file {

void Serializer::serialize(const fs::path& file) {
    std::ofstream ofs(file);
    if (!ofs.is_open()) {
        LOG_ERROR("Serializer", "Failed to open file for writing: [w]$0", file);
        return;
    }
    ofs << toString();
}

void Serializer::deserialize(const fs::path& file) {
    std::ifstream ifs(file);
    if (!ifs.is_open()) {
        LOG_ERROR("Serializer", "Failed to open file for reading: [w]$0", file);
        return;
    }

    std::string data((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());
    fromString(data);
}

std::string Serializer::toString() const {
    std::ostringstream ss;
    for (const auto& section : _sections) {
        ss << section.toString();
        ss << std::endl;
    }
    return ss.str();
}

void Serializer::fromString(const std::string& data) {
    std::istringstream iss(data);
    std::string line;
    Section* currentSection = nullptr;

    while (std::getline(iss, line)) {
        // Trim whitespace from the line
        line.erase(0, line.find_first_not_of(" \t"));
        line.erase(line.find_last_not_of(" \t") + 1);

        // Skip empty lines and comments
        if (line.empty() || line[0] == '#')
            continue;

        if (line.front() == '[' && line.back() == ']') {
            // New section detected
            std::string sectionName = line.substr(1, line.size() - 2);
            _sections.emplace_back(sectionName);
            currentSection = &_sections.back();
        } else if (currentSection) {
            // Insert key-value pairs to section from current line
            currentSection->insertFromString(line);
        }
    }
}

} // namespace atta::file
