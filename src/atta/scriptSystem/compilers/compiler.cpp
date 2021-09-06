//--------------------------------------------------
// Atta Script System
// compiler.cpp
// Date: 2021-09-05
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/scriptSystem/compilers/compiler.h>

namespace atta
{
	std::vector<std::string> Compiler::getTargets() const
	{
		std::vector<std::string> targets;
		targets.reserve(_targetFiles.size());

		for(auto& target : _targetFiles)
			targets.push_back(target.first);

		return targets;
	}
}
