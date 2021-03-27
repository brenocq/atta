//--------------------------------------------------
// Robot Simulator
// userInterface.cpp
// Date: 2020-07-10
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/graphics/gui/userInterface.h>
#include <atta/helpers/log.h>

namespace atta
{
	UserInterface::UserInterface(
			CreateInfo info):
		_device(info.device),
		_window(info.window),
		_scene(info.scene),
		_swapChain(info.swapChain),
		// Toggle variables
		_rootWidget(nullptr)
	{
		//---------- Create gui objects ----------//
		_guiCommandPool = std::make_shared<vk::CommandPool>(_device, vk::CommandPool::DEVICE_QUEUE_FAMILY_GRAPHICS, vk::CommandPool::QUEUE_GUI, VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT);
		_guiCommandBuffers = std::make_shared<vk::CommandBuffers>(_device, _guiCommandPool, 1);

		// Uniform buffer
		_guiUniformBuffer = std::make_shared<GuiUniformBuffer>(_device, sizeof(GuiUniformBufferObject));

		GuiUniformBufferObject ubo;
		ubo.ratio = _window->getWidth()/(float)_window->getHeight();
		ubo.debug = false;
		_guiUniformBuffer->setValue(ubo);

		// Load font to texture
		_fontLoader = std::make_shared<guib::FontLoader>(_device, _guiCommandPool, "/usr/include/atta/assets/fonts/BFont/bfont.ttf");


		_guiPipeline = std::make_shared<GuiPipeline>(_device, _window, _swapChain, _guiUniformBuffer, _fontLoader);
		_guiRender = std::make_shared<guib::GuiRender>(
				(VkExtent2D){_window->getWidth(),_window->getHeight()}, 
				_guiPipeline->getPipelineLayout(), 
				_window->handle(), 
				_fontLoader);

		createWidgetTree();
	}

	UserInterface::~UserInterface()
	{
	}

	void UserInterface::createWidgetTree()
	{
		_rootWidget = 
			new guib::TopBar(
			{
				.color = {.2,.2,.2,1},
				.buttons = {
					new guib::MenuButton(
					(guib::MenuButtonInfo){
						.name = "Config"
					})
				}
			}); 

		std::vector<guib::Widget*> sceneTreeWidgets;
		for(auto object : _scene->getObjects())
		{
			sceneTreeWidgets.push_back(
				new guib::Button(
				{
					.onClick = [&](){
					},
					.size = {1, 20, guib::UNIT_PERCENT, guib::UNIT_PIXEL},
					.child = new guib::Text(
					{
						.color = {1,1,1,1},
						.text = object->getName(),
						.textSize = 16
					})
				})
			);
		}

		_windows.push_back(
			new guib::Window(
			{
				.name = "Scene",
				.closable = false,
				.minimizable = true,
				.movable = false,
				.offset = {0, 20, guib::UNIT_PIXEL, guib::UNIT_PIXEL},
				.size = {200, 1, guib::UNIT_PIXEL, guib::UNIT_PERCENT},
				.child = new guib::Column(
					{
						.hAlignment = guib::ALIGN_CENTER,
						.vAlignment = guib::ALIGN_START,
						.children = sceneTreeWidgets
					})
			})	
		);

		_windows.push_back(
			new guib::Window(
			{
				.name = "Draggable",
				.closable = true,
				.minimizable = true,
				.movable = true,
				.offset = {0.8, 20, guib::UNIT_PERCENT, guib::UNIT_PIXEL},
				.size = {.2, 200, guib::UNIT_PERCENT, guib::UNIT_PIXEL},
				/*.child = new guib::Box(
					{
						.color = {1,0,1,1},
						.size  = {1,.7}
					})*/
			})	
		);

		_guiRender->setRootWidget(_rootWidget);
		_guiRender->setWindowWidgets(_windows);
	}

	void UserInterface::render(int imageIndex)
	{
		VkCommandBuffer commandBuffer = _guiCommandBuffers->begin(0);
		{
			_guiPipeline->beginRender(commandBuffer, imageIndex);
			_guiRender->render(commandBuffer);
			_guiPipeline->endRender(commandBuffer);
		}
		_guiCommandBuffers->end(0);
	}

	void UserInterface::render(VkCommandBuffer commandBuffer, int imageIndex)
	{
		_guiPipeline->beginRender(commandBuffer, imageIndex);
		_guiRender->render(commandBuffer);
		_guiPipeline->endRender(commandBuffer);
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
}
