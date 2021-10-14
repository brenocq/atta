//--------------------------------------------------
// Atta Component System
// factory.inl
// Date: 2021-09-17
// By Breno Cunha Queiroz
//--------------------------------------------------
namespace atta
{
    template <typename T>
    T* Factory::getComponent(uint64_t cloneId)
    {
        for(const auto& [typeHash, memory] : _componentMemories)
        {
            if(typeHash == typeid(T).hash_code())
                return reinterpret_cast<T*>(memory)+cloneId;
        }
        return nullptr;
    }
}
