//--------------------------------------------------
// Atta Script Module
// controllerRegistry.inl
// Date: 2023-05-25
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/parallel/interface.h>

namespace atta::script {

template <typename T>
TypedControllerRegistry<T>::TypedControllerRegistry(std::string name) : ControllerRegistry(name, typeid(T).name(), typeid(T).hash_code()) {
    ControllerRegistry::addRegistry(this);
}

template <typename T>
TypedControllerRegistry<T>::~TypedControllerRegistry() {
    ControllerRegistry::removeRegistry(this);
}

template <typename T>
void TypedControllerRegistry<T>::run(cmp::Entity entity, float dt, uint32_t num) const {
    parallel::run<T>(entity, dt, num);
}

} // namespace atta::script