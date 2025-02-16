//--------------------------------------------------
// Atta Utils Module
// stringUtils.cpp
// Date: 2025-02-16
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/utils/stringUtils.h>

namespace atta {

std::vector<std::string> splitWords(const std::string& input) {
    std::vector<std::string> words;
    std::string word;

    for (size_t i = 0; i < input.size(); i++) {
        char c = input[i];

        // If space, underscore, or hyphen, treat as a separator
        if (c == ' ' || c == '_' || c == '-') {
            if (!word.empty()) {
                words.push_back(word);
                word.clear();
            }
        }
        // Detect word boundary in camelCase or PascalCase
        else if (!word.empty() && std::isupper(c) && (std::islower(input[i - 1]) || std::isdigit(input[i - 1]))) {
            words.push_back(word);
            word = c;
        }
        // Append character to the word
        else {
            word += c;
        }
    }

    if (!word.empty()) {
        words.push_back(word);
    }

    return words;
}

std::string toCamelCase(const std::string& input) {
    std::vector<std::string> words = splitWords(input);
    if (words.empty())
        return "";

    std::string result = words[0]; // Keep first word as lowercase
    for (size_t i = 1; i < words.size(); i++) {
        words[i][0] = std::toupper(words[i][0]); // Capitalize first letter
        result += words[i];
    }

    return result;
}

std::string toPascalCase(const std::string& input) {
    std::string result = toCamelCase(input);
    if (result.empty())
        return "";
    result[0] = std::toupper(result[0]); // Capitalize first letter
    return result;
}

std::string toSnakeCase(const std::string& input) {
    std::vector<std::string> words = splitWords(input);
    if (words.empty())
        return "";

    std::ostringstream oss;
    for (size_t i = 0; i < words.size(); i++) {
        for (char& c : words[i])
            c = std::tolower(c); // Convert to lowercase
        oss << words[i];
        if (i < words.size() - 1)
            oss << "_";
    }

    return oss.str();
}

std::string toTitleCase(const std::string& input) {
    std::vector<std::string> words = splitWords(input);
    if (words.empty())
        return "";

    std::ostringstream oss;
    for (size_t i = 0; i < words.size(); i++) {
        words[i][0] = std::toupper(words[i][0]); // Capitalize first letter
        for (size_t j = 1; j < words[i].size(); j++)
            words[i][j] = std::tolower(words[i][j]); // Lowercase rest
        oss << words[i];
        if (i < words.size() - 1)
            oss << " ";
    }

    return oss.str();
}

} // namespace atta
