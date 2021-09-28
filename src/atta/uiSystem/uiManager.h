//--------------------------------------------------
// Atta UI System
// uiManager.h
// Date: 2021-09-28
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_UI_SYSTEM_UI_MANAGER_H
#define ATTA_UI_SYSTEM_UI_MANAGER_H

namespace atta::ui
{
	class UIManager final
	{
	public:
		static UIManager& getInstance();
		static void startUp();
		static void shutDown();

	private:
		void startUpImpl();
		void shutDownImpl();
	};
}

#endif// ATTA_UI_SYSTEM_UI_MANAGER_H
