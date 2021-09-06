//--------------------------------------------------
// Atta
// atta.h
// Date: 2021-08-17
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_ATTA_H
#define ATTA_ATTA_H
#include <atta/graphicsSystem/graphicsManager.h>
#include <atta/memorySystem/allocators/stackAllocator.h>

namespace atta
{
	class Atta
	{
	public:
		struct CreateInfo
		{
			std::filesystem::path projectFile = "";
		};

		Atta(CreateInfo info);
		~Atta();

		void run();

	private:
		// Handle events
		void onWindowClose(Event& event);

		// Graphics
		GraphicsManager* _graphicsManager;

		// Memory
		StackAllocator* _mainAllocator;

		// State
		bool _shouldFinish;
	};
}

#endif// ATTA_ATTA_H
