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
	void LinuxLinker::linkTarget(std::string target)
	{
		std::chrono::time_point<std::chrono::system_clock> begin = std::chrono::system_clock::now();

		fs::path projectDir = FileManager::getProjectDirectory();
		fs::path lib = projectDir / "build" / ("lib"+target+".so").c_str();

		LOG_DEBUG("LinuxLinker", "Linking target $0", lib);

		void* fLib = dlopen(fs::absolute(lib).c_str(), RTLD_NOW);
		if(!fLib)
		{
			LOG_WARN("LinuxLinker", "Cannot open library $0. Error: $1", lib.filename(), dlerror());
			return;
		}

		if(fLib)
		{
			using ScriptCreator = Script* (*)();
			ScriptCreator fn = reinterpret_cast<ScriptCreator>(dlsym(fLib, "createScript"));

			const char* dlsymError = dlerror();
			if(dlsymError)
			{
				LOG_WARN("LinuxLinker", "Could not get script from library $0. Error: $1", lib.filename(), dlsymError);
				dlclose(fLib);
				return;
			}

			std::chrono::time_point<std::chrono::system_clock> end = std::chrono::system_clock::now();
			auto micro = std::chrono::duration_cast<std::chrono::microseconds>(end - begin);
			LOG_INFO("LinuxLinker", "Time to link: $0 ms", micro.count()/1000.0f);

			Script* testScript = fn();
			testScript->update(0);
			delete testScript;
			fn = nullptr;

			dlclose(fLib);
		}

	}
}
#endif// ATTA_OS_LINUX