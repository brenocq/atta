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
#include <atta/eventSystem/events/projectSaveEvent.h>
#include <atta/cmakeConfig.h>

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
		// Check valid project file
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

		// Close project if still open
		if(_project != nullptr)
			closeProjectImpl();

		// Update project
		_project = std::make_shared<Project>(projectFile);
		_projectSerializer = std::make_shared<ProjectSerializer>(_project);
		_projectSerializer->deserialize();

		// Watch project directory file changes
		_fileWatcher->addWatch(_project->getDirectory());

		ProjectSaveEvent e;
		EventManager::publish(e);

		return true;
	}

	bool FileManager::isProjectOpenImpl() const
	{
		return _project != nullptr;
	}

	void FileManager::closeProjectImpl()
	{
		_projectSerializer->serialize();

		_fileWatcher->removeWatch(_project->getDirectory());
		_project.reset();
		_projectSerializer.reset();
	}

	// TODO remove
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
}
