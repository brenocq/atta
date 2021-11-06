//--------------------------------------------------
// Atta Script System
// linuxLinker.cpp
// Date: 2021-09-05
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/scriptSystem/linkers/linuxLinker.h>
#ifdef ATTA_OS_LINUX
#include <atta/fileSystem/fileManager.h>
#include <dlfcn.h>
#include <atta/scriptSystem/script.h>

#include <chrono>

namespace atta
{
    void LinuxLinker::linkTarget(StringId target, Script** script, ProjectScript** projectScript)
    {
        *script = nullptr;
        *projectScript = nullptr;

        std::chrono::time_point<std::chrono::system_clock> begin = std::chrono::system_clock::now();

        fs::path projectDir = FileManager::getProject()->getDirectory();
        fs::path lib = projectDir / "build" / ("lib"+target.getString()+".so").c_str();

        //LOG_DEBUG("LinuxLinker", "Linking target $0", lib);

        void* fLib = dlopen(fs::absolute(lib).c_str(), RTLD_LAZY);
        if(fLib)
        {
            //---------- Script ----------//
            using ScriptCreator = atta::Script* (*)();
            ScriptCreator fn = reinterpret_cast<ScriptCreator>(dlsym(fLib, "createScript"));

            const char* dlsymError = dlerror();
            //if(dlsymError)
            //{
            //	LOG_WARN("LinuxLinker", "Could not get script from library $0. Error: $1", lib.filename(), dlsymError);
            //	dlclose(fLib);
            //	return;
            //}

            if(!dlsymError)
                *script = fn();

            //---------- Project Script ----------//
            using ProjectScriptCreator = atta::ProjectScript* (*)();
            ProjectScriptCreator pfn = reinterpret_cast<ProjectScriptCreator>(dlsym(fLib, "createProjectScript"));

            dlsymError = dlerror();
            //if(dlsymError)
            //{
            //	LOG_WARN("LinuxLinker", "Could not get project script from library $0. Error: $1", lib.filename(), dlsymError);
            //	dlclose(fLib);
            //	return;
            //}

            if(!dlsymError)
                *projectScript = pfn();

            // Save library pointer to delete later
            _targetHandles[target] = fLib;

            std::chrono::time_point<std::chrono::system_clock> end = std::chrono::system_clock::now();
            auto micro = std::chrono::duration_cast<std::chrono::microseconds>(end - begin);
            //LOG_INFO("LinuxLinker", "Time to link: $0 ms", micro.count()/1000.0f);
        }
        else
        {
            LOG_WARN("LinuxLinker", "Cannot open library $0. Error: $1", lib.filename(), dlerror());
            return;
        }
    }

    void LinuxLinker::releaseTarget(StringId target)
    {
        if(_targetHandles.find(target) != _targetHandles.end())
        {
            dlclose(_targetHandles[target]);
            _targetHandles.erase(target);
        }
    }
}
#endif// ATTA_OS_LINUX
