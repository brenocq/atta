//--------------------------------------------------
// Atta Graphics System
// menuBar.h
// Date: 2021-09-01
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_GRAPHICS_SYSTEM_LAYERS_INTERNAL_EDITOR_MENU_BAR_H
#define ATTA_GRAPHICS_SYSTEM_LAYERS_INTERNAL_EDITOR_MENU_BAR_H

namespace atta
{
	class MenuBar
	{
	public:
		MenuBar();

		void render();

	private:
		void preferences();

		bool _showPreferences;
	};
}

#endif// ATTA_GRAPHICS_SYSTEM_LAYERS_INTERNAL_EDITOR_MENU_BAR_H
