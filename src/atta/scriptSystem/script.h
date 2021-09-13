//--------------------------------------------------
// Atta Script System
// script.h
// Date: 2021-09-06
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_SCRIPT_SYSTEM_SCRIPT_H
#define ATTA_SCRIPT_SYSTEM_SCRIPT_H

namespace atta
{
	class Script               
	{
	public:
		Script() = default;    
		virtual ~Script() {};  
		virtual void update(size_t index, float delta = 0.0f) = 0;
	}; 
   
}

#endif// ATTA_SCRIPT_SYSTEM_SCRIPT_H
