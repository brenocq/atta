//--------------------------------------------------
// Robot Simulator
// userInterface.cpp
// Date: 2020-07-10
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/graphics/gui/userInterface.h>
#include <atta/graphics/gui/guiState.h>
#include <atta/helpers/log.h>

namespace atta
{
	UserInterface::UserInterface(CreateInfo info):
		_device(info.device),
		_window(info.window),
		_scene(info.scene),
		_swapChain(info.swapChain),
		// Toggle variables
		_rootWidget(nullptr),
		_shouldClose(false)
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
		_fontLoader = std::make_shared<guib::FontLoader>(_device, _guiCommandPool, "/usr/include/atta/assets/fonts/Roboto/Roboto-Light.ttf");

		createTextures();
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
		// TODO release textures
	}

	void UserInterface::createWidgetTree()
	{
		_rootWidget = 
			new guib::TopBar(
			{
				.color = guib::state::palette["background"],
				.buttons = {
					new guib::Box((guib::BoxInfo){.color = guib::state::palette["background"], .size = {5, 1, guib::UNIT_PIXEL}}),
					new guib::MenuButton(
					(guib::MenuButtonInfo){
						.image = "logo16",
						.imageColor = guib::state::palette["main"],
						.color = guib::state::palette["background"],
						.hoverColor = guib::state::palette["light"],
						.clickColor = guib::state::palette["lightLight"],
						.children = {
							new guib::MenuItem((guib::MenuItemInfo){.text = "Welcome!"}),
							new guib::MenuItem((guib::MenuItemInfo){.text = "Atta main menu"}),
							new guib::MenuItem((guib::MenuItemInfo){.text = "Test"})
						}
					}),
					new guib::MenuButton(
					(guib::MenuButtonInfo){
						.text = "Rendering",
						.color = guib::state::palette["background"],
						.hoverColor = guib::state::palette["light"],
						.clickColor = guib::state::palette["lightLight"],
					}),
					new guib::MenuButton(
					(guib::MenuButtonInfo){
						.text = "Physics",
						.color = guib::state::palette["background"],
						.hoverColor = guib::state::palette["light"],
						.clickColor = guib::state::palette["lightLight"],
					}),
					new guib::MenuButton(
					(guib::MenuButtonInfo){
						.text = "Help",
						.color = guib::state::palette["background"],
						.hoverColor = guib::state::palette["light"],
						.clickColor = guib::state::palette["lightLight"],
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
					.child = new guib::Padding({
						.child = new guib::Text(
						{
							.color = {1,1,1,1},
							.text = object->getName(),
							.textSize = 10
						})
					})
				})
			);
		}

		//_windows.push_back(
		//	new guib::Window(
		//	{
		//		.name = "Scene",
		//		.closable = false,
		//		.minimizable = true,
		//		.movable = false,
		//		.offset = {0, 28, guib::UNIT_PIXEL, guib::UNIT_PIXEL},
		//		.size = {200, 1, guib::UNIT_PIXEL, guib::UNIT_PERCENT},
		//		.child = new guib::Column(
		//			{
		//				.hAlignment = guib::ALIGN_CENTER,
		//				.vAlignment = guib::ALIGN_START,
		//				.children = sceneTreeWidgets
		//			})
		//	})	
		//);

		//_windows.push_back(
		//	new guib::Window(
		//	{
		//		.name = "Draggable",
		//		.closable = true,
		//		.minimizable = true,
		//		.movable = true,
		//		.offset = {0.8, 28, guib::UNIT_PERCENT, guib::UNIT_PIXEL},
		//		.size = {.2, 200, guib::UNIT_PERCENT, guib::UNIT_PIXEL},
		//		/*.child = new guib::Box(
		//			{
		//				.color = {1,0,1,1},
		//				.size  = {1,.7}
		//			})*/
		//	})	
		//);

		_guiRender->setRootWidget(_rootWidget);
		_guiRender->setWindowWidgets(_windows);
	}

	void UserInterface::createTextures()
	{
		std::vector<std::pair<std::string, std::string>> logos = {
			{"logo16", "/usr/include/atta/assets/icons/logo16.png"},
			{"logo50", "/usr/include/atta/assets/icons/logo50.png"},
		};

		for(auto logo : logos)
		{
			guib::state::textureIndex[logo.first] = guib::state::textures.size();
			guib::state::textures.push_back(new atta::vk::Texture(_device, _guiCommandPool, logo.second));
		}
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
	//---------------- CALLBACKS ------------------//
	//---------------------------------------------//
	void UserInterface::onWindowResized(int width, int height)
	{
		_guiRender ->onWindowResized(width, height);
	}

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

		// Check if window closed by UI
		if(guib::state::shouldClose)
		{
			// This signal is propagated to userInterface->workerGui->threadManager
			// to finish execution
			_shouldClose = true;
		}
	}

	void UserInterface::onScroll(double xoffset, double yoffset)
	{
		_guiRender->onScroll(xoffset, yoffset);
	}
}
