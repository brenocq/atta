//--------------------------------------------------
// Atta Project
// nullFileWatcher.cpp
// Date: 2021-09-05
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/fileSystem/watcher/nullFileWatcher.h>

namespace atta
{
	void NullFileWatcher::addWatch(fs::path pathName)
	{
		LOG_WARN("NullFileWatcher", "Watching files was not implemented to this operating system. Can't watch [w]$0", pathName);
		FileWatcher::addWatch(pathName);
	}

	void NullFileWatcher::removeWatch(fs::path pathName)
	{
		FileWatcher::removeWatch(pathName);
	}

	void NullFileWatcher::update()
	{

	}
}
