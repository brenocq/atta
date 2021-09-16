//--------------------------------------------------
// Atta Script System
// nullLinker.cpp
// Date: 2021-09-05
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/scriptSystem/linkers/nullLinker.h>

namespace atta
{
	void NullLinker::linkTarget(StringId target, Script** script, ProjectScript** projectScript)
	{
		*script = nullptr;
		*projectScript = nullptr;
	}
}
