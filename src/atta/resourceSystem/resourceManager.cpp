//--------------------------------------------------
// Atta Resource System
// resourceManager.cpp
// Date: 2021-09-07
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/resourceSystem/resourceManager.h>
#include <atta/memorySystem/memoryManager.h>
#include <atta/eventSystem/eventManager.h>
#include <atta/eventSystem/events/meshLoadEvent.h>
#include <atta/eventSystem/events/textureLoadEvent.h>

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

		// Default resources
		ResourceManager::get<Mesh>("meshes/plane.obj");
		ResourceManager::get<Mesh>("meshes/cube.obj");
		ResourceManager::get<Mesh>("meshes/sphere.obj");
		ResourceManager::get<Mesh>("meshes/triangle.obj");
	}

	void ResourceManager::shutDown() { getInstance().shutDownImpl(); }
	void ResourceManager::shutDownImpl()
	{

	}

	template <>
	void ResourceManager::createLoadEvent<Mesh>(Mesh* resource, StringId sid)
	{
		MeshLoadEvent e(sid);
		EventManager::publish(e);
	}
	template <>
	void ResourceManager::createLoadEvent<Texture>(Texture* resource, StringId sid)
	{
		TextureLoadEvent e(sid);
		EventManager::publish(e);
	}
}
