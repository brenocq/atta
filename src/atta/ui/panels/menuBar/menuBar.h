//--------------------------------------------------
// Atta UI Module
// menuBar.h
// Date: 2021-09-01
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_UI_PANELS_MENU_BAR_MENU_BAR_H
#define ATTA_UI_PANELS_MENU_BAR_MENU_BAR_H

#include <atta/ui/panels/menuBar/localWindows/repoWindow.h>

namespace atta::ui {

class MenuBar {
  public:
    MenuBar();

    void render();

  private:
    void fileMenu();
    void editMenu();
    void windowMenu();
    void helpMenu();

    void preferences();         ///< Render preferences window
    void openProjectModal();    ///< Render open project modal
    void openPublishedWindow(); ///< Render open published project modal
    void createProjectModal();  ///< Render create project modal
    void saveProjectModal();    ///< Render save project modal

    bool _waitingChooseAttaFile;
    bool _showPreferences;      ///< Show preferences modal
    bool _showOpenProject;      ///< Show open project modal
    bool _showOpenPublished;    ///< Show open published project window
    bool _showPublishedProject; ///< Show published project window
    bool _showCreateProject;    ///< Show create project modal
    bool _showSaveProject;      ///< Show save project modal

    bool _quitAfterSaveModal; ///< If should quit application after save project modal response

    std::vector<RepoWindow> _repoWindows; ///< Which repository windows are open
};

} // namespace atta::ui

#endif // ATTA_UI_PANELS_MENU_BAR_MENU_BAR_H
