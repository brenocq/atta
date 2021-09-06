//--------------------------------------------------
// Atta Script System
// linker.h
// Date: 2021-09-06
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_SCRIPT_SYSTEM_LINKERS_LINKER_H
#define ATTA_SCRIPT_SYSTEM_LINKERS_LINKER_H

namespace atta
{
	class Linker
	{
	public:
		Linker() = default;
		virtual ~Linker() = default;

		virtual void linkTarget(std::string target) = 0;
	};
}

#endif// ATTA_SCRIPT_SYSTEM_LINKERS_LINKER_H
