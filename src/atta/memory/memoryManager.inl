//--------------------------------------------------
// Atta Memory
// memoryManager.inl
// Date: 2021-08-21
// By Breno Cunha Queiroz
//--------------------------------------------------
namespace atta
{
	template <typename T>
	std::shared_ptr<T> MemoryManager::getAllocator(StringHash hash)
	{
		return std::static_pointer_cast<T>(getInstance().getAllocatorImpl(hash));
	}
}
