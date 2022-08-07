//--------------------------------------------------
// Atta Script Module
// script.h
// Date: 2021-09-06
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_SCRIPT_SCRIPT_H
#define ATTA_SCRIPT_SCRIPT_H
#include <atta/component/componentManager.h>
#include <atta/component/entity.h>

namespace atta
{
	class Script               
	{
	public:
		Script() = default;  
		virtual ~Script() {};  
		virtual void update(Entity entity, float dt) = 0;
	}; 

#ifdef ATTA_STATIC_PROJECT
    template<typename T> class ScriptRegistration { static Script* reg; };
#define ATTA_REGISTER_SCRIPT(TYPE) \
    template<> \
    inline ::atta::Script* ::atta::ScriptRegistration<TYPE>::reg = ::atta::ScriptManager::registerScript(#TYPE, new TYPE());
#else
#define ATTA_REGISTER_SCRIPT(TYPE) \
    extern "C" {\
        std::pair<const char*, atta::Script*> createScript()\
        {\
            return { #TYPE, static_cast<atta::Script*>(new TYPE()) };\
        }\
    }
#endif

}


#endif// ATTA_SCRIPT_SCRIPT_H
