//--------------------------------------------------
// Atta Component Module
// typedRegistry.inl
// Date: 2021-01-01
// By Breno Cunha Queiroz
//--------------------------------------------------
namespace atta::component {

template <typename T>
TypedRegistry<T>::TypedRegistry(std::string name) : Registry(name, sizeof(T), typeid(T).name(), typeid(T).hash_code()) {
    // Initialize description static variable
    description = &getDescription();

    // Register
    std::vector<Registry*>& regs = getRegistries();
    regs.push_back(this);

    // Save id
    id = _id = regs.size() - 1;
    LOG_DEBUG("component::TypedRegistry", "New component [w]$0 -> $1", name, id);
}

template <typename T>
void TypedRegistry<T>::renderUI(Component* component) {
    DASSERT(this != nullptr, "Trying to call TypedRegistry<$0>::renderUI() on nullptr component", std::string(typeid(T).name()));

    // Check if full component renderUI was defined
    if (description->renderUI.find("") != description->renderUI.end()) {
        std::string imguiId = "##" + description->name;
        description->renderUI[""](reinterpret_cast<void*>(component), imguiId);
        return;
    }

    const std::vector<AttributeDescription> attributeDescriptions = description->attributeDescriptions;

    // Render UI for each attribute
    for (unsigned i = 0; i < attributeDescriptions.size(); i++) {
        AttributeDescription aDesc = attributeDescriptions[i];

        // Calculate data and size
        void* data = (void*)((uint8_t*)component + aDesc.offset);
        unsigned size = (i == attributeDescriptions.size() - 1) ? sizeof(T) - aDesc.offset : attributeDescriptions[i + 1].offset - aDesc.offset;

        const std::string imguiId = "##" + description->name;

        // Render attribute
        Registry::renderUIAttribute(aDesc, data, size, imguiId);
    }
}

template <typename T>
void TypedRegistry<T>::serialize(std::ostream& os, Component* component) {
    uint8_t* curr = reinterpret_cast<uint8_t*>(component);
    for (unsigned i = 0; i < description->attributeDescriptions.size(); i++) {
        auto aDesc = description->attributeDescriptions[i];
        unsigned size = (i == description->attributeDescriptions.size() - 1) ? sizeof(T) - aDesc.offset
                                                                             : description->attributeDescriptions[i + 1].offset - aDesc.offset;

        // Custom defined serialization
        if (description->serialize.find(aDesc.name) != description->serialize.end())
            description->serialize[aDesc.name](os, (void*)curr);
        // Default serialization
        else
            switch (aDesc.type) {
                case AttributeType::STRINGID: {
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

template <typename T>
void TypedRegistry<T>::deserialize(std::istream& is, Component* component) {
    char* curr = reinterpret_cast<char*>(component);
    for (unsigned i = 0; i < description->attributeDescriptions.size(); i++) {
        auto aDesc = description->attributeDescriptions[i];
        unsigned size = (i == description->attributeDescriptions.size() - 1) ? sizeof(T) - aDesc.offset
                                                                             : description->attributeDescriptions[i + 1].offset - aDesc.offset;
        // Custom defined deserialization
        if (description->deserialize.find(aDesc.name) != description->deserialize.end()) {
            description->deserialize[aDesc.name](is, (void*)curr);
        }
        // Default deserialization
        else
            switch (aDesc.type) {
                case AttributeType::STRINGID: {
                    // Calculate string size
                    int init = is.tellg();
                    while (is.get() != '\0' && !is.eof())
                        ;
                    int size = int(is.tellg()) - init - 1;

                    // Return to string first char
                    is.seekg(init);

                    // Read string
                    std::string str;
                    str.resize(size);
                    is.read(&str[0], size);
                    is.ignore(); // jump \0
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

template <typename T>
std::vector<uint8_t> TypedRegistry<T>::getDefault() {
    std::vector<uint8_t> defaultData(sizeof(T));
    T* newComponent = reinterpret_cast<T*>(defaultData.data());
    *newComponent = T{};
    return defaultData;
}

} // namespace atta::component
