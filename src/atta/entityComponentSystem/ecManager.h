//--------------------------------------------------
// Atta Entity-Component System
// ecManager.h
// Date: 2021-08-17
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_ENTITY_COMPONENT_SYSTEM_EC_MANAGER_H
#define ATTA_ENTITY_COMPONENT_SYSTEM_EC_MANAGER_H
#include <atta/eventSystem/event.h>

namespace atta
{
	class ECManager final
	{
	public:
		ECManager(uint);

	private:
		uint8_t* _memory;
		size_t _memSize;
	};
}

#endif// ATTA_ENTITY_COMPONENT_SYSTEM_EC_MANAGER_H
