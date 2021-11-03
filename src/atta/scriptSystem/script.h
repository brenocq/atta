//--------------------------------------------------
// Atta Script System
// script.h
// Date: 2021-09-06
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_SCRIPT_SYSTEM_SCRIPT_H
#define ATTA_SCRIPT_SYSTEM_SCRIPT_H
#include <atta/componentSystem/componentManager.h>
#include <atta/componentSystem/factory.h>

namespace atta
{
	class Script               
	{
	public:
		Script() = default;  
		virtual ~Script() {};  
		virtual void update(EntityId id, float delta = 0.0f) = 0;
		virtual void update(const std::vector<uint8_t*>& memories, uint64_t cloneId, float delta = 0.0f) = 0;
	}; 
}

#define ATTA_REGISTER_SCRIPT(DerivedScript) \
extern "C" {\
    atta::Script* createScript()\
    {\
        return static_cast<atta::Script*>(new DerivedScript());\
    }\
}

#endif// ATTA_SCRIPT_SYSTEM_SCRIPT_H
