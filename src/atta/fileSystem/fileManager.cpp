//--------------------------------------------------
// Atta File System
// fileManager.cpp
// Date: 2021-09-05
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/fileSystem/fileManager.h>
#include <atta/fileSystem/watchers/nullFileWatcher.h>
#include <atta/fileSystem/watchers/linuxFileWatcher.h>
#include <atta/eventSystem/eventManager.h>
#include <atta/eventSystem/events/projectOpenEvent.h>
#include <atta/eventSystem/events/projectCloseEvent.h>
#include <atta/componentSystem/componentManager.h>
#include <atta/cmakeConfig.h>

#ifdef ATTA_OS_WEB
// Need to use "linux" build-in commands instead of std::filesystem
#include <dirent.h> 
#endif

namespace atta
{
    void FileManager::startUpImpl()
    {
        _project = nullptr;
        _projectSerializer = nullptr;

#ifdef ATTA_OS_LINUX
        _fileWatcher = std::static_pointer_cast<FileWatcher>(std::make_shared<LinuxFileWatcher>());
#else
        _fileWatcher = std::static_pointer_cast<FileWatcher>(std::make_shared<NullFileWatcher>());
#endif
    }

    void FileManager::shutDownImpl()
    {

    }

    bool FileManager::openProjectImpl(fs::path projectFile)
    {
        // Check valid project file (skip file check if first time saving this project)
        if(!fs::exists(projectFile))
        {
            LOG_WARN("FileManager", "Could not find file [w]$0[]", fs::absolute(projectFile));
            return false;
        }
        if(projectFile.extension() != ".atta")
        {
            LOG_WARN("FileManager", "Project file must have .atta extension [w]$0[]", fs::absolute(projectFile));
            return false;
        }

        // Project open project is it exists
        if(_project != nullptr)
            closeProjectImpl();

        // Update project
        _project = std::make_shared<Project>(projectFile);
        _projectSerializer = std::make_shared<ProjectSerializer>(_project);

        // Clear components and read project file
        ComponentManager::clear();
        _projectSerializer->deserialize();

        // Watch project directory file changes
        _fileWatcher->addWatch(_project->getDirectory());

        ProjectOpenEvent e;
        EventManager::publish(e);

        return true;
    }

    bool FileManager::createProjectImpl(fs::path projectFile)
    {
        if(projectFile.extension() != ".atta")
        {
            LOG_WARN("FileManager", "Project file must have .atta extension [w]$0[]", fs::absolute(projectFile));
            return false;
        }

        // Close if there is an open project
        if(_project != nullptr)
            closeProjectImpl();

        // Update project
        _project = std::make_shared<Project>(projectFile);
        _projectSerializer = std::make_shared<ProjectSerializer>(_project);

        // Write data to the file
        _projectSerializer->serialize();

        // Watch project directory file changes
        _fileWatcher->addWatch(_project->getDirectory());

        ProjectOpenEvent e;
        EventManager::publish(e);

        return true;
    }

    void FileManager::saveProjectImpl()
    {
        if(_projectSerializer)
            _projectSerializer->serialize();
    }

    void FileManager::closeProjectImpl()
    {
        if(_project == nullptr)
            return;

        _fileWatcher->removeWatch(_project->getDirectory());

        _project.reset();
        _projectSerializer.reset();

        ProjectCloseEvent e;
        EventManager::publish(e);
    }

    bool FileManager::isProjectOpenImpl() const
    {
        return _project != nullptr;
    }

    // TODO remove and use multi threading
    void FileManager::updateImpl()
    {
        _fileWatcher->update();
    }

    fs::path FileManager::solveResourcePathImpl(fs::path relativePath)
    {
        if(_project != nullptr)
            return _project->solveResourcePath(relativePath);
        else
        {
            fs::path full = fs::path(ATTA_DIR)/"resources"/relativePath;
            if(fs::exists(full))
                return full;
            else 
                return fs::path();
        }
    }

    std::vector<fs::path> FileManager::getResourcePathsImpl() const
    {
        if(_project != nullptr)
            return _project->getResourceRootPaths();
        else
            return { fs::path(ATTA_DIR)/"resources" };
    }

    std::vector<fs::path> FileManager::getDirectoryFilesRecursive(fs::path directory)
    {
        std::vector<fs::path> files;
#ifndef ATTA_OS_WEB
        for(auto& p : fs::recursive_directory_iterator(directory))
            if(!p.is_directory())
                files.push_back(fs::relative(p.path(), directory));
#else
        // If compiling for the web, need to use system calls to get files recursivelly
        DIR *d;
        struct dirent *dir;

        std::stack<fs::path> directories;
        directories.push(directory);

        while(!directories.empty())
        {
            fs::path curr = directories.top();
            directories.pop();

            d = opendir(curr.string().c_str());
            // Check if curr is file or directory
            if(d)
            {
                // Push children directories to stack
                while((dir = readdir(d)) != NULL)
                    if(dir->d_name[0] != '.')
                        directories.push(curr/dir->d_name);
            }
            else
                // Push file to list
                files.push_back(fs::relative(curr, directory));
            closedir(d);
        }
#endif
        return files;
    }
}
