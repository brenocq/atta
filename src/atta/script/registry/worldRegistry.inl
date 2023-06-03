//--------------------------------------------------
// Atta Script Module
// worldRegistry.inl
// Date: 2023-05-25
// By Breno Cunha Queiroz
//--------------------------------------------------
namespace atta::script {

template <typename T>
TypedWorldRegistry<T>::TypedWorldRegistry(std::string name) : WorldRegistry(name, typeid(T).name(), typeid(T).hash_code()), _worldScript({}) {
    addRegistry(this);
}

template <typename T>
TypedWorldRegistry<T>::~TypedWorldRegistry() {
    removeRegistry(this);
}

template <typename T>
void TypedWorldRegistry<T>::runOnStart() {
    _worldScript.onStart();
}

template <typename T>
void TypedWorldRegistry<T>::runOnStop() {
    _worldScript.onStop();
}

template <typename T>
void TypedWorldRegistry<T>::runOnUpdateBefore() {
    _worldScript.onUpdateBefore();
}

template <typename T>
void TypedWorldRegistry<T>::runOnUpdateAfter() {
    _worldScript.onUpdateAfter();
}

} // namespace atta::script
