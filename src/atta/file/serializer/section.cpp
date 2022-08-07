//--------------------------------------------------
// Atta File Module
// section.cpp
// Date: 2022-04-22
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/file/serializer/section.h>

namespace atta::file {
//---------- SectionData ----------//
std::unordered_map<SectionData::TypeHash, SectionData::PrintFunction> SectionData::_typeToString = {};

SectionData::SectionData() : _typeHash("undefined"_ssid) {}

void SectionData::clear() {
    _typeHash = "undefined"_sid;
    _data.clear();
}

SectionData::TypeHash SectionData::getTypeHash() const { return _typeHash; }

std::string SectionData::toString() const {
    if (_typeHash == "undefined"_sid) {
        // LOG_WARN("SectionData", "Trying to call [w]toString()[] in data with undefined type");
        return "undefined";
    } else if (_typeToString.find(_typeHash) == _typeToString.end()) {
        // LOG_WARN("SectionData", "Trying to call [w]toString()[] in data with unknown type");
        return "unknown";
    } else {
        return _typeToString.at(_typeHash)(_data);
    }

    return "";
}

void SectionData::serialize(std::ostream& os) {
    write<uint32_t>(os, _data.size());
    write(os, _data.data(), _data.size());
}

void SectionData::deserialize(std::istream& is) {
    uint32_t size;
    read<uint32_t>(is, size);
    _data.resize(size);
    if (size > 0)
        read(is, _data.data(), _data.size());
}

//---------- Section ----------//
Section::Section() : _type(UNDEFINED) {}

Section::Section(const Section& section) {
    _type = section._type;
    _map = section._map;
    _vector = section._vector;
    _data = section._data;
}

bool Section::isUndefined() const { return _type == UNDEFINED; }

bool Section::isMap() const { return _type == MAP; }

bool Section::isVector() const { return _type == VECTOR; }

bool Section::isData() const { return _type == DATA; }

size_t Section::size() const {
    if (isMap())
        return map().size();
    else if (isVector())
        return vector().size();
    else
        return 0;
}

std::string Section::toString() const {
    if (isUndefined())
        return "undefined";
    else if (isData()) {
        return _data.toString();
    } else if (isMap()) {
        std::stringstream ss;
        ss << "{";
        unsigned i = 0;
        for (const auto& [key, val] : map()) {
            ss << "\"" << key << "\": " << val.toString();
            if (++i != size())
                ss << ", ";
        }
        ss << "}";
        return ss.str();
    } else if (isVector()) {
        std::stringstream ss;
        ss << "[";
        unsigned i = 0;
        for (const auto& el : vector()) {
            ss << el.toString();
            if (++i != size())
                ss << ", ";
        }
        ss << "]";
        return ss.str();
    }
    return "";
}

//---------- Map ----------//
SectionData& Section::data() {
    if (!isData()) {
        _map.clear();
        _vector.clear();
        _type = DATA;
    }
    return _data;
}

const SectionData& Section::data() const { return _data; }

//---------- Map ----------//
std::map<std::string, Section>& Section::map() {
    if (!isMap()) {
        _vector.clear();
        _data.clear();
        _type = MAP;
    }

    return _map;
}

const std::map<std::string, Section>& Section::map() const { return _map; }

Section& Section::operator[](std::string key) { return map()[key]; }

bool Section::contains(std::string key) { return map().find(key) != map().end(); }

//---------- Vector ----------//
std::vector<Section>& Section::vector() {
    if (!isVector()) {
        _map.clear();
        _data.clear();
        _type = VECTOR;
    }

    return _vector;
}

const std::vector<Section>& Section::vector() const { return _vector; }

Section& Section::operator[](unsigned i) { return vector()[i]; }

void Section::operator+=(Section section) { vector().push_back(section); }

void Section::push_back(Section section) { vector().push_back(section); }

Section& Section::back() { return vector().back(); }

//----- Serialization -----//
void Section::serialize(std::ostream& os) {
    if (isUndefined()) {
        write<uint32_t>(os, 0);
    } else if (isData()) {
        _data.serialize(os);
    } else if (isMap()) {
        write<uint32_t>(os, std::numeric_limits<uint32_t>::max());
        write<char>(os, '{');
        for (auto& [key, val] : map()) {
            write<std::string>(os, key);
            val.serialize(os);
        }
        write<char>(os, '}');
    } else if (isVector()) {
        write<uint32_t>(os, std::numeric_limits<uint32_t>::max());
        write<char>(os, '[');
        unsigned i = 0;
        for (auto& el : vector()) {
            el.serialize(os);
            if (++i != size())
                write<char>(os, ',');
        }
        write<char>(os, ']');
    }
}

void Section::deserialize(std::istream& is) {
    std::vector<Section*> levels;
    _type = UNDEFINED;
    levels.push_back(this);

    do {
        if (levels.back()->_type == UNDEFINED) {
            // Discover type
            uint32_t size;
            std::streampos oldPos = is.tellg();
            read<uint32_t>(is, size);
            if (size == std::numeric_limits<uint32_t>::max()) {
                // Section is map or vector
                oldPos = is.tellg();
                char open;
                read(is, open);
                if (open == '{') {
                    // Section is map
                    levels.back()->_type = MAP;
                } else if (open == '[') {
                    is.seekg(oldPos);

                    // Section is vector
                    levels.back()->_type = VECTOR;
                } else
                    LOG_ERROR("FileSystem::Section", "Could not deserialize, was expecting [w]'{'[] or [w]'['[] but found [w]'$0'[]", char(open));
            } else {
                is.seekg(oldPos);

                // Section is data
                levels.back()->data().deserialize(is);
                levels.pop_back();
            }
        } else if (levels.back()->_type == MAP) {
            std::streampos oldPos = is.tellg();
            char close;
            read(is, close);
            if (close == '}') {
                // Finished reading map
                levels.pop_back();
            } else {
                is.seekg(oldPos);

                // Read map pair key
                std::string key;
                read<std::string>(is, key);

                // Read map pair value
                Section value;
                (*levels.back())[key] = value;
                levels.push_back(&((*levels.back())[key]));
            }
        } else if (levels.back()->_type == VECTOR) {
            char close;
            read(is, close);
            if (close == ']') {
                // Finished reading map
                levels.pop_back();
            } else if (close == '[' || close == ',') {
                // Read vector data
                levels.back()->push_back(Section{}); // Add to vector
                levels.push_back(&(levels.back()->back()));
            } else
                LOG_ERROR("FileSystem::Section", "Could not deserialize, wrong data when reading vector, should be [w]][] or [w],[]", close);
        } else
            LOG_ERROR("FileSystem::Section", "Could not deserialize, Section is of type that is not expected");
    } while (!levels.empty());
}
} // namespace atta::file
