//--------------------------------------------------
// Atta Component Module
// factory.inl
// Date: 2021-09-17
// By Breno Cunha Queiroz
//--------------------------------------------------
namespace atta
{
    template <typename T>
    T* Factory::getComponent(uint64_t cloneId)
    {
        for(const auto& [componentId, memory] : _componentMemories)
        {
            if(componentId == typeid(T).hash_code())
                return reinterpret_cast<T*>(memory)+cloneId;
        }
        return nullptr;
    }
}
