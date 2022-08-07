//--------------------------------------------------
// Atta Memory Module
// manager.inl
// Date: 2021-08-21
// By Breno Cunha Queiroz
//--------------------------------------------------
namespace atta::memory {

template <typename T>
T* Manager::getAllocator(StringHash hash) {
    return static_cast<T*>(getInstance().getAllocatorImpl(hash));
}

template <typename T>
T** Manager::getAllocatorPtr(StringHash hash) {
    return static_cast<T**>(getInstance().getAllocatorPtrImpl(hash));
}

} // namespace atta::memory
