// SPDX-License-Identifier: MIT
// SPDX-FileCopyrightText: 2020-2026 Breno Cunha Queiroz
#pragma once
#ifdef ATTA_OS_LINUX
#include <atta/file/watchers/fileWatcher.h>
#include <sys/inotify.h>

namespace atta::file {
class LinuxFileWatcher : public FileWatcher {

  public:
    LinuxFileWatcher();
    ~LinuxFileWatcher();

    void addWatch(fs::path directory) override;
    void removeWatch(fs::path directory) override;

    void update() override;

  private:
    void checkAndRead();
    void readEvents();

    void publishEvent(struct inotify_event* ievent);

    static int _inotifyFd;

    // Buffer to receive events
    char* _buf;
    size_t _bufLen;
};

} // namespace atta::file

#endif // ATTA_OS_LINUX
