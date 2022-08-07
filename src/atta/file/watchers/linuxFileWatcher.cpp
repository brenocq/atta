//--------------------------------------------------
// Atta File Module
// linuxFileWatcher.cpp
// Date: 2021-09-05
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifdef ATTA_OS_LINUX
#include <atta/file/watchers/linuxFileWatcher.h>
#include <atta/event/eventManager.h>
#include <atta/event/events/fileWatchEvent.h>

#include <unistd.h>// read
// Module error
#include <cstring>
#include <cerrno>

namespace atta
{
    constexpr size_t EVENT_SIZE = sizeof(struct inotify_event);
    int LinuxFileWatcher::_inotifyFd = -1;

    LinuxFileWatcher::LinuxFileWatcher()
    {
        ASSERT(_inotifyFd == -1, "Only one LinuxFileWatcher should be active!");

        _bufLen = 1024*(EVENT_SIZE+16);
        _buf = new char[_bufLen];

        _inotifyFd = inotify_init();
        if(_inotifyFd < 0)
        {
            LOG_WARN("LinuxFileWatcher", "Could not initialize inotify! Error ($0): $1", _inotifyFd, std::strerror(errno));
            return;
        }
    }

    LinuxFileWatcher::~LinuxFileWatcher()
    {
        // Remove watches
        for(auto pathToWatch : _pathsToWatch)
            inotify_rm_watch(_inotifyFd, pathToWatch.second);

        // Close file descriptor
        close(_inotifyFd);
        _inotifyFd = -1;
    }

    void LinuxFileWatcher::addWatch(fs::path directory)
    {
        // Get watch descriptor
        int wd = inotify_add_watch(_inotifyFd, fs::absolute(directory).c_str(), IN_MODIFY);
        if(wd < 0)
        {
            LOG_WARN("LinuxFileWatcher", "Could not add watch to $0! (Error ($1): $2)", directory, wd, std::strerror(errno));
            return;
        }

        _pathsToWatch[directory] = wd;
        //LOG_INFO("LinuxFileWatcher", "Watcher $0 created", directory, wd);

        // Add subdirectories
        for(auto& p : fs::directory_iterator(directory))
            if(p.is_directory())
            {
                // Ignore hidden directories and build directory
                if(p.path().stem().string()[0] != '.' && p.path().stem().string() != "build")
                    addWatch(p.path());
            }
    }

    void LinuxFileWatcher::removeWatch(fs::path directory)
    {
        FileWatcher::removeWatch(directory);
    }

    void LinuxFileWatcher::update()
    {
        // TODO move to another thread
        checkAndRead();
    }

    void LinuxFileWatcher::checkAndRead()
    {
        struct timeval time;
        time.tv_sec = 0;
        time.tv_usec = 0;

        fd_set rfds;
        int ret;

        // Zero-out the fd_set
        FD_ZERO(&rfds);
        // Add inotify fd
        FD_SET(_inotifyFd, &rfds);

        ret = select(_inotifyFd+1, &rfds, NULL, NULL, &time);

        if(ret < 0)
        {
            LOG_WARN("LinuxFileWatcher", "Error when trying to select inotify file descriptor. Error ($0): $1", ret, std::strerror(errno));
            return;
        }
        else if (!ret)
        {
            //LOG_WARN("LinuxFileWatcher", "Timeout when trying to select inotify file descriptor. Error ($0): $1", ret, std::strerror(errno));
            return;
        }
        else if(FD_ISSET(_inotifyFd, &rfds))
        {
            readEvents();
        }
    }

    void LinuxFileWatcher::readEvents()
    {
        int len = read(_inotifyFd, _buf, _bufLen);
        if(len < 0)
        {
            LOG_WARN("LinuxFileWatcher", "Error when trying to read inotify file descriptor. Error ($0): $1", len, std::strerror(errno));
            return;
        }
        else if(len > 0)
        {
            int i = 0;
            while(i < len)
            {
                struct inotify_event* ievent;
                ievent = (struct inotify_event *) &_buf[i];

                publishEvent(ievent);

                i += EVENT_SIZE + ievent->len;
            }
        }
    }

    void LinuxFileWatcher::publishEvent(struct inotify_event* ievent)
    {
        if(ievent->len)
        {
            //LOG_DEBUG("LinuxFileWatcher", "name=$0", ievent->name);
            //LOG_DEBUG("LinuxFileWatcher", "wd=$0 cookie=$2 len=$3",
            //		ievent->wd, 
            //		ievent->cookie, ievent->len);

            for(auto pathToWatch : _pathsToWatch)
            {
                if(pathToWatch.second == ievent->wd)
                {
                    FileWatchEvent e;

                    if(ievent->mask & IN_MODIFY)
                        e.action = static_cast<FileWatchEvent::Action>(e.action | FileWatchEvent::MODIFY);
                    if(ievent->mask & IN_DELETE)
                        e.action = static_cast<FileWatchEvent::Action>(e.action | FileWatchEvent::DELETE);
                    if(ievent->mask & IN_OPEN)
                        e.action = static_cast<FileWatchEvent::Action>(e.action | FileWatchEvent::OPEN);
                    if(ievent->mask & IN_CLOSE)
                        e.action = static_cast<FileWatchEvent::Action>(e.action | FileWatchEvent::CLOSE);

                    e.file = pathToWatch.first / ievent->name;

                    EventManager::publish(e);
                    break;
                }
            }
        }
    }
}
#endif// ATTA_OS_LINUX
