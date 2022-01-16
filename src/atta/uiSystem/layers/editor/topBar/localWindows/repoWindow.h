//--------------------------------------------------
// Atta UI System
// repoWindow.h
// Date: 2021-12-26
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_UI_SYSTEM_LAYERS_EDITOR_TOP_BAR_LOCAL_WINDOWS_REPO_WINDOW_H
#define ATTA_UI_SYSTEM_LAYERS_EDITOR_TOP_BAR_LOCAL_WINDOWS_REPO_WINDOW_H
#include <atta/componentSystem/componentManager.h>
#include <atta/componentSystem/entity.h>

namespace atta::ui
{
    class RepoWindow
    {
    public:
        RepoWindow(fs::path repoPath);

        void render();

        fs::path getRepoPath() { return _repoPath; }
        bool getShouldClose() { return _shouldClose; }

    private:
        struct ReadmeMetadata
        {
            std::string title;
            std::string description;
            std::string image;
        };

        struct ProjectInfo
        {
            ReadmeMetadata metadata;
            fs::path attaFile;
        };

        /// Search for projects recursivelly and populate _projects
        void findProjects();
        /// Parse README.md file and extract metadata
        ReadmeMetadata parseReadmeMetadata(fs::path readme);

        fs::path _repoPath;
        std::string _title;
        std::string _description;
        std::vector<ProjectInfo> _projects;

        bool _shouldClose;
    };
}

#endif// ATTA_UI_SYSTEM_LAYERS_EDITOR_TOP_BAR_LOCAL_WINDOWS_REPO_WINDOW_H