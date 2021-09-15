//--------------------------------------------------
// Atta Script System
// scriptManager.cpp
// Date: 2021-09-06
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/scriptSystem/scriptManager.h>
#include <atta/eventSystem/eventManager.h>
#include <atta/eventSystem/events/fileWatchEvent.h>
#include <atta/eventSystem/events/projectOpenEvent.h>
#include <atta/eventSystem/events/projectCloseEvent.h>
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

		EventManager::subscribe<FileWatchEvent>(BIND_EVENT_FUNC(ScriptManager::onFileChange));
		EventManager::subscribe<ProjectOpenEvent>(BIND_EVENT_FUNC(ScriptManager::onProjectOpen));
		EventManager::subscribe<ProjectCloseEvent>(BIND_EVENT_FUNC(ScriptManager::onProjectClose));
	}

	void ScriptManager::shutDown() { getInstance().shutDownImpl(); }
	void ScriptManager::shutDownImpl()
	{

	}

	Script* ScriptManager::getScript(StringId target) { return getInstance().getScriptImpl(target); } 
	Script* ScriptManager::getScriptImpl(StringId target) const
	{
		if(_scripts.find(target) != _scripts.end())
			return _scripts.at(target);
		else
			return nullptr;
	}

	void ScriptManager::onFileChange(Event& event)
	{
		FileWatchEvent& e = reinterpret_cast<FileWatchEvent&>(event);

		if(e.file.filename() == "CMakeLists.txt")
		{
			updateAllTargets();
			return;
		}

		for(auto target : _compiler->getTargetFiles())
			for(auto file : target.second)
				if(file == e.file)
					updateTarget(target.first);

		LOG_DEBUG("ScriptManager", "New event: $0", e);
	}

	void ScriptManager::onProjectOpen(Event& event)
	{
		//ProjectOpenEvent& e = reinterpret_cast<ProjectOpenEvent&>(event);
		updateAllTargets();
	}

	void ScriptManager::onProjectClose(Event& event)
	{
		// Release all targets
		for(auto target : _compiler->getTargets())
			_linker->releaseTarget(target);

		_scripts.clear();
	}

	void ScriptManager::updateAllTargets()
	{
		// Release all targets
		for(auto target : _compiler->getTargets())
			_linker->releaseTarget(target);

		for(auto [key, script] : _scripts)
			delete script;
		_scripts.clear();

		// Recompile targets
		_compiler->compileAll();
		_compiler->updateTargets();

		// For each target in the project, link the script
		for(auto target : _compiler->getTargets())
		{
			Script* script = nullptr;
			_linker->linkTarget(target, &script);
			if(script != nullptr)
				_scripts[target] = script;

		}

		LOG_DEBUG("ScriptManager", "Targets updated: $0", _compiler->getTargets());
	}
	void ScriptManager::updateTarget(StringId target)
	{
		// Release this targets
		if(_scripts.find(target) != _scripts.end())
			delete _scripts[target];
		_scripts.erase(target);
		_linker->releaseTarget(target);

		// Compile and link specific target
		_compiler->compileTarget(target);

		Script* script = nullptr;
		_linker->linkTarget(target, &script);
		if(script != nullptr)
		{
			_scripts[target] = script;
			LOG_WARN("ScriptManager", "Target $0 -> calling test", target);
		}
		else
			LOG_WARN("ScriptManager", "Target $0 does not have an script", target);
	}
}
