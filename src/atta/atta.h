//--------------------------------------------------
// Atta
// atta.h
// Date: 2021-08-17
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_ATTA_H
#define ATTA_ATTA_H
#include <atta/eventSystem/eventDispatcher.h>
#include <atta/core/window.h>

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

		// Dispatcher to handle internal atta events
		std::shared_ptr<EventDispatcher> _eventDispatcher;

		std::unique_ptr<Window> _window;

		bool _shouldFinish;
	};
}

#endif// ATTA_ATTA_H
