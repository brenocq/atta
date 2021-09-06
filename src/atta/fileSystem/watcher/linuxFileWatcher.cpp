//--------------------------------------------------
// Atta Project
// linuxFileWatcher.cpp
// Date: 2021-09-05
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/fileSystem/watcher/linuxFileWatcher.h>
#include <atta/eventSystem/eventManager.h>
#include <atta/eventSystem/events/fileEvent.h>

#ifdef ATTA_OS_LINUX
#include <unistd.h>// read
// System error
#include <cstring>
#include <cerrno>

namespace atta
{
	constexpr size_t EVENT_SIZE = sizeof(struct inotify_event);
	int LinuxFileWatcher::_inotifyFd = -1;
	bool LinuxFileWatcher::_active = false;

	LinuxFileWatcher::LinuxFileWatcher()
	{
		ASSERT(!_active, "Only one LinuxFileWatcher should be active!");

		_bufLen = 1024*(EVENT_SIZE+16);
		_buf = new char[_bufLen];
	}

	LinuxFileWatcher::~LinuxFileWatcher()
	{
		_active = false;
	}

	void LinuxFileWatcher::addWatch(fs::path pathName)
	{
		if(_inotifyFd < 0)
		{
			_inotifyFd = inotify_init();
			if(_inotifyFd < 0)
			{
				LOG_WARN("LinuxFileWatcher", "Could not initialize inotify! Can't watch files. Error ($0): $1", _inotifyFd, std::strerror(errno));
				return;
			}
		}

		// Get watch descriptor
		int wd = inotify_add_watch(_inotifyFd, fs::absolute(pathName).c_str(), IN_MODIFY);
		if(wd < 0)
		{
			LOG_WARN("LinuxFileWatcher", "Could not add watch to $0! (Error ($1): $2)", pathName, wd, std::strerror(errno));
			return;
		}

		_pathsToWatch[pathName] = wd;
		LOG_INFO("LinuxFileWatcher", "Watch $0 created -> $1", pathName, wd);
	}

	void LinuxFileWatcher::removeWatch(fs::path pathName)
	{
		FileWatcher::removeWatch(pathName);
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
			size_t i = 0;
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
			LOG_DEBUG("LinuxFileWatcher", "name=$0", ievent->name);
			LOG_DEBUG("LinuxFileWatcher", "wd=$0 cookie=$2 len=$3",
					ievent->wd, 
					ievent->cookie, ievent->len);

			for(auto pathToWatch : _pathsToWatch)
			{
				if(pathToWatch.second == ievent->wd)
				{
					FileEvent e;

					if(ievent->mask & IN_MODIFY)
						e.action = e.action | FileEvent::Action::MODIFY;
					if(ievent->mask & IN_DELETE)
						e.action = e.action | FileEvent::Action::DELETE;
					if(ievent->mask & IN_OPEN)
						e.action = e.action | FileEvent::Action::OPEN;
					if(ievent->mask & IN_CLOSE)
						e.action = e.action | FileEvent::Action::CLOSE;

					e.file = pathToWatch.first / ievent->name;

					EventManager::publish(e);
					break;
				}
			}
		}
	}
}
#endif// ATTA_OS_LINUX
