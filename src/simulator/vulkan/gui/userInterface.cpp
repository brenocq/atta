//--------------------------------------------------
// Robot Simulator
// userInterface.cpp
// Date: 2020-07-10
// By Breno Cunha Queiroz
//--------------------------------------------------
#include "userInterface.h"
#include "simulator/helpers/log.h"

UserInterface::UserInterface(Device* device, Window* window, SwapChain* swapChain, Scene* scene):
	// Toggle variables
   	_enableRayTracing(nullptr), _splitRender(nullptr),
	_showPhysicsDebugger(false)
{
	//---------- Get main objects ----------//
	_device = device;
	_window = window;
	_swapChain = swapChain;
	_scene = scene;
	PhysicalDevice* physicalDevice = _device->getPhysicalDevice();
	Instance* instance = physicalDevice->getInstance();

	//---------- Create gui objects ----------//
	_guiCommandPool = new CommandPool(_device, VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT);
	_guiCommandBuffers = new CommandBuffers(_device, _guiCommandPool, _swapChain->getImageViews().size());

	_guiUniformBuffers.resize(_swapChain->getImageViews().size());
	for(size_t i = 0; i < _guiUniformBuffers.size(); i++) 
	{
		_guiUniformBuffers[i] = new GuiUniformBuffer(_device, sizeof(GuiUniformBufferObject));

		VkExtent2D extent = _swapChain->getExtent();
		GuiUniformBufferObject ubo;
		ubo.ratio = extent.width/extent.height;
		ubo.debug = false;
		_guiUniformBuffers[i]->setValue(ubo);
	}

	_guiPipeline = new GuiPipeline(_device, _swapChain, _guiUniformBuffers);
	_guiRender = new GuiRender(_swapChain->getExtent(), _guiPipeline->getPipelineLayout(), _window->handle());
	createWidgetTree();
}

UserInterface::~UserInterface()
{
	delete _guiRender;
	_guiRender = nullptr;

	delete _guiPipeline;
	_guiPipeline = nullptr;

	for(auto& uniformBuffer : _guiUniformBuffers)
	{
		delete uniformBuffer;
		uniformBuffer = nullptr;
	}

	delete _guiCommandBuffers;
	_guiCommandBuffers = nullptr;

	delete _guiCommandPool;
	_guiCommandPool = nullptr;

	delete _widgetTree;
	_widgetTree = nullptr;
}

void UserInterface::createWidgetTree()
{
	//_widgetTree = new guib::Box(
	//	{
	//		.color = {.2,.2,.2,1},
	//		.size  = {300, 1, guib::UNIT_PIXEL, guib::UNIT_PERCENT},
	//		.child = new guib::Column(
	//		{
	//			.hAlignment = guib::ALIGN_CENTER,
	//			.vAlignment = guib::ALIGN_START,
	//			.children = {
	//				new guib::Box(
	//				{
	//					.color = {.15,.15,.15,1},
	//					.size  = {1,20, guib::UNIT_PERCENT, guib::UNIT_PIXEL},
	//					.child = new guib::Row(
	//						{
	//							.hAlignment = guib::ALIGN_END,
	//							.vAlignment = guib::ALIGN_CENTER,
	//							.children = {
	//								new guib::ClickDetector(
	//								{
	//									.onClick = [&](){
	//										Log::debug("Click", "Minimize!");
	//									},
	//									.child = new guib::Box(
	//									{
	//										.color = {.8,.8,.3,1},
	//										.radius = {.5, .5, .5, .5},
	//										.size  = {10,10, guib::UNIT_PIXEL, guib::UNIT_PIXEL}
	//									}),
	//								}),
	//								new guib::Box(
	//								{
	//									.color = {0,0,0,0},
	//									.size  = {4,1, guib::UNIT_PIXEL}
	//								}),
	//								new guib::ClickDetector(
	//								{
	//									.onClick = [&](){
	//										Log::debug("Click", "Close!");
	//									},
	//									.child = new guib::Box(
	//									{
	//										.color = {.8,.3,.3,1},
	//										.radius = {.5, .5, .5, .5},
	//										.size  = {10,10, guib::UNIT_PIXEL, guib::UNIT_PIXEL}
	//									}),
	//								}),
	//								new guib::Box(
	//								{
	//									.color = {0,0,0,0},
	//									.size  = {4,1, guib::UNIT_PIXEL}
	//								})
	//							}
	//						})
	//				}),
	//				new guib::Box(
	//				{
	//					.color = {1,.15,.15,1},
	//					.size  = {0.5,30, guib::UNIT_PERCENT, guib::UNIT_PIXEL}
	//				})
	//			}
	//		})
	//	}); 

	_windows.push_back(
		new guib::Window(
		{
			.name = "Main",
			.closable = false,
			.minimizable = true,
			.movable = false,
			.offset = {0, 0},
			.size = {200, 1, guib::UNIT_PIXEL, guib::UNIT_PERCENT},
			/*.child = new guib::Box(
				{
					.color = {1,0,1,1},
					.size  = {1,.7}
				})*/
		})	
	);

	_windows.push_back(
		new guib::Window(
		{
			.name = "Draggable",
			.closable = true,
			.minimizable = true,
			.movable = true,
			.offset = {0.8, 0},
			.size = {.2, 200, guib::UNIT_PERCENT, guib::UNIT_PIXEL},
			/*.child = new guib::Box(
				{
					.color = {1,0,1,1},
					.size  = {1,.7}
				})*/
		})	
	);
}

void UserInterface::checkResult(VkResult result)
{
	//if(result != VK_SUCCESS)
	//{
	//	Log::error("UserInterface", "Failed to setup Imggui info!");
	//	exit(1);
	//}
}


void UserInterface::render(int i)
{
	VkCommandBuffer commandBuffer = _guiCommandBuffers->begin(i);
	{
		_guiPipeline->beginRender(commandBuffer, i);
		_guiRender->render(commandBuffer, _widgetTree, _windows);
		_guiPipeline->endRender(commandBuffer);
	}
	_guiCommandBuffers->end(i);
}

void UserInterface::draw()
{

}


//---------------------------------------------//
//-------------- DRAW FUNCTIONS ---------------//
//---------------------------------------------//

// Window callbacks
void UserInterface::onKey(int key, int scancode, int action, int mods)
{
	_guiRender->onKey(key, scancode, action, mods);
}

void UserInterface::onCursorPosition(double xpos, double ypos)
{
	_guiRender->onCursorPosition(xpos, ypos);
}

void UserInterface::onMouseButton(int button, int action, int mods)
{
	_guiRender->onMouseButton(button, action, mods);
}

void UserInterface::onScroll(double xoffset, double yoffset)
{
	_guiRender->onScroll(xoffset, yoffset);
}
