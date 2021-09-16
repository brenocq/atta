//--------------------------------------------------
// Atta Resource System
// resourceManager.inl
// Date: 2021-09-07
// By Breno Cunha Queiroz
//--------------------------------------------------
namespace atta
{
	template <typename R>
	R* ResourceManager::getImpl(const fs::path& filename)
	{
		StringId sid = StringId(filename.string());
		if(_resourceMap.find(sid.getId()) == _resourceMap.end())
		{
			// Load resource if not loaded yet
			R* resource = new R(filename);
			if(resource == nullptr)
			{
				LOG_ERROR("ResourceManager", "Could not load resource [*w]$0[] ([w]$1[]). Probably out of memory", 
						sid, typeid(R).name());
				return nullptr;
			}
			_resourceMap[sid.getId()] = reinterpret_cast<uint8_t*>(resource);
			_resourcesByType[typeid(R).hash_code()].push_back(sid);

			// Create resource load event
			createLoadEvent<R>(resource, sid);

			return resource;
		}
		else
		{
			// Return loaded resource
			return reinterpret_cast<R*>(_resourceMap[sid.getId()]);
		}
	}

	template <typename R>
	void ResourceManager::createLoadEvent(R* resource, StringId sid)
	{
		LOG_ERROR("ResourceManager", "Could create load event for resource [*w]$0[] ([w]$1[]). It is a resource?", sid, typeid(R).name());
	}

	template <> void ResourceManager::createLoadEvent(Mesh* resource, StringId sid);

	template <typename R>
	std::vector<StringId> ResourceManager::getResourcesImpl()
	{
		return _resourcesByType[typeid(R).hash_code()];
	}
}
