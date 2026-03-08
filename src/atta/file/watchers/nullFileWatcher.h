// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2020-2026 Breno Cunha Queiroz
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
