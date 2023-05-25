//--------------------------------------------------
// Atta Script Module
// controllerRegistry.inl
// Date: 2023-05-25
// By Breno Cunha Queiroz
//--------------------------------------------------

namespace atta::script {

template <typename T>
TypedControllerRegistry<T>::TypedControllerRegistry() : ControllerRegistry(typeid(T).name(), typeid(T).hash_code()) {
    addRegistry((ControllerRegistry*)this);
}

template <typename T>
TypedControllerRegistry<T>::~TypedControllerRegistry() {
    removeRegistry((ControllerRegistry*)this);
}

} // namespace atta::script
