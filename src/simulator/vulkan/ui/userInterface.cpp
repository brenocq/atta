//--------------------------------------------------
// Robot Simulator
// userInterface.cpp
// Date: 10/07/2020
// By Breno Cunha Queiroz
//--------------------------------------------------
#include "userInterface.h"

UserInterface::UserInterface(Device* device, Window* window, SwapChain* swapChain, CommandPool* commandPool)
{
	//---------- Get main objects ----------//
	_device = device;
	_window = window;
	_swapChain = swapChain;
	_commandPool = commandPool;
	PhysicalDevice* physicalDevice = _device->getPhysicalDevice();
	Instance* instance = physicalDevice->getInstance();

	//---------- Create ui objects ----------//
	createDescriptorPool();
	_renderPass = new UiRenderPass(_device, _swapChain);

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

    if(!ImGui_ImplVulkan_Init(&initInfo, _renderPass->handle()))
	{
		std::cout << BOLDRED << "[UserInterface]" << RESET << RED << " Failed to initialise ImGui VULKAN adapter!" << RESET << std::endl;
		exit(1);
	}
	
	//---------- Upload fonts ----------//
	VkCommandBuffer command_buffer = _commandPool->beginSingleTimeCommands();

	if(!ImGui_ImplVulkan_CreateFontsTexture(command_buffer))
	{
		std::cout << BOLDRED << "[UserInterface]" << RESET << RED << " Failed to create fonts texture!" << RESET << std::endl;
		exit(1);
	}

	_commandPool->endSingleTimeCommands(command_buffer);

	//ImGui_ImplVulkan_DestroyFontUploadObjects();
}

UserInterface::~UserInterface()
{
	ImGui_ImplVulkan_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

	delete _imguiDescriptorPool;
	_imguiDescriptorPool = nullptr;

	delete _renderPass;
	_renderPass = nullptr;
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

void UserInterface::render(VkCommandBuffer commandBuffer, FrameBuffer* frameBuffer)
{
	ImGui_ImplVulkan_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
	draw();
	ImGui::Render();

	//---------- User interface render pass ----------//
	VkRenderPassBeginInfo uiRenderPassInfo = {};
	uiRenderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
	uiRenderPassInfo.renderPass = _renderPass->handle();
	uiRenderPassInfo.framebuffer = frameBuffer->handle();
	uiRenderPassInfo.renderArea.extent = _swapChain->getExtent();
	uiRenderPassInfo.clearValueCount = 1;
	VkClearValue clearValueUi{};
	clearValueUi.color = {0.0f, 0.0f, 0.0f, 1.0f};
	uiRenderPassInfo.pClearValues = &clearValueUi;
	vkCmdBeginRenderPass(commandBuffer, &uiRenderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
	{
		ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), commandBuffer);
	}
	vkCmdEndRenderPass(commandBuffer);
}
