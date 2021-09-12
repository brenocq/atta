//--------------------------------------------------
// Atta Script System
// scriptManager.cpp
// Date: 2021-09-06
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/scriptSystem/scriptManager.h>
#include <atta/eventSystem/eventManager.h>
#include <atta/eventSystem/events/fileEvent.h>
#include <atta/eventSystem/events/projectEvent.h>
#include <atta/scriptSystem/compilers/nullCompiler.h>
#include <atta/scriptSystem/compilers/linuxCompiler.h>
#include <atta/scriptSystem/linkers/nullLinker.h>
#include <atta/scriptSystem/linkers/linuxLinker.h>

namespace atta
{
	ScriptManager& ScriptManager::getInstance()
	{
		static ScriptManager instance;
		return instance;
	}

	void ScriptManager::startUp() { getInstance().startUpImpl(); }
	void ScriptManager::startUpImpl()
	{
#ifdef ATTA_OS_LINUX
		_compiler = std::static_pointer_cast<Compiler>(std::make_shared<LinuxCompiler>());
		_linker = std::static_pointer_cast<Linker>(std::make_shared<LinuxLinker>());
#else
		_compiler = std::static_pointer_cast<Compiler>(std::make_shared<NullCompiler>());
		_linker = std::static_pointer_cast<Linker>(std::make_shared<NullLinker>());
#endif

		EventManager::subscribe(SSID("File"), BIND_EVENT_FUNC(ScriptManager::onFileChange));
		EventManager::subscribe(SSID("Project"), BIND_EVENT_FUNC(ScriptManager::onProjectChange));
	}

	void ScriptManager::shutDown() { getInstance().shutDownImpl(); }
	void ScriptManager::shutDownImpl()
	{

	}

	void ScriptManager::onFileChange(Event& event)
	{
		FileEvent& e = reinterpret_cast<FileEvent&>(event);
		LOG_DEBUG("ScriptManager", "New event: $0", e);

		if(e.file.filename() == "CMakeLists.txt")
			updateTargets();

		for(auto target : _compiler->getTargetFiles())
			for(auto file : target.second)
				if(file == e.file)
				{
					_compiler->compileTarget(target.first);
					_linker->linkTarget(target.first);// TODO check error
				}
	}

	void ScriptManager::onProjectChange(Event& event)
	{
		ProjectEvent& e = reinterpret_cast<ProjectEvent&>(event);
		updateTargets();
	}

	void ScriptManager::updateTargets()
	{
		_compiler->updateTargets();

		for(auto target : _compiler->getTargets())
			_linker->linkTarget(target);// TODO check error

		LOG_DEBUG("ScriptManager", "Targets: $0", _compiler->getTargets());
	}
}
