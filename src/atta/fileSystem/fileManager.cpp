//--------------------------------------------------
// Atta Project
// fileManager.cpp
// Date: 2021-09-05
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/fileSystem/fileManager.h>

namespace atta
{
	FileManager::FileManager():
		_projectDefined(false)
	{

	}

	bool FileManager::setProjectFileImpl(fs::path projectFile)
	{
		if(!fs::exists(projectFile))
		{
			LOG_WARN("FileManager", "[w]$0[] does not exists", fs::absolute(projectFile));
			return false;
		}
		if(projectFile.extension() != ".atta")
		{
			LOG_WARN("FileManager", "[w]$0[] does not exists", fs::absolute(projectFile));
			return false;
		}

		_projectFile = projectFile; 
		_projectDirectory = projectFile.parent_path(); 
		_projectDefined = true;
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
		_projectDefined = false;
	}
}
