//--------------------------------------------------
// Atta
// atta.h
// Date: 2021-08-17
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_ATTA_H
#define ATTA_ATTA_H
#include <atta/eventSystem/eventManager.h>
#include <atta/graphicsSystem/graphicsManager.h>
#include <atta/memorySystem/allocators/stackAllocator.h>

namespace atta
{
	class Atta
	{
	public:
		Atta();
		~Atta();

		void run();

	private:
		// Handle events
		void onEvent(Event& event);
		void onWindowClose(Event& event);

		// Manager to handle internal atta events
		std::shared_ptr<EventManager> _eventManager;

		// Graphics
		GraphicsManager* _graphicsManager;

		// Memory
		StackAllocator* _mainAllocator;

		// State
		bool _shouldFinish;
	};
}

#endif// ATTA_ATTA_H
