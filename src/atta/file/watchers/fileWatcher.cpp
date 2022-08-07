//--------------------------------------------------
// Atta File Module
// fileWatcher.cpp
// Date: 2021-09-05
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/file/watchers/fileWatcher.h>

namespace atta
{
    void FileWatcher::addWatch(fs::path directory)
    {
        _pathsToWatch[directory] = -1;
    }

    void FileWatcher::removeWatch(fs::path directory)
    {
        _pathsToWatch.erase(directory);
    }
}
