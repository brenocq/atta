//--------------------------------------------------
// Atta Project
// fileManager.h
// Date: 2021-09-05
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_FILE_SYSTEM_FILE_MANAGER_H
#define ATTA_FILE_SYSTEM_FILE_MANAGER_H
#include <atta/fileSystem/watcher/fileWatcher.h>

namespace atta
{
	class FileManager final
	{
	public:
		static FileManager& getInstance()
		{
			static FileManager instance;
			return instance;
		}

		static void startUp() { getInstance().startUpImpl(); }
		static void shutDown() { getInstance().shutDownImpl(); }

		// Project managing
		static bool setProjectFile(fs::path projectFile) { return getInstance().setProjectFileImpl(projectFile); }
		static bool isProjectDefined() { return getInstance().isProjectDefinedImpl(); }
		static std::string getProjectName() { return getInstance().getProjectNameImpl(); }
		static void closeProject() { return getInstance().closeProjectImpl(); }

		// TODO remove
		static void update() { getInstance().updateImpl(); };

	private:
		void startUpImpl();
		void shutDownImpl();
		bool setProjectFileImpl(fs::path projectFile);
		bool isProjectDefinedImpl() const;
		std::string getProjectNameImpl() const;
		void closeProjectImpl();

		// TODO remove
		void updateImpl();

		fs::path _projectFile;
		fs::path _projectDirectory;
		bool _projectDefined;
		std::shared_ptr<FileWatcher> _fileWatcher;
	};
}

#endif// ATTA_FILE_SYSTEM_FILE_MANAGER_H
