//--------------------------------------------------
// Atta Script Module
// typedRegistry.inl
// Date: 2023-05-25
// By Breno Cunha Queiroz
//--------------------------------------------------

namespace atta::script {

template <typename T>
TypedRegistry<T>::TypedRegistry() : Registry(typeid(T).name(), typeid(T).hash_code()) {
    addRegistry((Registry*)this);
}

template <typename T>
TypedRegistry<T>::~TypedRegistry() {
    removeRegistry((Registry*)this);
}

} // namespace atta::script
