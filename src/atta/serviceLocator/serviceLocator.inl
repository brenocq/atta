//--------------------------------------------------
// Atta Service Locator
// serviceLocator.inl
// Date: 2021-08-19
// By Breno Cunha Queiroz
//--------------------------------------------------
namespace atta
{
	template <typename T>
	T* Service<T>::nullService = nullptr;

	template<typename T>
	void ServiceLocator::provide(T* service)
	{
		_services[T::getHash()] = service;
	}

	template<typename T>
	const T** ServiceLocator::locate()
	{
		int hash = typeid(T).hash_code();
		auto service = _services.find(hash);
		if(service != _services.end())
		{
			// Provide stored service
			return (const T**)(&service->second);
		}
		else
		{
			// Register null service and provide it
			DASSERT(Service<T>::nullService != nullptr, "Every service must have a nullService defined");
			_services[hash] = Service<T>::nullService;
			return (const T**)(&_services[hash]);
		}
	}
}
