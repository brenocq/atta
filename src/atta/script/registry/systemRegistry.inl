//--------------------------------------------------
// Atta Script Module
// systemRegistry.inl
// Date: 2023-06-03
// By Breno Cunha Queiroz
//--------------------------------------------------
namespace atta::script {

template <typename T>
TypedSystemRegistry<T>::TypedSystemRegistry(std::string name) : SystemRegistry(name, typeid(T).name(), typeid(T).hash_code()), _systemScript({}) {
    addRegistry(this);
}

template <typename T>
TypedSystemRegistry<T>::~TypedSystemRegistry() {
    removeRegistry(this);
}

template <typename T>
void TypedSystemRegistry<T>::runOnLoad() {
    _systemScript.onLoad();
}

template <typename T>
void TypedSystemRegistry<T>::runOnUnload() {
    _systemScript.onUnload();
}

template <typename T>
void TypedSystemRegistry<T>::runOnUIRender() {
    _systemScript.onUIRender();
}

template <typename T>
void TypedSystemRegistry<T>::runOnAttaLoop() {
    _systemScript.onAttaLoop();
}

} // namespace atta::script
