//--------------------------------------------------
// Atta File System
// project.h
// Date: 2021-09-15
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_FILE_SYSTEM_PROJECT_PROJECT_H
#define ATTA_FILE_SYSTEM_PROJECT_PROJECT_H
#include <atta/fileSystem/project/project.h>

namespace atta
{
	class FileManager;
	class Project final
	{
	public:
		Project(fs::path file);

		std::string getName() { return _name; }
		fs::path getFile() const { return _file; }
		fs::path getDirectory() const { return _directory; }

		fs::path getBuildDirectory();
		fs::path getSnapshotDirectory();

	private:
		fs::path solveResourcePath(fs::path relativePath);

		std::string _name;

		fs::path _file;// .atta file
		fs::path _directory;

		std::vector<fs::path> _resourceRootPaths;

		friend FileManager;
	};
}

#endif// ATTA_FILE_SYSTEM_PROJECT_PROJECT_H

