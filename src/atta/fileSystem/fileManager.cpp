//--------------------------------------------------
// Atta Project
// fileManager.cpp
// Date: 2021-09-05
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/fileSystem/fileManager.h>
#include <atta/fileSystem/watcher/nullFileWatcher.h>
#include <atta/fileSystem/watcher/linuxFileWatcher.h>

namespace atta
{
	void FileManager::startUpImpl()
	{
		_projectDefined = false;

#ifdef ATTA_OS_LINUX
		_fileWatcher = static_pointer_cast<FileWatcher>(std::make_shared<LinuxFileWatcher>());
#else
		_fileWatcher = static_pointer_cast<FileWatcher>(std::make_shared<NullFileWatcher>());
#endif// ATTA_OS_LINUX
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
