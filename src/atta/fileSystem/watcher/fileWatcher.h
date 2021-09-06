//--------------------------------------------------
// Atta Project
// fileWatcher.h
// Date: 2021-09-05
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_FILE_SYSTEM_WATCHER_FILE_WATCHER_H
#define ATTA_FILE_SYSTEM_WATCHER_FILE_WATCHER_H

namespace atta
{
	class FileWatcher
	{
	public:
		FileWatcher() = default;
		virtual ~FileWatcher() = default;

		virtual void addWatch(fs::path pathName);
		virtual void removeWatch(fs::path pathName);
		virtual void update() = 0;

	protected:
		std::map<fs::path, int> _pathsToWatch;
	};
}

#endif// ATTA_FILE_SYSTEM_WATCHER_FILE_WATCHER_H
