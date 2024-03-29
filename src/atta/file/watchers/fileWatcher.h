//--------------------------------------------------
// Atta File Module
// fileWatcher.h
// Date: 2021-09-05
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_FILE_WATCHER_FILE_WATCHER_H
#define ATTA_FILE_WATCHER_FILE_WATCHER_H

namespace atta::file {

class FileWatcher {
  public:
    FileWatcher() = default;
    virtual ~FileWatcher() = default;

    virtual void addWatch(fs::path directory);
    virtual void removeWatch(fs::path directory);
    virtual void update() = 0;

  protected:
    std::map<fs::path, int> _pathsToWatch;
};

} // namespace atta::file

#endif // ATTA_FILE_WATCHER_FILE_WATCHER_H
