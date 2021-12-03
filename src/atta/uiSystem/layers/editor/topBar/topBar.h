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
        void preferences();///< Render preferences window
        void openProjectModal();///< Render open project modal
        void createProjectModal();///< Render create project modal
        void saveProjectModal();///< Render save project modal
		/** The viewport modal is used to change the viewport settings */
        void viewportModals();///< Render viewport modal

        bool _showPreferences;///< Show preferences modal
        bool _showOpenProject;///< Show open project modal
        bool _showCreateProject;///< Show create project modal
        bool _showSaveProject;///< Show save project modal

		bool _quitAfterSaveModal;///< If should quit application after save project modal response

		/** True/false for each viewport window */
        std::vector<bool> _viewportModals;///< Which viewports should be rendered
    };
}

#endif// ATTA_UI_SYSTEM_LAYERS_EDITOR_TOP_BAR_TOP_BAR_H
