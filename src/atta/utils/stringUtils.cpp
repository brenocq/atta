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
        // Detect word boundary for camelCase and PascalCase
        else if (!word.empty() && std::isupper(c)) {
            // Check if previous character is lowercase or number (camelCase split)
            if (std::islower(input[i - 1]) || std::isdigit(input[i - 1])) {
                words.push_back(word);
                word.clear();
            }
            // Check if next character is lowercase (e.g., "GLRenderer" should be "GL", "Renderer")
            else if (i + 1 < input.size() && std::islower(input[i + 1])) {
                words.push_back(word);
                word.clear();
            }
            word += c;
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

    std::ostringstream result;
    // First word should be lowercase
    for (char& c : words[0])
        c = std::tolower(c);
    result << words[0];

    // Capitalize first letter of remaining words
    for (size_t i = 1; i < words.size(); i++) {
        if (std::all_of(words[i].begin(), words[i].end(), ::isupper)) {
            result << words[i]; // Keep full-uppercase words unchanged
        } else {
            words[i][0] = std::toupper(words[i][0]);
            for (size_t j = 1; j < words[i].size(); j++)
                words[i][j] = std::tolower(words[i][j]);
            result << words[i];
        }
    }

    return result.str();
}

std::string toPascalCase(const std::string& input) {
    std::vector<std::string> words = splitWords(input);
    if (words.empty())
        return "";

    std::ostringstream result;
    for (size_t i = 0; i < words.size(); i++) {
        if (std::all_of(words[i].begin(), words[i].end(), ::isupper)) {
            result << words[i]; // Keep all-uppercase words as is
        } else {
            words[i][0] = std::toupper(words[i][0]);
            for (size_t j = 1; j < words[i].size(); j++)
                words[i][j] = std::tolower(words[i][j]);
            result << words[i];
        }
    }

    return result.str();
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

    std::ostringstream result;
    for (size_t i = 0; i < words.size(); i++) {
        if (std::all_of(words[i].begin(), words[i].end(), ::isupper)) {
            result << words[i]; // Keep all-uppercase words as is
        } else {
            words[i][0] = std::toupper(words[i][0]);
            for (size_t j = 1; j < words[i].size(); j++)
                words[i][j] = std::tolower(words[i][j]);
            result << words[i];
        }
        if (i < words.size() - 1)
            result << " ";
    }

    return result.str();
}

} // namespace atta
