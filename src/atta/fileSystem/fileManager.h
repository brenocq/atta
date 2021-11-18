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
#include <atta/eventSystem/eventManager.h>

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
        static bool openProject(fs::path projectFile) { return getInstance().openProjectImpl(projectFile); }
        static bool createProject(fs::path projectFile) { return getInstance().createProjectImpl(projectFile); }
        static void saveProject() { getInstance().saveProjectImpl(); }
        static void closeProject() { getInstance().closeProjectImpl(); }
        static bool isProjectOpen() { return getInstance().isProjectOpenImpl(); }
        static std::shared_ptr<Project> getProject() { return getInstance().getProjectImpl(); }

        // Receives a relative resource path and searches the registered directories for that file
        // By default searches on the <ATTA_DIR>/resources and <PROJECT_DIR>/resources directories
        // The return is the absolute resource path
        static fs::path solveResourcePath(fs::path relativePath) { return getInstance().solveResourcePathImpl(relativePath); }
        static std::vector<fs::path> getResourcePaths() { return getInstance().getResourcePathsImpl(); }
        static std::vector<fs::path> getDirectoryFilesRecursive(fs::path directory);

        // Update watcher (TODO remove)
        static void update() { getInstance().updateImpl(); };

    private:
        void startUpImpl();
        void shutDownImpl();

        bool openProjectImpl(fs::path projectFile);
        bool createProjectImpl(fs::path projectFile);
        void saveProjectImpl();
        void closeProjectImpl();
        bool isProjectOpenImpl() const;
        std::shared_ptr<Project> getProjectImpl() const { return _project; }

        fs::path solveResourcePathImpl(fs::path relativePath);
        std::vector<fs::path> getResourcePathsImpl() const;

        // Handle events
        void onSimulationStateChange(Event& event);

        // TODO remove
        void updateImpl();

        std::shared_ptr<FileWatcher> _fileWatcher;
        std::shared_ptr<Project> _project;
        std::shared_ptr<ProjectSerializer> _projectSerializer;
        bool _simulationRunning;
    };
}

#endif// ATTA_FILE_SYSTEM_FILE_MANAGER_H
