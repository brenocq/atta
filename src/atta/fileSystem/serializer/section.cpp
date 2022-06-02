//--------------------------------------------------
// Atta File System
// section.cpp
// Date: 2022-04-22
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/fileSystem/serializer/section.h>

namespace atta
{
    Section::Section()
    {

    }

    bool Section::isUndefined() const
    {
        return !_data.has_value();
    }
    
    bool Section::isMap() const
    {
        return _data.type().hash_code() == typeid(std::map<std::string, Section>).hash_code();
    }

    bool Section::isVector() const
    {
        return _data.type().hash_code() == typeid(std::vector<Section>).hash_code();
    }

    bool Section::isValue() const
    { 
        return !(isVector() || isMap());
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
        else if(isValue())
            return "value";
        else if(isMap())
            return "map";
        else if(isVector())
            return "vector";
        return "";
    }

    //---------- Value ----------//

    //---------- Map ----------//
    std::map<std::string, Section>& Section::map()
    {
        return *std::any_cast<std::map<std::string, Section>>(&_data);
    }

    const std::map<std::string, Section>& Section::map() const
    {
        return *std::any_cast<std::map<std::string, Section>>(&_data);
    }

    Section& Section::operator[](std::string key)
    {
        if(!isMap())
            _data = std::map<std::string, Section>{};

        std::map<std::string, Section>* m = std::any_cast<std::map<std::string, Section>>(&_data);
        return (*m)[key];
    }

    //---------- Vector ----------//
    std::vector<Section>& Section::vector()
    {
        return *std::any_cast<std::vector<Section>>(&_data);
    }

    const std::vector<Section>& Section::vector() const
    {
        return *std::any_cast<std::vector<Section>>(&_data);
    }

    Section& Section::operator[](unsigned i)
    {
        if(!isVector())
            _data = std::vector<Section>{};

        std::vector<Section>* v = std::any_cast<std::vector<Section>>(&_data);
        return (*v)[i];
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

    //std::stringstream& operator <<(std::stringstream& ss, const Section& section)
    //{
    //    return ss << section.toString();
    //}
}
