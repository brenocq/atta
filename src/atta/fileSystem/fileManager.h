//--------------------------------------------------
// Atta Project
// fileManager.h
// Date: 2021-09-05
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_FILE_SYSTEM_FILE_MANAGER_H
#define ATTA_FILE_SYSTEM_FILE_MANAGER_H

namespace atta
{
	class FileManager final
	{
	public:
		FileManager();

		static FileManager& getInstance()
		{
			static FileManager instance;
			return instance;
		}

		// Project managing
		static bool setProjectFile(fs::path projectFile) { return getInstance().setProjectFileImpl(projectFile); }
		static bool isProjectDefined() { return getInstance().isProjectDefinedImpl(); }
		static std::string getProjectName() { return getInstance().getProjectNameImpl(); }
		static void closeProject() { return getInstance().closeProjectImpl(); }

	private:
		bool setProjectFileImpl(fs::path projectFile);
		bool isProjectDefinedImpl() const;
		std::string getProjectNameImpl() const;
		void closeProjectImpl();

		fs::path _projectFile;
		fs::path _projectDirectory;
		bool _projectDefined;
	};
}

#endif// ATTA_FILE_SYSTEM_FILE_MANAGER_H
