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
#include <atta/eventSystem/events/projectEvent.h>

namespace atta
{
	void FileManager::startUpImpl()
	{
		_projectDefined = false;

#ifdef ATTA_OS_LINUX
		_fileWatcher = std::static_pointer_cast<FileWatcher>(std::make_shared<LinuxFileWatcher>());
#else
		_fileWatcher = std::static_pointer_cast<FileWatcher>(std::make_shared<NullFileWatcher>());
#endif
	}

	void FileManager::shutDownImpl()
	{

	}

	bool FileManager::setProjectFileImpl(fs::path projectFile)
	{
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
		if(_projectDefined)
			closeProjectImpl();

		_projectFile = projectFile; 
		_projectDirectory = projectFile.parent_path(); 
		_projectDefined = true;

		_fileWatcher->addWatch(_projectDirectory);

		ProjectEvent e;
		EventManager::publish(e);

		return true;
	}

	bool FileManager::isProjectDefinedImpl() const
	{
		return _projectDefined;
	}

	std::string FileManager::getProjectNameImpl() const
	{
		return _projectFile.stem();
	}

	fs::path FileManager::getProjectDirectoryImpl() const
	{
		return _projectDirectory;
	}

	void FileManager::closeProjectImpl()
	{
		_fileWatcher->removeWatch(_projectDirectory);
		_projectDefined = false;
	}

	// TODO remove
	void FileManager::updateImpl()
	{
		_fileWatcher->update();
	}
}
