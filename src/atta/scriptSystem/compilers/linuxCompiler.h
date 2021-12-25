//--------------------------------------------------
// Atta Script System
// linuxCompiler.h
// Date: 2021-09-05
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_SCRIPT_SYSTEM_COMPILERS_LINUX_COMPILER_H
#define ATTA_SCRIPT_SYSTEM_COMPILERS_LINUX_COMPILER_H
#ifdef ATTA_OS_LINUX
#include <atta/scriptSystem/compilers/compiler.h>

namespace atta
{
	class LinuxCompiler : public Compiler
	{
	public:
		LinuxCompiler();
		~LinuxCompiler();

		void compileAll() override;
		void compileTarget(StringId target) override;
		void updateTargets() override;

	private:
		void findTargetFiles(StringId target);
        std::string runCommand(std::string cmd);

        std::string _compiler;
	};
}

#endif// ATTA_OS_LINUX
#endif// ATTA_SCRIPT_SYSTEM_COMPILERS_LINUX_COMPILER_H
