//--------------------------------------------------
// Atta Component System
// factory.cpp
// Date: 2021-09-17
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/componentSystem/factory.h>

namespace atta
{
	Factory::Factory(CreateInfo info):
		_prototypeId(info.prototypeId), _maxClones(info.maxClones), _componentMemories(info.componentMemories)
	{

	}
	
	void Factory::runScripts()
	{

	}

	std::vector<size_t> Factory::getComponents() const
	{
		std::vector<size_t> components;

		for(auto [componentHash, memory] : _componentMemories)
			components.push_back(componentHash);

		return components;
	}
}
