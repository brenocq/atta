//--------------------------------------------------
// Atta Resource System
// resourceManager.cpp
// Date: 2021-09-07
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/resourceSystem/resourceManager.h>
#include <atta/memorySystem/memoryManager.h>

namespace atta
{
	ResourceManager& ResourceManager::getInstance()
	{
		static ResourceManager instance;
		return instance;
	}

	void ResourceManager::startUp() { getInstance().startUpImpl(); }
	void ResourceManager::startUpImpl()
	{
		//----- System Memory -----//
		// Get main memory
		Allocator* mainAllocator = MemoryManager::getAllocator(SSID("MainAllocator"));
		size_t size = 512*1024*1024;// 512MB
		// Alloc memory inside main memory
		uint8_t* resourceMemory = static_cast<uint8_t*>(mainAllocator->allocBytes(size, sizeof(uint8_t)));
		_allocator = new BitmapAllocator(resourceMemory, size);
		MemoryManager::registerAllocator(SSID("ResourceAllocator"), static_cast<Allocator*>(_allocator));
	}

	void ResourceManager::shutDown() { getInstance().shutDownImpl(); }
	void ResourceManager::shutDownImpl()
	{

	}
}
