//--------------------------------------------------
// Atta Service Locator
// service.h
// Date: 2021-08-19
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_SERVICE_LOCATOR_SERVICE_H
#define ATTA_SERVICE_LOCATOR_SERVICE_H
#include <atta/core/stringId.h>

namespace atta
{
	template <typename T>
	class Service
	{
	public:
		Service() = default;
		virtual ~Service() = default;

		static constexpr int getHash() { return typeid(T).hash_code(); }

		// TODO Rethink how to set the null service (default service)
		static T* nullService;
	};
}

#endif// ATTA_SERVICE_LOCATOR_SERVICE_H
