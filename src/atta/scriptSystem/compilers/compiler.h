//--------------------------------------------------
// Atta Script System
// compiler.h
// Date: 2021-09-05
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_SCRIPT_SYSTEM_COMPILERS_COMPILER_H
#define ATTA_SCRIPT_SYSTEM_COMPILERS_COMPILER_H
#include <atta/core/stringId.h>

namespace atta
{
	class Compiler
	{
	public:
		Compiler() = default;
		virtual ~Compiler() = default;

		virtual void compileAll() = 0;
		virtual void compileTarget(StringId target) = 0;
		virtual void updateTargets() = 0;


		std::vector<StringId> getTargets() const;
		std::map<StringId, std::vector<fs::path>> getTargetFiles() const { return _targetFiles; };

	protected:
		std::map<StringId, std::vector<fs::path>> _targetFiles;
	};
}

#endif// ATTA_SCRIPT_SYSTEM_COMPILERS_COMPILER_H
