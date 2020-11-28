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
	createWidgetTree();
}

UserInterface::~UserInterface()
{
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
	_widgetTree = new guib::Box(
		{
			.color = {1,0,0,1},
			.size  = {.5,.5},
			.child = new guib::Box(
			{
				.color = {0,1,0,1},
				.size  = {.5,1},
				.child = new guib::Box(
				{
					.color = {0,0,1,1},
					.size  = {.5,.9},
					.child = (guib::Widget*) new guib::Column(
					{
						.children = {
							new guib::Padding(
							{
								.padding = guib::PaddingValues::all(.05),
								.child = new guib::Box(
									{
										.color = {0,1,0,1},
										.size  = {1,.2}
									})
							}),
							new guib::Padding(
							{
								.padding = guib::PaddingValues::symmetric(.4, .05),
								.child = new guib::Box(
									{
										.color = {1,0,0,1},
										.size  = {1,.2}
									})
							}),
							new guib::Padding(
							{
								.padding = {.5, .5, .1, .1},
								.child = new guib::Box(
									{
										.color = {0,1,1,1},
										.size  = {1,.2}
									})
							}),
							new guib::Box(
							{
								.color = {1,0,0,1},
								.size  = {.9,.4},
								.child = new guib::Row(
									{
										.children = {
											new guib::Box(
											{
												.color = {1,1,1,1},
												.size  = {.3,.9}
											}),
											new guib::Box(
											{
												.color = {0,0,0,1},
												.size  = {.3,.9}
											})
										}
									})
							})
						}
					})
				})
			})
		}); 
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
		_guiPipeline->render(commandBuffer, _widgetTree, i);
		_guiPipeline->endRender(commandBuffer);
	}
	_guiCommandBuffers->end(i);
}

//---------------------------------------------//
//-------------- DRAW FUNCTIONS ---------------//
//---------------------------------------------//

void UserInterface::draw()
{

}

