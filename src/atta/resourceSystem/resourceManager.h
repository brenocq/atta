//--------------------------------------------------
// Atta Resource System
// resourceManager.h
// Date: 2021-09-07
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_RESOURCE_SYSTEM_RESOURCE_MANAGER_H
#define ATTA_RESOURCE_SYSTEM_RESOURCE_MANAGER_H
#include <atta/memorySystem/allocators/bitmapAllocator.h>

namespace atta
{
	class ResourceManager final
	{
	public:
		static ResourceManager& getInstance();
		static void startUp();
		static void shutDown();

		template <typename R>
		R* get(const fs::path& filename) { getInstance().getImpl<R>(filename); }

	private:
		void startUpImpl();
		void shutDownImpl();

		template <typename R>
		R* getImpl(const fs::path& filename);

		BitmapAllocator* _allocator;
	};
}

#endif// ATTA_RESOURCE_SYSTEM_RESOURCE_MANAGER_H
