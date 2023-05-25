//--------------------------------------------------
// Atta Script Module
// controller.inl
// Date: 2023-05-25
// By Breno Cunha Queiroz
//--------------------------------------------------

namespace atta::script {

template <typename T>
ATTA_CPU_GPU T* Controller::get() const {
    return _entity.get<T>();
}

} // namespace atta::script
