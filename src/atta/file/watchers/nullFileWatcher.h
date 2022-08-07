//--------------------------------------------------
// Atta File Module
// nullFileWatcher.h
// Date: 2021-09-05
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_FILE_WATCHER_NULL_FILE_WATCHER_H
#define ATTA_FILE_WATCHER_NULL_FILE_WATCHER_H
#include <atta/file/watchers/fileWatcher.h>

namespace atta::file {

class NullFileWatcher : public FileWatcher {
  public:
    NullFileWatcher() = default;
    virtual ~NullFileWatcher() = default;

    void addWatch(fs::path directory) override;
    void removeWatch(fs::path directory) override;

    void update() override;
};

} // namespace atta::file

#endif // ATTA_FILE_WATCHER_NULL_FILE_WATCHER_H
