//--------------------------------------------------
// Atta Utils Module
// stringUtils.h
// Date: 2025-02-16
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_UTILS_STRING_UTILS_H
#define ATTA_UTILS_STRING_UTILS_H

namespace atta {

// Helper function to split words from a mixed-case or spaced string
std::vector<std::string> splitWords(const std::string& input);

// Convert string to camelCase
std::string toCamelCase(const std::string& input);

// Convert string to PascalCase
std::string toPascalCase(const std::string& input);

// Convert string to snake_case
std::string toSnakeCase(const std::string& input);

// Convert string to Title Case
std::string toTitleCase(const std::string& input);

} // namespace atta

#endif // ATTA_UTILS_STRING_UTILS_H
