//--------------------------------------------------
// Atta Script Module
// worldRegistry.inl
// Date: 2023-05-25
// By Breno Cunha Queiroz
//--------------------------------------------------

namespace atta::script {

template <typename T>
TypedWorldRegistry<T>::TypedWorldRegistry() : WorldRegistry(typeid(T).name(), typeid(T).hash_code()), _worldScript({}) {
    addRegistry((WorldRegistry*)this);
}

template <typename T>
TypedWorldRegistry<T>::~TypedWorldRegistry() {
    removeRegistry((WorldRegistry*)this);
}

template <typename T>
void TypedWorldRegistry<T>::runOnLoad() {
    _worldScript.onLoad();
}

template <typename T>
void TypedWorldRegistry<T>::runOnUnload() {
    _worldScript.onUnload();
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
void TypedWorldRegistry<T>::runOnContinue() {
    _worldScript.onContinue();
}

template <typename T>
void TypedWorldRegistry<T>::runOnPause() {
    _worldScript.onPause();
}

template <typename T>
void TypedWorldRegistry<T>::runOnUpdateBefore(float dt) {
    _worldScript.onUpdateBefore(dt);
}

template <typename T>
void TypedWorldRegistry<T>::runOnUpdateAfter(float dt) {
    _worldScript.onUpdateAfter(dt);
}

template <typename T>
void TypedWorldRegistry<T>::runOnUIRender() {
    _worldScript.onUIRender();
}

template <typename T>
void TypedWorldRegistry<T>::runOnAttaLoop() {
    _worldScript.onAttaLoop();
}

} // namespace atta::script
