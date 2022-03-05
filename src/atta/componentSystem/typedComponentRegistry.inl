//--------------------------------------------------
// Atta Component System
// typedComponentRegistry.inl
// Date: 2021-01-01
// By Breno Cunha Queiroz
//--------------------------------------------------
namespace atta
{
    template<typename T>
    TypedComponentRegistry<T>::TypedComponentRegistry():
        ComponentRegistry(sizeof(T), typeid(T).name(), typeid(T).hash_code())
    {
        //LOG_DEBUG("TypedComponentRegistry", "Created new registry for [w]$0[]", typeid(T).name());
        description = &getDescription();// Initialize description static variable
        ComponentRegistry::registerToComponentManager();
        //LOG_DEBUG("TypedComponentRegistry", "Registered [w]$0[]", description->name);
    }

    template<typename T>
    void TypedComponentRegistry<T>::renderUIImpl(T* component)
    {
        DASSERT(this != nullptr, "Trying to call TypedComponentRegistry<$0>::renderUI() on nullptr component", std::string(typeid(T).name()));

        // Check if full component renderUI was defined
        if(description->renderUI.find("") != description->renderUI.end())
        {
            std::string imguiId = "##"+description->name;
            description->renderUI[""](reinterpret_cast<void*>(component), imguiId);
            return;
        }

        const std::vector<AttributeDescription> attributeDescriptions = description->attributeDescriptions;

        // Render UI for each attribute
        for(unsigned i = 0; i < attributeDescriptions.size(); i++)
        {
            AttributeDescription aDesc = attributeDescriptions[i];

            // Calculate data and size
            void* data = (void*)((uint8_t*)component+aDesc.offset);
            unsigned size = (i == attributeDescriptions.size()-1) ? 
                sizeof(T)-aDesc.offset : 
                attributeDescriptions[i+1].offset - aDesc.offset;

            const std::string imguiId = "##"+description->name;

            // Render attribute
            ComponentRegistry::renderUIAttribute(aDesc, data, size, imguiId);
        }
    }

    template<typename T>
    void TypedComponentRegistry<T>::serializeImpl(std::ostream& os, T* component)
    {
        uint8_t* curr = reinterpret_cast<uint8_t*>(component);
        for(unsigned i = 0; i < description->attributeDescriptions.size(); i++)
        {
            auto aDesc = description->attributeDescriptions[i];
            unsigned size = (i == description->attributeDescriptions.size()-1)
                ? sizeof(T)-aDesc.offset 
                : description->attributeDescriptions[i+1].offset - aDesc.offset;

            // Custom defined serialization
            if(description->serialize.find(aDesc.name) != description->serialize.end())
                description->serialize[aDesc.name](os, (void*)curr);
            // Default serialization
            else
                switch(aDesc.type)
                {
                    case AttributeType::STRINGID:
                    {
                        StringId* sid = reinterpret_cast<StringId*>(curr);
                        std::string str = sid->getString();
                        os.write(str.c_str(), str.size());
                        os.put('\0');
                        break;
                    }
                    default:
                        os.write(reinterpret_cast<const char*>(curr), size);
                }
            curr += size;
        }
    }

    template<typename T>
    void TypedComponentRegistry<T>::deserializeImpl(std::istream& is, T* component)
    {
        char* curr = reinterpret_cast<char*>(component);
        for(unsigned i = 0; i < description->attributeDescriptions.size(); i++)
        {
            auto aDesc = description->attributeDescriptions[i];
            unsigned size = (i == description->attributeDescriptions.size()-1)
                ? sizeof(T)-aDesc.offset 
                : description->attributeDescriptions[i+1].offset - aDesc.offset;

            //LOG_DEBUG("TypedComponentRegistry", "Deserializing: $0, attrib: $1", description->name, aDesc.name);
            //LOG_DEBUG("TypedComponentRegistry", "Deserialize size: $0", description->deserialize.size());

            // Custom defined deserialization
            if(description->deserialize.find(aDesc.name) != description->deserialize.end())
            {
                description->deserialize[aDesc.name](is, (void*)curr);
            }
            // Default deserialization
            else
                switch(aDesc.type)
                {
                    case AttributeType::STRINGID:
                    {
                        // Calculate string size
                        int init = is.tellg();
                        while(is.get()!='\0' && !is.eof());
                        int size = int(is.tellg())-init-1;

                        // Return to string first char
                        is.seekg(init);

                        // Read string
                        std::string str;
                        str.resize(size);
                        is.read(&str[0], size);
                        is.ignore();// jump \0
                        StringId sid(str);
                        // Save stringId to component
                        memcpy(curr, reinterpret_cast<char*>(&sid), sizeof(StringId));
                        break;
                    }
                    default:
                        is.read(reinterpret_cast<char*>(curr), size);
                }
            curr += size;
        }
    }

    template<typename T>
    std::vector<uint8_t> TypedComponentRegistry<T>::getDefault()
    {
        std::vector<uint8_t> defaultData(sizeof(T));
        T* newComponent = reinterpret_cast<T*>(defaultData.data());
        *newComponent = T{};
        return defaultData;
    }
}
