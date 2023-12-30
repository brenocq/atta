//--------------------------------------------------
// Atta IO Module
// jsonParse.cpp
// Date: 2022-06-09
// By Breno Cunha Queiroz
//--------------------------------------------------
namespace atta::io {

bool Json::parse(const std::string& str) {
    bool res;
    unsigned pos = 0;
    parseAux(str, pos, res);
    return res;
}

std::string getWord(const std::string& str, unsigned& pos) {
    // Read word starting at position pos
    unsigned start = pos;
    while (pos < str.size() && str[pos] != ' ' && str[pos] != '\n' && str[pos] != '\r' && str[pos] != '\t' && str[pos] != ',' && str[pos] != ':' &&
           str[pos] != '}' && str[pos] != ']')
        pos++;
    return str.substr(start, pos - start);
}

void Json::parseAux(const std::string& str, unsigned& pos, bool& res) {
    do {
        if (str[pos] == ' ' || str[pos] == '\n' || str[pos] == '\r' || str[pos] == '\t')
            pos++;
        else if (str[pos] == '[') {
            vector() = parseVector(str, pos, res);
            return;
        } else if (str[pos] == '{') {
            map() = parseMap(str, pos, res);
            return;
        } else if (str[pos] == '"') {
            *this = parseString(str, pos, res);
            return;
        } else if (str[pos] == 'n') {
            _type = NONE;
            parseNone(str, pos, res);
            return;
        } else if (str[pos] == 't' || str[pos] == 'f') {
            *this = parseBool(str, pos, res);
            return;
        } else if ((str[pos] >= '0' && str[pos] <= '9') || str[pos] == '-') {
            // INT or FLOAT
            unsigned curr = pos;
            // Find first char that is not a digit or '-'
            while (curr + 1 < str.size() && ((str[curr] >= '0' && str[curr] <= '9') || str[curr] == '-'))
                curr++;

            // Check if it is .
            if (str[curr] == '.')
                *this = parseFloat(str, pos, res);
            else
                *this = parseInt(str, pos, res);
            return;
        } else {
            LOG_WARN("io::Json", "Found unexpected character '[w]$0[]'", str[pos]);
            res = false;
            return;
        }
    } while (pos < str.size());

    LOG_WARN("io::Json", "Reached end of string while parsing");

    res = false;
    return;
}

void Json::parseNone(const std::string& str, unsigned& pos, bool& res) {
    std::string nullWord = getWord(str, pos);
    if (nullWord == "null")
        res = true;
    else {
        LOG_WARN("io::Json", "Could not parse None ([w]$0[] != [w]null[])", nullWord);
        res = false;
    }
}

bool Json::parseBool(const std::string& str, unsigned& pos, bool& res) {
    std::string boolWord = getWord(str, pos);
    if (boolWord == "true") {
        res = true;
        return true;
    } else if (boolWord == "false") {
        res = true;
        return false;
    } else {
        LOG_WARN("io::Json", "Could not parse bool, expecting [w]true[] or [w]false[], but found [w]$0[]", boolWord);
        res = false;
    }
    return {};
}

int Json::parseInt(const std::string& str, unsigned& pos, bool& res) {
    std::string intWord = getWord(str, pos);
    res = true;
    return std::stoi(intWord);
}

float Json::parseFloat(const std::string& str, unsigned& pos, bool& res) {
    std::string floatWord = getWord(str, pos);
    res = true;
    return std::stof(floatWord);
}

std::string Json::parseString(const std::string& str, unsigned& pos, bool& res) {
    // For now ignoring all '\\'
    pos++; // Ignore "
    std::vector<char> result;
    while (pos < str.size() && !(str[pos] == '"' && str[pos - 1] != '\\')) {
        if (str[pos] != '\\')
            result.push_back(str[pos]);
        pos++;
    }
    pos++; // Ignore "

    res = true;
    return std::string(result.begin(), result.end());
}

std::vector<Json> Json::parseVector(const std::string& str, unsigned& pos, bool& res) {
    std::vector<Json> result;
    res = false;

    if (str[pos] == '[')
        pos++;
    else {
        LOG_WARN("io::Json", "Expecting vector [w][[], but found [w]$0[]", str[pos]);
        return {};
    }

    while (str[pos] != ']') {
        if (pos >= str.size()) {
            LOG_WARN("io::Json", "Reached unexpected end of vector");
            return {};
        }

        while (str[pos] == ' ' || str[pos] == '\n' || str[pos] == '\r' || str[pos] == '\t') {
            if (pos == str.size()) {
                LOG_WARN("io::Json", "Expecting vector end [w]][] or element, but reached end of string");
                return {};
            }
            pos++;
        }

        if (str[pos] == ',') {
            // Vector next element
            pos++;
            continue;
        } else if (str[pos] == ']') {
            // Vector end
            continue;
        } else {
            // Vector element
            Json element;
            bool vres;
            element.parseAux(str, pos, vres);
            if (vres == false) {
                LOG_WARN("io::Json", "Failed to read vector element");
                return {};
            }
            result.push_back(element);
        }
    }
    pos++; // Ignore ]

    res = true;
    return result;
}

std::map<std::string, Json> Json::parseMap(const std::string& str, unsigned& pos, bool& res) {
    std::map<std::string, Json> result;
    res = false;

    if (str[pos] == '{')
        pos++;
    else {
        LOG_WARN("io::Json", "Expecting map [w]{[], but found [w]$0[]", str[pos]);
        return {};
    }

    while (str[pos] != '}') {
        if (pos >= str.size()) {
            LOG_WARN("io::Json", "Reached unexpected end of map");
            return {};
        }

        while (str[pos] == ' ' || str[pos] == '\n' || str[pos] == '\r' || str[pos] == '\t') {
            if (pos == str.size()) {
                LOG_WARN("io::Json", "Expecting map key [w]\"[] or map end [w]}[], but reached end of string");
                return {};
            }
            pos++;
        }

        if (str[pos] == '"') {
            // Key
            std::string key;
            size_t keyEnd = str.find('"', pos + 1);
            if (keyEnd != std::string::npos) {
                key = str.substr(pos + 1, keyEnd - pos - 1);
                size_t separatorPos = str.find(':', keyEnd);
                if (separatorPos == std::string::npos) {
                    LOG_WARN("io::Json", "Could not find map key separator [w]:[]");
                    return {};
                }
                pos = separatorPos + 1;

                Json value;
                bool vres;
                value.parseAux(str, pos, vres);
                if (vres == false) {
                    LOG_WARN("io::Json", "Failed to read map value for key [w]$0[]", key);
                    return {};
                }
                result[key] = value;
            } else {
                LOG_WARN("io::Json", "Could not find map key end [w]\"[]");
                return {};
            }
        } else if (str[pos] == ',') {
            pos++;
            continue;
        } else if (str[pos] == '}') {
            // Map end
            continue;
        } else {
            LOG_WARN("io::Json", "Expecting map key [w]\"[] or map end [w]}[], but found [w]$0[]", str[pos]);
            return {};
        }
    }
    pos++; // Ignore }

    res = true;
    return result;
}

} // namespace atta::io
