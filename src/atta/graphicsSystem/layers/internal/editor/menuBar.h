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
		void openProjectModal();
		void createProjectModal();
		void viewportModals();

		bool _showPreferences;
		bool _showOpenProject;
		bool _showCreateProject;

		std::vector<bool> _viewportModals;// True/false for each viewport window
	};
}

#endif// ATTA_GRAPHICS_SYSTEM_LAYERS_INTERNAL_EDITOR_MENU_BAR_H
