//--------------------------------------------------
// Robot Simulator
// userInterface.cpp
// Date: 10/07/2020
// By Breno Cunha Queiroz
//--------------------------------------------------
#include "userInterface.h"

UserInterface::UserInterface(Device* device, Window* window, SwapChain* swapChain)
{
	//---------- Get main objects ----------//
	_device = device;
	_window = window;
	_swapChain = swapChain;
	PhysicalDevice* physicalDevice = _device->getPhysicalDevice();
	Instance* instance = physicalDevice->getInstance();

	//---------- Create ui objects ----------//
	createDescriptorPool();
	_imguiRenderPass = new UiRenderPass(_device, _swapChain);
	_imguiCommandPool = new CommandPool(_device, VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT);

	_imguiFrameBuffers.resize(_swapChain->getImageViews().size());
	for(size_t i = 0; i < _imguiFrameBuffers.size(); i++) 
		_imguiFrameBuffers[i] = new UiFrameBuffer(_swapChain->getImageViews()[i], _imguiRenderPass);
	createCommandBuffers();

	//---------- Init imgui ----------//
	IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui::StyleColorsDark();

	if(!ImGui_ImplGlfw_InitForVulkan(_window->handle(), true))
	{
		std::cout << BOLDRED << "[UserInterface]" << RESET << RED << " Failed to initialise ImGui GLFW adapter!" << RESET << std::endl;
		exit(1);
	}

    ImGui_ImplVulkan_InitInfo initInfo = {};
    initInfo.Instance = instance->handle();
    initInfo.PhysicalDevice = physicalDevice->handle();
    initInfo.Device = _device->handle();
    initInfo.QueueFamily = 0;// TODO not being used
    initInfo.Queue = _device->getGraphicsQueue();
    initInfo.PipelineCache = VK_NULL_HANDLE;
    initInfo.DescriptorPool = _imguiDescriptorPool->handle();
    initInfo.Allocator = nullptr;
    initInfo.MinImageCount = _swapChain->getImages().size();
    initInfo.ImageCount = _swapChain->getImages().size();
    initInfo.CheckVkResultFn = UserInterface::checkResult;

    if(!ImGui_ImplVulkan_Init(&initInfo, _imguiRenderPass->handle()))
	{
		std::cout << BOLDRED << "[UserInterface]" << RESET << RED << " Failed to initialise ImGui VULKAN adapter!" << RESET << std::endl;
		exit(1);
	}
	
	//---------- Upload fonts ----------//
	VkCommandBuffer command_buffer = _imguiCommandPool->beginSingleTimeCommands();
	if(!ImGui_ImplVulkan_CreateFontsTexture(command_buffer))
	{
		std::cout << BOLDRED << "[UserInterface]" << RESET << RED << " Failed to create fonts texture!" << RESET << std::endl;
		exit(1);
	}
	_imguiCommandPool->endSingleTimeCommands(command_buffer);

	//ImGui_ImplVulkan_DestroyFontUploadObjects();
	ImGui_ImplVulkan_SetMinImageCount(2);
}

UserInterface::~UserInterface()
{
	ImGui_ImplVulkan_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

	for (auto frameBuffer : _imguiFrameBuffers) 
	{
		delete frameBuffer;
		frameBuffer = nullptr;
    }

	vkFreeCommandBuffers(_device->handle(), _imguiCommandPool->handle(), static_cast<uint32_t>(_imguiCommandBuffers.size()), _imguiCommandBuffers.data());

	delete _imguiCommandPool;
	_imguiCommandPool = nullptr;

	delete _imguiRenderPass;
	_imguiRenderPass = nullptr;

	// Resources to destroy when the program ends
	ImGui_ImplVulkan_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	delete _imguiDescriptorPool;
	_imguiDescriptorPool = nullptr;
}

void UserInterface::createDescriptorPool()
{
	std::vector<VkDescriptorPoolSize> poolSizes =
        {
            { VK_DESCRIPTOR_TYPE_SAMPLER, 1000 },
            { VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1000 },
            { VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 1000 },
            { VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 1000 },
            { VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER, 1000 },
            { VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER, 1000 },
            { VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1000 },
            { VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1000 },
            { VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, 1000 },
            { VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC, 1000 },
            { VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, 1000 }
        };
	_imguiDescriptorPool = new DescriptorPool(_device, poolSizes);
}

void UserInterface::createCommandBuffers()
{
	// TODO put it in a beautiful way
	_imguiCommandBuffers.resize(_imguiFrameBuffers.size());

	VkCommandBufferAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocInfo.commandPool = _imguiCommandPool->handle();
	allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	allocInfo.commandBufferCount = (uint32_t) _imguiCommandBuffers.size();

	if(vkAllocateCommandBuffers(_device->handle(), &allocInfo, _imguiCommandBuffers.data()) != VK_SUCCESS)
	{
		std::cout << BOLDRED << "[UserInterface]" << RESET << RED << " Failed to allocate command buffers!" << RESET << std::endl;
		exit(1);
	}
}

void UserInterface::checkResult(VkResult result)
{
	if(result != VK_SUCCESS)
	{
		std::cout << BOLDRED << "[UserInterface]" << RESET << RED << " Failed to setup Imgui info!" << RESET << std::endl;
		exit(1);
	}
}

void UserInterface::draw()
{
	ImGui::ShowDemoWindow();
}

void UserInterface::render(int i)
{
	ImGui_ImplVulkan_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
	draw();
	ImGui::Render();

	//---------- Begin command buffer ----------//
	VkCommandBufferBeginInfo info = {};
    info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    info.flags |= VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
    vkBeginCommandBuffer(_imguiCommandBuffers[i], &info);

	//---------- User interface render pass ----------//
	VkRenderPassBeginInfo uiRenderPassInfo = {};
	uiRenderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
	uiRenderPassInfo.renderPass = _imguiRenderPass->handle();
	uiRenderPassInfo.framebuffer = _imguiFrameBuffers[i]->handle();
	uiRenderPassInfo.renderArea.extent = _swapChain->getExtent();
	uiRenderPassInfo.clearValueCount = 1;
	VkClearValue clearValueUi{};
	clearValueUi.color = {0.0f, 0.0f, 0.0f, 1.0f};
	uiRenderPassInfo.pClearValues = &clearValueUi;
	vkCmdBeginRenderPass(_imguiCommandBuffers[i], &uiRenderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
	{
		ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), _imguiCommandBuffers[i]);
	}
	vkCmdEndRenderPass(_imguiCommandBuffers[i]);
	vkEndCommandBuffer(_imguiCommandBuffers[i]);
}
