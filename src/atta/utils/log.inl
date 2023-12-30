//--------------------------------------------------
// Atta Utils Module
// log.inl
// Date: 2021-08-11
// By Breno Cunha Queiroz
//--------------------------------------------------

namespace atta {

template <class... Args>
void Log::verbose(std::string tag, std::string text, Args&&... args) {
    if (logLevel <= LOG_LEVEL_VERBOSE)
        log(COLOR_BOLD_WHITE, tag, COLOR_RESET_WHITE, text, args...);
}

template <class... Args>
void Log::debug(std::string tag, std::string text, Args&&... args) {
    if (logLevel <= LOG_LEVEL_DEBUG)
        log(COLOR_BOLD_BLUE, tag, COLOR_RESET_BLUE, text, args...);
}

template <class... Args>
void Log::success(std::string tag, std::string text, Args&&... args) {
    if (logLevel <= LOG_LEVEL_SUCCESS)
        log(COLOR_BOLD_GREEN, tag, COLOR_RESET_GREEN, text, args...);
}

template <class... Args>
void Log::info(std::string tag, std::string text, Args&&... args) {
    if (logLevel <= LOG_LEVEL_INFO)
        log(COLOR_BOLD_CYAN, tag, COLOR_RESET_CYAN, text, args...);
}

template <class... Args>
void Log::warning(std::string tag, std::string text, Args&&... args) {
    if (logLevel <= LOG_LEVEL_WARNING)
        log(COLOR_BOLD_YELLOW, tag, COLOR_RESET_YELLOW, text, args...);
}

template <class... Args>
void Log::error(std::string tag, std::string text, Args&&... args) {
    if (logLevel <= LOG_LEVEL_ERROR)
        log(COLOR_BOLD_RED, tag, COLOR_RESET_RED, text, args...);
}

// std::array overload
template <typename Tstream, typename T, size_t N>
std::ostream& operator<<(Tstream& s, const std::array<T, N>& arr) {
    s << "{";
    for (typename std::array<T, N>::const_iterator it = arr.begin(); it != arr.end(); it++) {
        if (it != arr.begin())
            s << ", ";
        s << *it;
    }
    s << "}";
    return s;
}

// std::vector overload
template <typename Tstream, typename T>
std::ostream& operator<<(Tstream& s, const std::vector<T>& vec) {
    s << "{";
    for (typename std::vector<T>::const_iterator it = vec.begin(); it != vec.end(); it++) {
        if (it != vec.begin())
            s << ", ";
        s << *it;
    }
    s << "}";
    return s;
}

// std::set overload
template <typename Tstream, typename T>
std::ostream& operator<<(Tstream& s, const std::set<T>& set) {
    s << "{";
    for (typename std::set<T>::const_iterator it = set.begin(); it != set.end(); it++) {
        if (it != set.begin())
            s << ", ";
        s << *it;
    }
    s << "}";
    return s;
}

// std::map overload
template <typename Tstream, typename K, typename V>
std::ostream& operator<<(Tstream& s, const std::map<K, V>& map) {
    s << "{";
    for (typename std::map<K, V>::const_iterator it = map.begin(); it != map.end(); it++) {
        if (it != map.begin())
            s << ", ";
        s << "{" << it->first << ", " << it->second << "}";
    }
    s << "}";
    return s;
}

template <typename T>
std::string getArgStr(const T& t) {
    std::stringstream ss;
    ss << t;
    return ss.str();
}

template <class... Args>
void Log::log(const char* tagColor, std::string tag, const char* textColor, std::string text, Args&&... args) {
    // Replace arguments
    std::vector<std::string> argsStr;
    (argsStr.push_back(getArgStr(args)), ...);

    std::stringstream finalText;
    for (size_t i = 0; i < text.size(); i++) {
        if (text[i] == '[') {
            i++; // Skip [
            if (text[i] == '*') {
                finalText << COLOR_BOLD;
                i++; // Skip *
            } else if (text[i] == ']') {
                finalText << textColor; // Return color to default if found []
                continue;
            }

            switch (text[i]) {
                case 'w':
                    finalText << COLOR_RESET << COLOR_WHITE;
                    break;
                case 'r':
                    finalText << COLOR_RESET << COLOR_RED;
                    break;
                case 'g':
                    finalText << COLOR_RESET << COLOR_GREEN;
                    break;
                case 'b':
                    finalText << COLOR_RESET << COLOR_BLUE;
                    break;
                case 'y':
                    finalText << COLOR_RESET << COLOR_YELLOW;
                    break;
                case 'm':
                    finalText << COLOR_RESET << COLOR_MAGENTA;
                    break;
                case 'c':
                    finalText << COLOR_RESET << COLOR_CYAN;
                    break;
                case 'k':
                    finalText << COLOR_RESET << COLOR_BLACK;
                    break;
                default:
                    finalText << '[';
                    i -= 2;
            }
            i++; // Skip color and ]
        } else if (text[i] == '$') {
            if (text[i + 1] == 'x') {
                // Hex print
                i++;
                finalText << std::hex << "0x";
                size_t idx = static_cast<size_t>(text[++i] - '0');
                if (idx < sizeof...(Args))
                    finalText << std::stoi(argsStr[idx]);
                finalText << std::dec;
            } else if (text[i + 1] == 'b') {
                // Binary print
                i++;
                finalText << "0b";
                size_t idx = static_cast<size_t>(text[++i] - '0');
                if (idx < sizeof...(Args))
                    finalText << std::bitset<CHAR_BIT>(std::stoi(argsStr[idx]));
            } else {
                // Stream print
                size_t idx = static_cast<size_t>(text[++i] - '0');
                if (idx < sizeof...(Args))
                    finalText << argsStr[idx];
            }
        } else
            finalText << text[i];
    }

    // Print [tag] text
    std::string output = std::string(tagColor) + "[" + tag + "] " + textColor + finalText.str() + COLOR_RESET + "\n";
    std::cout << output;
}
} // namespace atta
