//--------------------------------------------------
// Atta File System
// section.cpp
// Date: 2022-04-22
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/fileSystem/serializer/section.h>
#include <atta/fileSystem/serializer/serializer.h>

namespace atta
{
    //---------- SectionData ----------//
    std::unordered_map<size_t, SectionData::PrintFunction> SectionData::_typeToString = {};

    SectionData::SectionData():
        _typeHash(0)
    {

    }

    void SectionData::clear()
    {
        _typeHash = 0;
        _data.clear();
    }

    size_t SectionData::getTypeHash() const
    {
        return _typeHash;
    }

    std::string SectionData::toString() const
    {
        if(_typeHash == 0)
        {
            LOG_WARN("SectionData", "Trying to convert uninitialized type to string");
            return "";
        }
        else if(_typeToString.find(_typeHash) == _typeToString.end())
        {
            LOG_WARN("SectionData", "Trying to convert unknown type to string");
            return "";
        }
        else
        {
            return _typeToString.at(_typeHash)(_data);
        }

        return "";
    }

    void SectionData::serialize(std::ostream& os)
    {
        write<size_t>(os, _data.size());
        write(os, _data.data(), _data.size());
    }

    void SectionData::deserialize(std::istream& is)
    {
        size_t size;
        read<size_t>(is, size);
        _data.resize(size);
        read(is, _data.data(), _data.size());
    }

    //---------- Section ----------//
    Section::Section():
        _type(UNDEFINED)
    {

    }

    Section::Section(const Section& section)
    {
        _type = section._type;
        _map = section._map;
        _vector = section._vector;
        _data = section._data;
    }

    bool Section::isUndefined() const
    {
        return _type == UNDEFINED;
    }
    
    bool Section::isMap() const
    {
        return _type == MAP;
    }

    bool Section::isVector() const
    {
        return _type == VECTOR;
    }

    bool Section::isData() const
    { 
        return _type == DATA;
    }

    size_t Section::size() const
    {
        if(isMap()) return map().size();
        else if(isVector()) return vector().size();
        else return 0;
    }

    std::string Section::toString() const
    {
        if(isUndefined()) 
            return "undefined";
        else if(isData())
        {
            return _data.toString();
        }
        else if(isMap())
        {
            std::stringstream ss;
            ss << "{";
            unsigned i = 0;
            for(const auto& [key, val] : map())
            {
                ss << "\"" << key << "\": " << val.toString();
                if(++i != size())
                    ss << ", ";
            }
            ss << "}";
            return ss.str();
        }
        else if(isVector())
        {
            std::stringstream ss;
            ss << "[";
            unsigned i = 0;
            for(const auto& el : vector())
            {
                ss << el.toString();
                if(++i != size())
                    ss << ", ";
            }
            ss << "]";
            return ss.str();
        }
        return "";
    }

    //---------- Map ----------//
    SectionData& Section::data()
    {
        if(!isData())
        {
            _map.clear();
            _vector.clear();
            _type = DATA;
        }
        return _data;
    }

    const SectionData& Section::data() const
    {
        return _data;
    }

    //---------- Map ----------//
    std::map<std::string, Section>& Section::map()
    {
        if(!isMap())
        {
            _vector.clear();
            _data.clear();
            _type = MAP;
        }

        return _map;
    }

    const std::map<std::string, Section>& Section::map() const
    {
        return _map;
    }

    Section& Section::operator[](std::string key)
    {
        return map()[key];
    }

    //---------- Vector ----------//
    std::vector<Section>& Section::vector()
    {
        if(!isVector())
        {
            _map.clear();
            _data.clear();
            _type = VECTOR;
        }

        return _vector;
    }

    const std::vector<Section>& Section::vector() const
    {
        return _vector;
    }

    Section& Section::operator[](unsigned i)
    {
        return vector()[i];
    }

    void Section::operator+=(Section section)
    {
        vector().push_back(section);
    }

    void Section::push_back(Section section)
    {
        vector().push_back(section);
    }

    Section& Section::back()
    {
        return vector().back();
    }

    //----- Serialization -----//
    void Section::serialize(std::ostream& os)
    {
        if(isUndefined()) 
        {
            os << "undefined";
        }
        else if(isData())
        {
            _data.serialize(os);
        }
        else if(isMap())
        {
            write<uint8_t>(os, 0);
            write<char>(os, '{');
            for(auto& [key, val] : map())
            {
                write<std::string>(os, key);
                val.serialize(os);
            }
            write<char>(os, '}');
        }
        else if(isVector())
        {
            write<uint8_t>(os, 0);
            write<char>(os, '[');
            unsigned i = 0;
            for(auto& el : vector())
            {
                os << el.toString();
                el.serialize(os);
                if(++i != size())
                    write<char>(os, ',');
            }
            write<char>(os, ']');
        }
    }

    void Section::deserialize(std::istream& is)
    {
        Section section;
        std::vector<Section*> levels;
        levels.push_back(&section);

        do
        {
            size_t size;
            read<size_t>(is, size);
            if(size == 0)
            {
                // Section is map or vector
                char open;
                read<char>(is, open);
                if(open == '{')
                {
                    
                }
                else if(open == '[')
                {

                }
                else
                {
                    LOG_ERROR("FileSystem::Section", "Could not deserialize, was expecting [w]'{'[] or [w]'['[] but found [w]'$0'[]", open);
                }
            }
            else
            {
                // Section is data
                //levels.back()->deserialize(is);
            }
        } while(levels.size() > 0);
    }
}
