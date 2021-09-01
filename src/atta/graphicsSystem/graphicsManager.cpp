//--------------------------------------------------
// Atta Graphics System
// graphicsManager.cpp
// Date: 2021-08-23
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/graphicsSystem/graphicsManager.h>
#include <atta/memorySystem/memoryManager.h>
#include <atta/memorySystem/allocators/stackAllocator.h>
#include <atta/graphicsSystem/layers/internal/uiLayer.h>
#include <atta/graphicsSystem/layers/internal/layer2D.h>

namespace atta
{
	GraphicsManager::GraphicsManager(std::shared_ptr<EventManager> eventManager):
		_eventManager(eventManager)
	{
		//----- System Memory -----//
		// Get main memory
		StackAllocator* main = MemoryManager::getAllocator<StackAllocator>(SID("Main"));
		size_t size = 32*1024*1024;// 32mb
		// Alloc memory inside main memory
		uint8_t* graphicsMemory = main->alloc<uint8_t>(size);
		// Create new allocator with graphics memory
		StackAllocator* graphics = new StackAllocator(graphicsMemory, size);
		MemoryManager::registerAllocator(SID("Graphics"), static_cast<Allocator*>(graphics));

		//----- Window -----//
		Window::CreateInfo windowInfo = {};
		windowInfo.eventManager = _eventManager;
		_window = std::make_shared<Window>(windowInfo);

		//----- Render API -----//
		_rendererAPI = std::make_shared<OpenGLRenderer>(_window);

		//----- Create Layers -----//
		_layerStack = std::make_unique<LayerStack>();
		_layerStack->push(new Layer2D());
		_layerStack->push(new UILayer());
	}

	GraphicsManager::~GraphicsManager()
	{
	}

	void GraphicsManager::update()
	{
		_window->update();

		_rendererAPI->beginFrame();
		_layerStack->render();
		_rendererAPI->endFrame();

		_window->swapBuffers();
	}
}
