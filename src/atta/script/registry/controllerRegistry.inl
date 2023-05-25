//--------------------------------------------------
// Atta Script Module
// controllerRegistry.inl
// Date: 2023-05-25
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/parallel/interface.h>

namespace atta::script {

template <typename T>
TypedControllerRegistry<T>::TypedControllerRegistry() : ControllerRegistry(typeid(T).name(), typeid(T).hash_code()) {
    addRegistry(this);
}

template <typename T>
TypedControllerRegistry<T>::~TypedControllerRegistry() {
    removeRegistry(this);
}

template <typename T>
void TypedControllerRegistry<T>::run(cmp::Entity entity, float dt, uint32_t num) {
    parallel::run<T>(entity, dt, num);
}

} // namespace atta::script
