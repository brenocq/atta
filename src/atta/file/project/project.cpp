//--------------------------------------------------
// Atta File Module
// project.cpp
// Date: 2021-09-15
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/file/project/project.h>
#include <atta/cmakeConfig.h>

namespace atta
{
    Project::Project(fs::path file):
        _name(file.stem().string()),
        _file(fs::absolute(file)), 
        _directory(file.parent_path())
    {
        fs::path projectResourcePath = _directory/"resources";
        if(fs::exists(projectResourcePath))
            _resourceRootPaths.push_back(projectResourcePath);
        _resourceRootPaths.push_back(fs::path(ATTA_DIR)/"resources");

        LOG_DEBUG("Project", "Opened project [*g]$0[] ([w]$1[])", _name, _file);
    }

    fs::path Project::getBuildDirectory()
    {
        return _directory/"build";
    }

    fs::path Project::getSnapshotDirectory()
    {
        return _directory/"snapshots";
    }

    fs::path Project::solveResourcePath(fs::path relativePath, bool mustExist)
    {
        if(!mustExist)
            return _directory/"resources"/relativePath;
        else
        {
            for(auto& root : _resourceRootPaths)	
            {
                fs::path full = root/relativePath;
                if(fs::exists(full))
                    return full;
            }
        }

        return fs::path();
    }
}
