//--------------------------------------------------
// Atta Script System
// nullLinker.h
// Date: 2021-09-06
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_SCRIPT_SYSTEM_LINKERS_NULL_LINKER_H
#define ATTA_SCRIPT_SYSTEM_LINKERS_NULL_LINKER_H
#include <atta/scriptSystem/linkers/linker.h>

namespace atta
{
	class NullLinker : public Linker
	{
	public:
		NullLinker() = default;
		~NullLinker() = default;

		void linkTarget(StringId target, Script** script, ProjectScript** projectScript) override;
		virtual void releaseTarget(StringId target) override {};
	};
}

#endif// ATTA_SCRIPT_SYSTEM_LINKERS_NULL_LINKER_H
