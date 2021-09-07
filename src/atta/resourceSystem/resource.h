//--------------------------------------------------
// Atta Resource System
// resource.h
// Date: 2021-09-07
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_RESOURCE_SYSTEM_RESOURCE_H
#define ATTA_RESOURCE_SYSTEM_RESOURCE_H
#include <atta/core/stringId.h>

namespace atta
{
	class Resource
	{
	public:
		Resource(StringId id);

		StringId getId() const { return _id; }
		uint8_t* getData() const { return _data; }

	protected:
		StringId _id;
		uint8_t* _data;
	};
}

#endif// ATTA_RESOURCE_SYSTEM_RESOURCE_H
