//--------------------------------------------------
// Atta Service Locator
// serviceLocator.h
// Date: 2021-08-19
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_SERVICE_LOCATOR_SERVICE_LOCATOR_H
#define ATTA_SERVICE_LOCATOR_SERVICE_LOCATOR_H
#include <atta/serviceLocator/service.h>

namespace atta
{
	class ServiceLocator final
	{
	public:
		template<typename T>
		void provide(T* service);

		template<typename T>
		const T** locate();

	private:
		std::unordered_map<int, void*> _services;
	};

	class GlobalServiceLocator
	{
		public:
			template<typename T>
			static void provide(T* service) { _serviceLocator.provide<T>(service); }

			template<typename T>
			static const T** locate() { return _serviceLocator.locate<T>(); }

		private:
			static ServiceLocator _serviceLocator;
	};
}

#include <atta/serviceLocator/serviceLocator.inl>
#endif// ATTA_SERVICE_LOCATOR_SERVICE_LOCATOR_H
