//--------------------------------------------------
// Atta Script System
// compiler.h
// Date: 2021-09-05
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_SCRIPT_SYSTEM_COMPILERS_COMPILER_H
#define ATTA_SCRIPT_SYSTEM_COMPILERS_COMPILER_H

namespace atta
{
	class Compiler
	{
	public:
		Compiler() = default;
		virtual ~Compiler() = default;

		virtual void updateTargets() = 0;
		virtual void compileTarget(std::string target) = 0;
		std::vector<std::string> getTargets() const;
		std::map<std::string, std::vector<fs::path>> getTargetFiles() const { return _targetFiles; };

	protected:
		std::map<std::string, std::vector<fs::path>> _targetFiles;
	};
}

#endif// ATTA_SCRIPT_SYSTEM_COMPILERS_COMPILER_H
