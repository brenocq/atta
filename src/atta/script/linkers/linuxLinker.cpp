//--------------------------------------------------
// Atta Script Module
// linuxLinker.cpp
// Date: 2021-09-05
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/script/linkers/linuxLinker.h>
#ifdef ATTA_OS_LINUX
#include <atta/file/manager.h>
#include <dlfcn.h>

#include <chrono>

namespace atta::script {

void LinuxLinker::linkTarget(StringId target) {
    std::chrono::time_point<std::chrono::system_clock> begin = std::chrono::system_clock::now();

    fs::path projectDir = file::getProject()->getDirectory();
    fs::path lib = projectDir / "build" / ("lib" + target.getString() + ".so").c_str();

    // Copy lib to temporary file (should not reload same .so file)
    fs::path libCpyDir = projectDir / "build" / "atta" / "script" / target.getString();

    // Keep some of the last library versions (segfault if library still in use)
    if (fs::exists(libCpyDir)) {
        // Get files
        std::vector<std::string> files;
        for (auto entry : fs::directory_iterator(libCpyDir))
            files.push_back(entry.path().string());

        std::sort(files.begin(), files.end());
        // Keep up to 10 old files (should be only one)
        if (files.size())
            for (int i = 0; i < int(files.size()) - 10; i++)
                fs::remove(files[i]);
    }
    fs::create_directories(libCpyDir);
    unsigned long uniqueNum = std::chrono::duration_cast<std::chrono::milliseconds>(begin.time_since_epoch()).count();
    fs::path libCpy = libCpyDir / ("lib" + target.getString() + "." + std::to_string(uniqueNum) + ".so").c_str();
    fs::copy(lib, libCpy);

    // Open shared library
    void* fLib = dlopen(fs::absolute(libCpy).c_str(), RTLD_LAZY);
    std::chrono::time_point<std::chrono::system_clock> end = std::chrono::system_clock::now();
    auto linkTime = std::chrono::duration_cast<std::chrono::microseconds>(end - begin);

    if (fLib)
        LOG_INFO("script::LinuxLinker", "Time to link [*w]$0[]: [w]$1ms", target, linkTime.count() / 1000.0f);
    else
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
