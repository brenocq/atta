//--------------------------------------------------
// Atta Project
// fileWatcher.cpp
// Date: 2021-09-05
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/fileSystem/watcher/fileWatcher.h>

namespace atta
{
	void FileWatcher::addWatch(fs::path pathName)
	{
		_pathsToWatch[pathName] = -1;
	}

	void FileWatcher::removeWatch(fs::path pathName)
	{
		_pathsToWatch.erase(pathName);
	}
}
