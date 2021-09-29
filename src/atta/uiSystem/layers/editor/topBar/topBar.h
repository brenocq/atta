//--------------------------------------------------
// Atta UI System
// topBar.h
// Date: 2021-09-01
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_UI_SYSTEM_LAYERS_EDITOR_TOP_BAR_TOP_BAR_H
#define ATTA_UI_SYSTEM_LAYERS_EDITOR_TOP_BAR_TOP_BAR_H

namespace atta::ui
{
	class TopBar
	{
	public:
		TopBar();

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

#endif// ATTA_UI_SYSTEM_LAYERS_EDITOR_TOP_BAR_TOP_BAR_H
