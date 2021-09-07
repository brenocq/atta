//--------------------------------------------------
// Atta File System
// nullFileWatcher.cpp
// Date: 2021-09-05
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/fileSystem/watchers/nullFileWatcher.h>

namespace atta
{
	void NullFileWatcher::addWatch(fs::path directory)
	{
		LOG_WARN("NullFileWatcher", "Watching files was not implemented to this operating system. Can't watch [w]$0", directory);
		FileWatcher::addWatch(directory);
	}

	void NullFileWatcher::removeWatch(fs::path directory)
	{
		FileWatcher::removeWatch(directory);
	}

	void NullFileWatcher::update()
	{

	}
}