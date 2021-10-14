//--------------------------------------------------
// Atta File System
// fileManager.h
// Date: 2021-09-05
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_FILE_SYSTEM_FILE_MANAGER_H
#define ATTA_FILE_SYSTEM_FILE_MANAGER_H
#include <atta/fileSystem/watchers/fileWatcher.h>
#include <atta/fileSystem/project/project.h>
#include <atta/fileSystem/project/projectSerializer.h>

namespace atta
{
    class FileManager final
    {
    public:
        static FileManager& getInstance()
        {
            static FileManager instance;
            return instance;
        }

        static void startUp() { getInstance().startUpImpl(); }
        static void shutDown() { getInstance().shutDownImpl(); }

        // Project
        static bool openProject(fs::path projectFile, bool newProject = false) { return getInstance().openProjectImpl(projectFile, newProject); }
        static bool isProjectOpen() { return getInstance().isProjectOpenImpl(); }
        static std::shared_ptr<Project> getProject() { return getInstance().getProjectImpl(); }
        static void closeProject(bool save = true) { return getInstance().closeProjectImpl(save); }
        static void saveProject() { return getInstance().saveProjectImpl(); }
        static void saveNewProject(fs::path projectFile) { return getInstance().saveNewProjectImpl(projectFile); }

        // Receives a relative resource path and searches the registered directories for that file
        // By default searches on the <ATTA_DIR>/resources and <PROJECT_DIR>/resources directories
        // The return is the absolute resource path
        static fs::path solveResourcePath(fs::path relativePath) { return getInstance().solveResourcePathImpl(relativePath); }
        static std::vector<fs::path> getResourcePaths() { return getInstance().getResourcePathsImpl(); }

        // Update watcher (TODO remove)
        static void update() { getInstance().updateImpl(); };

    private:
        void startUpImpl();
        void shutDownImpl();

        bool openProjectImpl(fs::path projectFile, bool newProject = false);
        bool isProjectOpenImpl() const;
        std::shared_ptr<Project> getProjectImpl() const { return _project; }
        void closeProjectImpl(bool save = true);
        void saveProjectImpl();
        void saveNewProjectImpl(fs::path projectFile);

        fs::path solveResourcePathImpl(fs::path relativePath);
        std::vector<fs::path> getResourcePathsImpl() const;

        // TODO remove
        void updateImpl();

        std::shared_ptr<FileWatcher> _fileWatcher;
        std::shared_ptr<Project> _project;
        std::shared_ptr<ProjectSerializer> _projectSerializer;
    };
}

#endif// ATTA_FILE_SYSTEM_FILE_MANAGER_H
