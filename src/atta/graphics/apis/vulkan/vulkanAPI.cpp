//--------------------------------------------------
// Atta Graphics Module
// vulkanAPI.h
// Date: 2021-08-30
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/graphics/apis/vulkan/vulkanAPI.h>

#include <atta/graphics/apis/vulkan/common.h>
#include <atta/graphics/apis/vulkan/shaderGroup.h>

namespace atta::graphics {

VulkanAPI::VulkanAPI(std::shared_ptr<Window> window) : GraphicsAPI(GraphicsAPI::VULKAN, window), _currFrame(0) {
    _instance = std::make_shared<vk::Instance>();
#ifdef ATTA_DEBUG_BUILD
    _debugMessenger = std::make_shared<vk::DebugMessenger>(_instance);
#endif
    _physicalDevice = std::make_shared<vk::PhysicalDevice>(_instance);
    _device = std::make_shared<vk::Device>(_physicalDevice);
    _commandPool = std::make_shared<vk::CommandPool>(_device);
    _commandBuffers = std::make_shared<vk::CommandBuffers>(_device, _commandPool, MAX_FRAMES_IN_FLIGHT);
    _swapChainInitialized = false;

    _imageAvailableSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
    _renderFinishedSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
    _inFlightFences.resize(MAX_FRAMES_IN_FLIGHT);
    for (int i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
        _imageAvailableSemaphores[i] = std::make_shared<vk::Semaphore>(_device);
        _renderFinishedSemaphores[i] = std::make_shared<vk::Semaphore>(_device);
        _inFlightFences[i] = std::make_shared<vk::Fence>(_device);
    }

    LOG_SUCCESS("gfx::VulkanAPI", "Initialized");
}

VulkanAPI::~VulkanAPI() {
    vkDeviceWaitIdle(_device->getHandle());

    _inFlightFences.clear();
    _renderFinishedSemaphores.clear();
    _imageAvailableSemaphores.clear();

    _pipeline.reset();
    _renderPass.reset();
    _framebuffers.clear();
    _swapChain.reset();
    _surface.reset();

    _commandBuffers.reset();
    _commandPool.reset();
    _device.reset();
    _physicalDevice.reset();
    _debugMessenger.reset();
    _instance.reset();
}

void VulkanAPI::beginFrame() {
    if (!_swapChainInitialized) {
        _swapChainInitialized = true;
        _surface = std::make_shared<vk::Surface>(_instance, _window);
        _swapChain = std::make_shared<vk::SwapChain>(_device, _surface);

        gfx::Framebuffer::CreateInfo framebufferInfo{};
        framebufferInfo.width = _window->getWidth();
        framebufferInfo.height = _window->getHeight();
        for (std::shared_ptr<Image> image : _swapChain->getImages()) {
            gfx::Framebuffer::Attachment attachment{};
            attachment.image = std::dynamic_pointer_cast<gfx::Image>(image);
            framebufferInfo.attachments = {attachment};
            _framebuffers.push_back(std::make_shared<vk::Framebuffer>(framebufferInfo));
        }
        gfx::RenderPass::CreateInfo renderPassInfo{};
        renderPassInfo.framebuffer = _framebuffers[0];
        _renderPass = std::make_shared<vk::RenderPass>(renderPassInfo);

        for (std::shared_ptr<vk::Framebuffer> fb : _framebuffers)
            fb->create(_renderPass);

        gfx::ShaderGroup::CreateInfo info;
        info.shaderPaths = {"shaders/triangle/shader-spv.vert", "shaders/triangle/shader-spv.frag"};
        std::shared_ptr<vk::ShaderGroup> shaderGroup = std::make_shared<vk::ShaderGroup>(info);

        gfx::Pipeline::CreateInfo pipelineInfo{};
        pipelineInfo.shaderGroup = shaderGroup;
        pipelineInfo.renderPass = _renderPass;
        _pipeline = std::make_shared<vk::Pipeline>(pipelineInfo);
    }

    //---------- CPU-GPU synchronization ----------//
    _inFlightFences[_currFrame]->wait();
    _inFlightFences[_currFrame]->reset();

    //---------- Get swap chain image ----------//
    uint32_t imageIndex;
    VkResult result = vkAcquireNextImageKHR(_device->getHandle(), _swapChain->getHandle(), std::numeric_limits<uint64_t>::max(),
                                            _imageAvailableSemaphores[_currFrame]->getHandle(), VK_NULL_HANDLE, &imageIndex);
    if (result == VK_ERROR_OUT_OF_DATE_KHR) {
        // Recreate the swapchain (window resized)
        LOG_WARN("gfx::VulkanAPI", "Swap chain out of date! Must reset the swapChain");
        return;
    } else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
        LOG_ERROR("gfx::VulkanAPI", "Failed to acquire swap chain image");
        return;
    }

    //---------- Record to command buffer ----------//
    VkCommandBuffer cmdBuf = _commandBuffers->begin(_currFrame);
    _renderPass->setFramebuffer(_framebuffers[imageIndex]);
    _renderPass->begin();
    _pipeline->begin();
    vkCmdDraw(cmdBuf, 3, 1, 0, 0);
    _pipeline->end();
    _renderPass->end();
    _commandBuffers->end(_currFrame);

    //---------- GPU-GPU synchronization ----------//
    VkSemaphore waitSemaphores[] = {_imageAvailableSemaphores[_currFrame]->getHandle()};
    VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
    VkSemaphore signalSemaphores[] = {_renderFinishedSemaphores[_currFrame]->getHandle()};

    //---------- Submit to graphics queue ----------//
    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.waitSemaphoreCount = 1;
    submitInfo.pWaitSemaphores = waitSemaphores;
    submitInfo.pWaitDstStageMask = waitStages;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &cmdBuf;
    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores = signalSemaphores;
    if (vkQueueSubmit(_device->getGraphicsQueue(), 1, &submitInfo, _inFlightFences[_currFrame]->getHandle()) != VK_SUCCESS)
        LOG_ERROR("gfx::vulkanAPI", "Failed to submit draw command buffer!");

    //---------- Submit do present queue ----------//
    VkSwapchainKHR swapChains[] = {_swapChain->getHandle()};
    VkPresentInfoKHR presentInfo{};
    presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
    presentInfo.waitSemaphoreCount = 1;
    presentInfo.pWaitSemaphores = signalSemaphores;
    presentInfo.swapchainCount = 1;
    presentInfo.pSwapchains = swapChains;
    presentInfo.pImageIndices = &imageIndex;

    result = vkQueuePresentKHR(_device->getPresentQueue(), &presentInfo);
    if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR) {
        LOG_WARN("gfx::VulkanAPI", "Framebuffer resized! Must reset the swapChain!");
    } else if (result != VK_SUCCESS) {
        LOG_ERROR("gfx::VulkanAPI", "Failed to present swap chain image!");
        return;
    }

    //---------- Next frame ----------//
    _currFrame = (_currFrame + 1) % MAX_FRAMES_IN_FLIGHT;
}

void VulkanAPI::endFrame() {}

void VulkanAPI::renderMesh(StringId meshSid) {}

void VulkanAPI::renderQuad() {}

void VulkanAPI::renderQuad3() {}

void VulkanAPI::renderCube() {}

void VulkanAPI::renderFramebufferToQuad(std::shared_ptr<Framebuffer> framebuffer) {}

void VulkanAPI::generateCubemap(StringId textureSid, mat4 rotationMatrix) {}

void VulkanAPI::generateProcessedCubemap(GenerateProcessedCubemapInfo gpcInfo) {}

void VulkanAPI::generateProcessedTexture(GenerateProcessedTextureInfo gptInfo) {}

void* VulkanAPI::getImGuiImage(StringId sid) const { return nullptr; }

std::shared_ptr<vk::Device> VulkanAPI::getDevice() const { return _device; }
std::shared_ptr<vk::CommandBuffers> VulkanAPI::getCommandBuffers() const { return _commandBuffers; }

} // namespace atta::graphics
