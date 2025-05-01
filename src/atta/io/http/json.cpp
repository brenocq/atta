//--------------------------------------------------
// Atta IO Module
// json.cpp
// Date: 2022-06-09
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/io/http/json.h>

namespace atta::io {

const std::vector<std::string> typeToString = {"none", "bool", "int", "float", "std::string", "std::vector<Json>", "std::map<std::string, Json>"};

Json::Json(std::string str) : _type(NONE) { parse(str); }

bool ignoreSpaces(const std::string& str, unsigned& pos) {
    while (str[pos] == ' ' || str[pos] == '\n' || str[pos] == '\r' || str[pos] == '\t') {
        if (pos == str.size()) {
            LOG_WARN("io::Json", "Reached end of string when ignoring spaces");
            return false;
        }
        pos++;
    }
    return true;
}

std::string Json::dump() {
    std::string result;
    switch (_type) {
        case NONE: {
            result = "null";
            break;
        }
        case BOOL: {
            result = _value.b ? "true" : "false";
            break;
        }
        case INT: {
            result = std::to_string(_value.i);
            break;
        }
        case FLOAT: {
            result = std::to_string(_value.f);
            break;
        }
        case STRING: {
            std::vector<char> str;
            str.push_back('"');
            for (char c : _string) {
                if (c == '"')
                    str.push_back('\\');
                str.push_back(c);
            }
            str.push_back('"');
            result = std::string(str.begin(), str.end());
            break;
        }
        case VECTOR: {
            result += "[";
            for (unsigned i = 0; i < _vector.size(); i++) {
                result += _vector[i].dump();
                if (i < _vector.size() - 1)
                    result += ", ";
            }
            result += "]";
            break;
        }
        case MAP: {
            result += "{";
            unsigned i = 0;
            for (auto& [key, value] : _map) {
                result += "\"" + key + "\": ";
                result += value.dump();
                if (i++ < _map.size() - 1)
                    result += ", ";
            }
            result += "}";
            break;
        }
    }
    return result;
}

//---------- Access ----------//
Json& Json::operator[](std::string key) { return map()[key]; }

Json& Json::operator[](unsigned i) { return vector()[i]; }

const Json& Json::at(std::string key) const { return map().at(key); }

const Json& Json::at(unsigned i) const { return vector().at(i); }

//---------- Assign ----------//
void Json::operator=(bool b) {
    _type = BOOL;
    _string.clear();
    _vector.clear();
    _map.clear();
    _value.b = b;
}

void Json::operator=(int i) {
    _type = INT;
    _string.clear();
    _vector.clear();
    _map.clear();
    _value.i = i;
}

void Json::operator=(float f) {
    _type = FLOAT;
    _string.clear();
    _vector.clear();
    _map.clear();
    _value.f = f;
}

void Json::operator=(std::string s) {
    _type = STRING;
    _vector.clear();
    _map.clear();
    _string = s;
}

void Json::operator=(std::vector<Json> v) {
    _type = VECTOR;
    _string.clear();
    _map.clear();
    _vector = v;
}

void Json::operator=(std::map<std::string, Json> m) {
    _type = MAP;
    _string.clear();
    _vector.clear();
    _map = m;
}

//---------- Get ----------//
Json::Type Json::getType() const { return _type; }

std::map<std::string, Json>& Json::map() {
    if (_type != MAP) {
        _type = MAP;
        _vector.clear();
        _string.clear();
    }
    return _map;
}

const std::map<std::string, Json>& Json::map() const { return _map; }

std::vector<Json>& Json::vector() {
    if (_type != VECTOR) {
        _type = VECTOR;
        _map.clear();
        _string.clear();
    }
    return _vector;
}

const std::vector<Json>& Json::vector() const { return _vector; }

Json::operator bool() const {
    if (_type == BOOL)
        return _value.b;
    else {
        LOG_WARN("io::Json", "Wrong casting to [w]bool[], type should be [w]$0[]", typeToString[int(_type)]);
        return 0;
    }
}

Json::operator int() const {
    if (_type == INT)
        return _value.i;
    else {
        LOG_WARN("io::Json", "Wrong casting to [w]int[], type should be [w]$0[]", typeToString[int(_type)]);
        return 0;
    }
}

Json::operator float() const {
    if (_type == FLOAT)
        return _value.f;
    else {
        LOG_WARN("io::Json", "Wrong casting to [w]float[], type should be [w]$0[]", typeToString[int(_type)]);
        return 0.0f;
    }
}

Json::operator std::string() const {
    if (_type == STRING)
        return _string;
    else {
        LOG_WARN("io::Json", "Wrong casting to [w]std::string[], type should be [w]$0[]", typeToString[int(_type)]);
        return "";
    }
}

} // namespace atta::io

#include <atta/io/http/jsonParse.cpp>
