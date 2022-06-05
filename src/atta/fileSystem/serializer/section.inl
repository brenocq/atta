//--------------------------------------------------
// Atta File System
// section.inl
// Date: 2022-04-22
// By Breno Cunha Queiroz
//--------------------------------------------------

namespace atta
{
    template<typename Test, template<typename...> class Ref>
    struct is_specialization : std::false_type {};
    template<template<typename...> class Ref, typename... Args>
    struct is_specialization<Ref<Args...>, Ref>: std::true_type {};

    //---------- SectionData ----------//
    template<typename T>
    void SectionData::operator=(T&& value)
    {
        using U = std::decay_t<T>;
        registerType<U>();
        _typeHash = StringId(typeid(U).name());

        //----- Store serializable object -----//
        if constexpr(std::is_base_of_v<atta::Serializable, U>)
        {
            // Allocate space and copy to _data
            _data.resize(value.getSerializedSize());

            // Serialize to temporary buffer
            std::stringstream ss;
            value.serialize(ss);

            // Copy to _data
            ss.seekg(0, std::ios_base::beg);
            ss.read((char*)_data.data(), _data.size());
        }
        //----- Store std::vector -----//
        else if constexpr(is_specialization<U, std::vector>::value)
        {
            // If it is std::vector
            using V = typename U::value_type;

            if constexpr(std::is_base_of_v<atta::Serializable, V>)
            {
                // If it is std::vector of serializable
                _data.clear();
                for(auto el : value)
                {
                    // Create temporary buffer
                    std::vector<uint8_t> newData(el.getSerializedSize());

                    // Serialize to temporary buffer
                    std::stringstream ss;
                    el.serialize(ss);
                    ss.seekg(0, std::ios_base::beg);
                    ss.read((char*)newData.data(), newData.size());

                    // Copy newData to _data
                    _data.insert(_data.end(), newData.begin(), newData.end());
                }
            }
            else
            {
                // Allocate space
                _data.resize(sizeof(V)*value.size());

                // Copy data
                V* data = (V*)_data.data();
                for(unsigned i = 0; i < value.size(); i++)
                {
                    *data = value[i];
                    data++;
                }
            }
        }
        //----- Store std::string -----//
        else if constexpr(std::is_same_v<U, std::string>)
        {
            // If it is std::string
            _data.resize(value.size());
            for(unsigned i = 0; i < value.size(); i++)
                _data[i] = value[i];
        }
        //----- Store other types -----//
        else
        {
            // If it is any other type
            //LOG_DEBUG("SectionData", "Attrib $0", typeid(T).name());
            _data.resize(sizeof(U));
            U* u = (U*)_data.data();
            *u = value;
        }
    }

    template<typename T>
    T SectionData::get()
    {
        // Set type if it was not set yet
        if(_typeHash == "undefined"_sid) 
        {
            _typeHash = StringId(typeid(T).name());
            registerType<T>();
        }

        return getConst<T>();
    }

    template<typename T>
    T SectionData::getConst() const
    {
        if(_data.empty())
            return T{};

        //----- Get serializable object -----//
        if constexpr(std::is_base_of_v<atta::Serializable, T>)
        {
            // Serialize to temporary buffer
            std::stringstream ss;
            write(ss, _data.data(), _data.size());
            
            T t;
            t.deserialize(ss);
            return t;
        }
        //----- Get std::vector -----//
        else if constexpr(is_specialization<T, std::vector>::value)
        {
            using V = typename T::value_type;// Vector value type

            // Build vector from data and return
            if(_typeHash == StringId(typeid(T).name()))
            {
                // Create return vector
                T vec;

                if constexpr(std::is_base_of_v<atta::Serializable, V>)
                {
                    // If it is std::vector of serializable
                
                    // Serialize to temporary buffer
                    std::stringstream ss;
                    write(ss, _data.data(), _data.size());
                    
                    // Deserialize all elements
                    while((size_t)ss.tellg() < _data.size())
                    {
                        V v;
                        v.deserialize(ss);
                        vec.push_back(v);
                    }
                }
                else
                {
                    vec.resize(_data.size()/sizeof(V));

                    // Copy to return vector
                    const V* data = (const V*)&_data.at(0);
                    for(unsigned i = 0; i < vec.size(); i++)
                        vec[i] = data[i];

                }
                return vec;

            }
            else 
            {
                LOG_WARN("Filesystem::SectionData", "Wrong section casting to [w]$0[] (aka [w]std::vector<$1>[]), value is of type [w]$2[]", 
                       typeid(T).name(), typeid(V).name(), _typeHash);
                return T{};
            }
        }
        //----- Get std::string -----//
        else if constexpr(std::is_same_v<T, std::string>)
        {
            std::string str(_data.begin(), _data.end());
            return str;
        }
        //----- Get other types -----//
        else
        {
            // Return data
            const T* ptr = getPtrConst<T>();
            if(ptr == nullptr)
                return T{};
            else
            {
                T copy = *ptr;
                return copy;
            }
        }
    }

    template<typename T>
    T* SectionData::getPtr()
    {
        // Set type if it was not set yet
        if(_typeHash == "undefined"_sid) 
        {
            _typeHash = StringId(typeid(T).name());
            registerType<T>();
        }

        return getPtrConst<T>();
    }

    template<typename T>
    T* SectionData::getPtrConst() const
    {
        if(_data.size() > 0 && _typeHash == StringId(typeid(T).name()))
            return (T*)&_data[0];
        else 
        {
            LOG_WARN("Filesystem::SectionData", "Wrong section casting to [w]$0[], value is of type [w]$1[]", typeid(T).name(), _typeHash);
            return nullptr;
        }
    }

    template<typename T>
    void SectionData::registerType()
    {
        if(_typeToString.find(StringId(typeid(T).name())) == _typeToString.end())
        {
            _typeToString[StringId(typeid(T).name())] =
                [](std::vector<uint8_t> data)
                {
                    //----- Print std::vector -----//
                    if constexpr(is_specialization<T, std::vector>::value)
                    {
                        using V = typename T::value_type;// Vector value type
                        //T vec;
                        //vec.resize(data.size()/sizeof(V));
                        //const V* d = (const V*)&data.at(0);
                        //for(unsigned i = 0; i < vec.size(); i++)
                        //    vec[i] = d[i];

                        //std::stringstream ss;
                        //ss << vec;
                        //return std::string(ss.str());
                        return "std::vector<" + std::string(typeid(V).name()) + ">";
                    }
                    //----- Print std::string -----//
                    else if constexpr (std::is_same_v<std::string,T> || std::is_same_v<const char*,T>)
                    {
                        // If it is string
                        std::string str(data.begin(), data.end());
                        return "\"" + str + "\"";
                    }
                    //----- Print others -----//
                    else
                    {
                        //if constexpr(is_streamable<std::stringstream, T>::value)
                        //{
                        //    // If it is printable
                        //    std::stringstream ss;
                        //    ss << *(T*)(&data[0]);
                        //    return std::string(ss.str());
                        //}
                        //else
                            // Otherwise, print name
                        return std::string(typeid(T).name());
                    }

                    return std::string("");
                };
        }
    }

    //---------- Section ----------//
    template <typename T>
    Section::Section(T value)
    {
        if constexpr(std::is_same_v<T, std::vector<Section>>)
            vector() = value;
        else if constexpr(std::is_same_v<T, std::map<std::string, Section>>)
            map() = value;
        else
            data() = value;
    }
    
    template <typename T>
    void Section::operator=(T value)
    {
        if constexpr(std::is_same_v<T, std::vector<Section>>)
            vector() = value;
        else if constexpr(std::is_same_v<T, std::map<std::string, Section>>)
            map() = value;
        else
            data() = value;
    }

    template <typename T>
    void Section::operator=(std::initializer_list<T> list)
    {
        if constexpr(std::is_same_v<T, Section>)
            vector() = std::vector<Section>(list);
        else
            data() = std::vector<T>(list);
    }

    template <typename T>
    Section::operator T()
    {
        if(isData())
            return _data.get<T>();
        else
            LOG_WARN("Filesystem::Section", "Trying to cast section as [w]$0[], but this section is not data", typeid(T).name());

        return T{};
    }
}
