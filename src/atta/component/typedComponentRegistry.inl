//--------------------------------------------------
// Atta Component Module
// typedComponentRegistry.inl
// Date: 2021-01-01
// By Breno Cunha Queiroz
//--------------------------------------------------
namespace atta::component {

template <typename T>
TypedComponentRegistry<T>::TypedComponentRegistry() : ComponentRegistry(sizeof(T), typeid(T).name(), typeid(T).hash_code()) {
    description = &getDescription(); // Initialize description static variable
    ComponentRegistry::registerToManager();
}

template <typename T>
std::vector<uint8_t> TypedComponentRegistry<T>::getDefault() {
    std::vector<uint8_t> defaultData(sizeof(T));
    T* newComponent = reinterpret_cast<T*>(defaultData.data());
    *newComponent = T{};
    return defaultData;
}

} // namespace atta::component
