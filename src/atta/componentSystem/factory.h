//--------------------------------------------------
// Atta Component System
// factory.h
// Date: 2021-09-17
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_COMPONENT_SYSTEM_FACTORY_H
#define ATTA_COMPONENT_SYSTEM_FACTORY_H
#include <atta/componentSystem/componentManager.h>

namespace atta
{
	class Factory
	{
		public:
			// The entity must be a prototype entity (has prototype component)
			struct CreateInfo
			{
				EntityId prototypeId;
				uint64_t maxClones = 0;
				std::vector<std::pair<size_t, uint8_t*>> componentMemories;
			};
			Factory(CreateInfo info);

			template <typename T>
			T* getComponent(uint64_t cloneId = 0);
			void runScripts();

			EntityId getPrototypeId() const { return _prototypeId; }
			uint64_t getMaxClones() const { return _maxClones; }
			std::vector<std::pair<size_t, uint8_t*>> getComponentMemories() const { return _componentMemories; }
			std::vector<size_t> getComponents() const;

		private:
			EntityId _prototypeId;
			uint64_t _maxClones;

			// Map the hash of each type to the memory start
			// Using vector assuming just a few of components
			std::vector<std::pair<size_t, uint8_t*>> _componentMemories;
	};
}

#include <atta/componentSystem/factory.inl>
#endif// ATTA_COMPONENT_SYSTEM_ENTITY_H
