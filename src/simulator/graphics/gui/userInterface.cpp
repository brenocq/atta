//--------------------------------------------------
// Robot Simulator
// userInterface.cpp
// Date: 2020-07-10
// By Breno Cunha Queiroz
//--------------------------------------------------
#include "userInterface.h"
#include "simulator/helpers/log.h"

namespace atta
{
	UserInterface::UserInterface(
			CreateInfo info):
		_device(info.device),
		_window(info.window),
		// Toggle variables
		_rootWidget(nullptr)
	{
		createOutputImage();

		//---------- Create gui objects ----------//
		_guiCommandPool = std::make_shared<vk::CommandPool>(_device, VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT);
		_guiCommandBuffers = std::make_shared<vk::CommandBuffers>(_device, _guiCommandPool, 1);

		// Uniform buffer
		_guiUniformBuffer = std::make_shared<GuiUniformBuffer>(_device, sizeof(GuiUniformBufferObject));

		GuiUniformBufferObject ubo;
		ubo.ratio = _window->getWidth()/(float)_window->getHeight();
		ubo.debug = false;
		_guiUniformBuffer->setValue(ubo);

		// Load font to texture
		_fontLoader = std::make_shared<guib::FontLoader>(_device, _guiCommandPool, "assets/fonts/Ubuntu-Medium.ttf");


		_guiPipeline = std::make_shared<GuiPipeline>(_device, _window, _imageView, _guiUniformBuffer, _fontLoader);
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

	void UserInterface::createOutputImage()
	{
		_image = std::make_shared<vk::Image>(
			_device, 
			_window->getWidth(), _window->getHeight(), 
			VK_FORMAT_R32G32B32A32_SFLOAT, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT|VK_IMAGE_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
		_imageView = std::make_shared<vk::ImageView>(_device, _image->handle(), VK_FORMAT_R32G32B32A32_SFLOAT, VK_IMAGE_ASPECT_COLOR_BIT);
	}

	void UserInterface::createWidgetTree()
	{
		_rootWidget = 
			new guib::Box(
			{
				.color = {.2,.2,.2,1},
				.size  = {300, 1, guib::UNIT_PIXEL, guib::UNIT_PERCENT},
				.child = new guib::Align({
					.hAlignment = guib::ALIGN_CENTER,
					.vAlignment = guib::ALIGN_END,
					.child = new guib::Box({
						.color = {.8, .5, .3},
						.size = {1, .8},
						.child = new guib::Align({
							.hAlignment = guib::ALIGN_CENTER,
							.vAlignment = guib::ALIGN_CENTER,
							.child = new guib::Visibility({
								.visible = true,
								.child = new guib::Column(
								{
									.hAlignment = guib::ALIGN_CENTER,
									.vAlignment = guib::ALIGN_END,
									.children = {
										new guib::Box({
											.color = {.1,.8,.3,1},
											.size = {.3,.9},
										}),
										new guib::Box({
											.color = {.8,.1,.3,1},
											.size = {.99,.1},
										})
									}
								})
							})
						})
					})
				})
			}); 

		_windows.push_back(
			new guib::Window(
			{
				.name = "Main",
				.closable = false,
				.minimizable = true,
				.movable = false,
				.offset = {0, 0},
				.size = {200, 1, guib::UNIT_PIXEL, guib::UNIT_PERCENT},
				.child = new guib::Align(
					{
						.hAlignment = guib::ALIGN_CENTER,
						.vAlignment = guib::ALIGN_START,
						.child = new guib::Button(
						{
							.onClick = [&](){
							},
							.size = {100, 20, guib::UNIT_PIXEL, guib::UNIT_PIXEL},
							.child = new guib::Text(
							{
								.color = {1,1,1,1},
								.text = "abcdefghi",
								.textSize = 16
							})
						})
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
				.offset = {0.8, 0},
				.size = {.2, 200, guib::UNIT_PERCENT, guib::UNIT_PIXEL},
				/*.child = new guib::Box(
					{
						.color = {1,0,1,1},
						.size  = {1,.7}
					})*/
			})	
		);

		//_guiRender->setRootWidget(_rootWidget);
		_guiRender->setWindowWidgets(_windows);
	}

	void UserInterface::render()
	{
		VkCommandBuffer commandBuffer = _guiCommandBuffers->begin(0);
		{
			_guiPipeline->beginRender(commandBuffer);
			_guiRender->render(commandBuffer);
			_guiPipeline->endRender(commandBuffer);
		}
		_guiCommandBuffers->end(0);
	}

	void UserInterface::render(VkCommandBuffer commandBuffer)
	{
		_guiPipeline->beginRender(commandBuffer);
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
