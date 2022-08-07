//--------------------------------------------------
// Atta File Module
// linuxFileWatcher.h
// Date: 2021-09-05
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_FILE_WATCHER_LINUX_FILE_WATCHER_H
#define ATTA_FILE_WATCHER_LINUX_FILE_WATCHER_H
#ifdef ATTA_OS_LINUX
#include <atta/file/watchers/fileWatcher.h>
#include <sys/inotify.h>

namespace atta
{
    class LinuxFileWatcher : public FileWatcher
    {
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
}

#endif// ATTA_OS_LINUX
#endif// ATTA_FILE_WATCHER_LINUX_FILE_WATCHER_H
