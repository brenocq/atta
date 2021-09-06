//--------------------------------------------------
// Atta Script System
// nullCompiler.h
// Date: 2021-09-05
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_SCRIPT_SYSTEM_COMPILERS_NULL_COMPILER_H
#define ATTA_SCRIPT_SYSTEM_COMPILERS_NULL_COMPILER_H
#include <atta/scriptSystem/compilers/compiler.h>

namespace atta
{
	class NullCompiler : public Compiler
	{
	public:
		NullCompiler() = default;

		void updateTargets() override {};
		void compileTarget(std::string target) override {};
	};
}

#endif// ATTA_SCRIPT_SYSTEM_COMPILERS_NULL_COMPILER_H
