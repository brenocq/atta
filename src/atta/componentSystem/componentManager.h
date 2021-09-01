//--------------------------------------------------
// Atta Entity-Component System
// ecManager.h
// Date: 2021-08-17
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_COMPONENT_SYSTEM_COMPONENT_MANAGER_H
#define ATTA_COMPONENT_SYSTEM_COMPONENT_MANAGER_H
#include <atta/eventSystem/event.h>

namespace atta
{
	class ComponentManager final
	{
	public:
		ComponentManager();

	private:
		uint8_t* _memory;
		size_t _memSize;
	};
}

#endif// ATTA_COMPONENT_SYSTEM_COMPONENT_MANAGER_H
