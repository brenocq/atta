//--------------------------------------------------
// Atta Script System
// linuxLinker.h
// Date: 2021-09-06
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_SCRIPT_SYSTEM_LINKERS_LINUX_LINKER_H
#define ATTA_SCRIPT_SYSTEM_LINKERS_LINUX_LINKER_H
#ifdef ATTA_OS_LINUX
#include <atta/scriptSystem/linkers/linker.h>

namespace atta
{
	class LinuxLinker : public Linker
	{
	public:
		LinuxLinker() = default;
		~LinuxLinker() = default;

		void linkTarget(std::string target) override;
	};
}

#endif// ATTA_OS_LINUX
#endif// ATTA_SCRIPT_SYSTEM_LINKERS_LINUX_LINKER_H
