//--------------------------------------------------
// Atta Script Module
// linuxLinker.cpp
// Date: 2021-09-05
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/script/linkers/linuxLinker.h>
#ifdef ATTA_OS_LINUX
#include <atta/file/manager.h>
#include <atta/script/script.h>
#include <dlfcn.h>

#include <chrono>

namespace atta::script {

void LinuxLinker::linkTarget(StringId target, Script** script, ProjectScript** projectScript, std::string& name) {
    *script = nullptr;
    *projectScript = nullptr;

    std::chrono::time_point<std::chrono::system_clock> begin = std::chrono::system_clock::now();

    fs::path projectDir = file::getProject()->getDirectory();
    fs::path lib = projectDir / "build" / ("lib" + target.getString() + ".so").c_str();

    // Copy lib to temporary file (should not reload same .so file)
    fs::path libCpyDir = projectDir / "build" / "atta" / "script" / target.getString();

    // Make sure to only save the new the old files
    if (fs::exists(libCpyDir))
    {
        // Get files
        std::vector<std::string> files;
        for(auto entry : fs::directory_iterator(libCpyDir))
            files.push_back(entry.path().string());

        std::sort(files.begin(), files.end());
        // Remove all old files (should be only one)
        if(files.size())
            for(unsigned i = 0; i < files.size()-1; i++)
                fs::remove(files[i]);
    }
    fs::create_directories(libCpyDir);
    unsigned long uniqueNum = std::chrono::duration_cast<std::chrono::milliseconds>(begin.time_since_epoch()).count();
    fs::path libCpy = libCpyDir / ("lib" + target.getString() + "." + std::to_string(uniqueNum) + ".so").c_str();
    fs::copy(lib, libCpy);

    // Open shared library
    void* fLib = dlopen(fs::absolute(libCpy).c_str(), RTLD_LAZY);
    if (fLib) {
        //---------- Script ----------//
        using ScriptCreator = std::pair<const char*, Script*> (*)();
        ScriptCreator fn = reinterpret_cast<ScriptCreator>(dlsym(fLib, "createScript"));

        const char* dlsymError = dlerror();
        if (!dlsymError) {
            *script = fn().second;
            name = std::string(fn().first);
        }

        //---------- Project Script ----------//
        using ProjectScriptCreator = std::pair<const char*, ProjectScript*> (*)();
        ProjectScriptCreator pfn = reinterpret_cast<ProjectScriptCreator>(dlsym(fLib, "createProjectScript"));

        dlsymError = dlerror();
        if (!dlsymError) {
            *projectScript = pfn().second;
            name = std::string(pfn().first);
        }

        // Save library pointer to delete later
        _targetHandles[target] = fLib;

        std::chrono::time_point<std::chrono::system_clock> end = std::chrono::system_clock::now();
        auto micro = std::chrono::duration_cast<std::chrono::microseconds>(end - begin);

        std::string type;
        if (*projectScript)
            type = "ProjectScript " + name;
        else if (*script)
            type = "Script " + name;
        else
            type = "Other";
        LOG_INFO("script::LinuxLinker", "Time to link [*w]$0[]: [w]$1[] ms ([w]$2[])", target, micro.count() / 1000.0f, type);
    } else
        LOG_ERROR("script::LinuxLinker", "Cannot open library [w]$0[]. Error: $1", libCpy.filename(), dlerror());
}

void LinuxLinker::releaseTarget(StringId target) {
    if (_targetHandles.find(target) != _targetHandles.end()) {
        dlclose(_targetHandles[target]);

        const char* errStr = dlerror();
        if (errStr) {
            LOG_ERROR("script::LinuxLinker", "Could not release target [w]$0[]: [w]$1", target, errStr);
            return;
        }
        _targetHandles.erase(target);
    } else
        LOG_WARN("script::LinuxLinker", "Could not release target [w]$0[]: [w]Target not found", target);
}

} // namespace atta::script

#endif // ATTA_OS_LINUX
