//--------------------------------------------------
// Atta Script System
// projectScript.h
// Date: 2021-09-16
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_SCRIPT_SYSTEM_PROJECT_SCRIPT_H
#define ATTA_SCRIPT_SYSTEM_PROJECT_SCRIPT_H

namespace atta
{
	class ProjectScript               
	{
	public:
		ProjectScript() = default;    
		virtual ~ProjectScript() {};  
		virtual void onStart() = 0;
		virtual void onStop() = 0;

		virtual void onPlay() = 0;
		virtual void onPause() = 0;

		virtual void onUpdateBefore(float delta) = 0;
		virtual void onUpdateAfter(float delta) = 0;

		virtual void onLoad() = 0;
		virtual void onUnload() = 0;
	}; 
}

#endif// ATTA_SCRIPT_SYSTEM_PROJECT_SCRIPT_H
