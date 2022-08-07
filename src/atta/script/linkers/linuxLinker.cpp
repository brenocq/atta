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

    fs::path projectDir = file::Manager::getProject()->getDirectory();
    fs::path lib = projectDir / "build" / ("lib" + target.getString() + ".so").c_str();

    // LOG_DEBUG("script::LinuxLinker", "Linking target $0", lib);

    void* fLib = dlopen(fs::absolute(lib).c_str(), RTLD_LAZY);
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
            type = "Component";
        LOG_INFO("script::LinuxLinker", "Time to link [w]$0[]: $1 ms ($2)", target, micro.count() / 1000.0f, type);
    } else {
        LOG_WARN("script::LinuxLinker", "Cannot open library $0. Error: $1", lib.filename(), dlerror());
        return;
    }
}

void LinuxLinker::releaseTarget(StringId target) {
    if (_targetHandles.find(target) != _targetHandles.end()) {
        // FIXME error when releasing target
        // dlclose(_targetHandles[target]);
        _targetHandles.erase(target);
    }
}

} // namespace atta::script

#endif // ATTA_OS_LINUX
