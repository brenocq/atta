//--------------------------------------------------
// Atta Graphics Module
// vulkanAPI.h
// Date: 2021-08-30
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/graphics/apis/vulkan/vulkanAPI.h>

#include <atta/event/events/windowResize.h>
#include <atta/event/interface.h>
#include <atta/graphics/apis/vulkan/common.h>
#include <atta/graphics/apis/vulkan/shader.h>

namespace atta::graphics {

VulkanAPI::VulkanAPI(std::shared_ptr<Window> window) : GraphicsAPI(GraphicsAPI::VULKAN, window), _currFrame(0), _windowResized(false) {}

VulkanAPI::~VulkanAPI() {
    if (_instance)
        LOG_WARN("gfx::vk::VulkanAPI", "VulkanAPI should be [w]shutDown[] before being destroyed");
}

void VulkanAPI::startUp() {
    _instance = std::make_shared<vk::Instance>();
#ifdef ATTA_DEBUG_BUILD
    _debugMessenger = std::make_shared<vk::DebugMessenger>(_instance);
#endif
    _physicalDevice = std::make_shared<vk::PhysicalDevice>(_instance);
    _device = std::make_shared<vk::Device>(_physicalDevice);
    _commandPool = std::make_shared<vk::CommandPool>(_device);
    _commandBuffers = std::make_shared<vk::CommandBuffers>(_device, _commandPool, MAX_FRAMES_IN_FLIGHT);

    _imageAvailableSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
    _renderFinishedSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
    _inFlightFences.resize(MAX_FRAMES_IN_FLIGHT);
    for (int i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
        _imageAvailableSemaphores[i] = std::make_shared<vk::Semaphore>(_device);
        _renderFinishedSemaphores[i] = std::make_shared<vk::Semaphore>(_device);
        _inFlightFences[i] = std::make_shared<vk::Fence>(_device);
    }

    _swapChainInitialized = false;
    _surface = std::make_shared<vk::Surface>(_instance, _window);
    _swapChain = std::make_shared<vk::SwapChain>(_device, _surface);

    // Subscribe to window resize event
    evt::subscribe<evt::WindowResize>(BIND_EVENT_FUNC(VulkanAPI::onWindowResize));

    //----- Create vulkan objects -----//
    // clang-format off
    //static std::vector<float> vertices = {
    //    -0.5f, -0.5f, 1.0f, 0.0f, 0.0f,
    //    0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
    //    0.5f, 0.5f, 0.0f, 0.0f, 1.0f,
    //    -0.5f, 0.5f, 1.0f, 1.0f, 1.0f,
    //};
    //static std::vector<uint32_t> indices = {
    //    0, 1, 2, 2, 3, 0
    //};
    // clang-format on

    //----- Framebuffer -----//
    gfx::Framebuffer::CreateInfo framebufferInfo{};
    framebufferInfo.width = _swapChain->getImages()[0]->getWidth();
    framebufferInfo.height = _swapChain->getImages()[0]->getHeight();
    for (std::shared_ptr<Image> image : _swapChain->getImages()) {
        gfx::Framebuffer::Attachment attachment{};
        attachment.image = std::dynamic_pointer_cast<gfx::Image>(image);
        framebufferInfo.attachments = {attachment};
        _framebuffers.push_back(std::make_shared<vk::Framebuffer>(framebufferInfo));
    }

    //----- Render pass -----//
    gfx::RenderPass::CreateInfo renderPassInfo{};
    renderPassInfo.framebuffer = _framebuffers[0];
    _renderPass = std::make_shared<vk::RenderPass>(renderPassInfo);
    for (std::shared_ptr<vk::Framebuffer> fb : _framebuffers)
        fb->create(_renderPass);

    //----- Send vertices to GPU -----//
    // gfx::VertexBuffer::CreateInfo vertexBufferInfo{};
    // vertexBufferInfo.layout = {
    //    {"inPosition", VertexBufferElement::Type::VEC2},
    //    {"inColor", VertexBufferElement::Type::VEC3},
    //};
    // vertexBufferInfo.size = vertices.size() * sizeof(float);
    // vertexBufferInfo.data = (uint8_t*)vertices.data();
    //_vertexBuffer = std::make_shared<vk::VertexBuffer>(vertexBufferInfo);

    ////----- Send indices to GPU -----//
    // gfx::IndexBuffer::CreateInfo indexBufferInfo{};
    // indexBufferInfo.size = indices.size() * sizeof(uint32_t);
    // indexBufferInfo.data = (uint8_t*)indices.data();
    //_indexBuffer = std::make_shared<vk::IndexBuffer>(indexBufferInfo);

    //----- Pipeline -----//
    // gfx::ShaderGroup::CreateInfo shaderGroupInfo;
    // shaderGroupInfo.shaderPaths = {"shaders/fastRenderer/shader.vert", "shaders/fastRenderer/shader.frag"};
    // std::shared_ptr<vk::ShaderGroup> shaderGroup = std::make_shared<vk::ShaderGroup>(shaderGroupInfo);

    // gfx::Pipeline::CreateInfo pipelineInfo{};
    // pipelineInfo.shaderGroup = shaderGroup;
    // pipelineInfo.renderPass = _renderPass;
    // pipelineInfo.layout = vertexBufferInfo.layout;
    //_pipeline = std::make_shared<vk::Pipeline>(pipelineInfo);

    //----- UI Descriptor Pool -----//
    std::vector<VkDescriptorType> descriptorTypes = {
        VK_DESCRIPTOR_TYPE_SAMPLER,
        VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER,
        VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE,
        VK_DESCRIPTOR_TYPE_STORAGE_IMAGE,
        VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER,
        VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER,
        VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
        VK_DESCRIPTOR_TYPE_STORAGE_BUFFER,
        VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC,
        VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC,
        VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT,
    };
    std::vector<vk::DescriptorSetLayout::Binding> uiDescriptorBindings;
    for (VkDescriptorType type : descriptorTypes) {
        vk::DescriptorSetLayout::Binding b{};
        b.descriptorCount = 1;
        b.type = type;
        uiDescriptorBindings.push_back(b);
    }
    _uiDescriptorPool = std::make_shared<vk::DescriptorPool>(uiDescriptorBindings, 1000);

    LOG_SUCCESS("gfx::VulkanAPI", "Initialized");
}

void VulkanAPI::shutDown() {
    waitDevice();
    _framebuffers.clear();
    _swapChain.reset();

    //_pipeline.reset();
    _renderPass.reset();
    _vertexBuffer.reset();

    _inFlightFences.clear();
    _renderFinishedSemaphores.clear();
    _imageAvailableSemaphores.clear();

    _commandBuffers.reset();
    _commandPool.reset();
    _device.reset();
    _physicalDevice.reset();
    _debugMessenger.reset();

    _surface.reset();
    _instance.reset();
}

void VulkanAPI::waitDevice() { vkDeviceWaitIdle(_device->getHandle()); }

void VulkanAPI::beginFrame() {
    if (!_swapChainInitialized) {
        _swapChainInitialized = true;
    }

    //---------- CPU-GPU synchronization ----------//
    _inFlightFences[_currFrame]->wait();

    //---------- Get swap chain image ----------//
    VkResult result = vkAcquireNextImageKHR(_device->getHandle(), _swapChain->getHandle(), std::numeric_limits<uint64_t>::max(),
                                            _imageAvailableSemaphores[_currFrame]->getHandle(), VK_NULL_HANDLE, &_imageIndex);
    if (result == VK_ERROR_OUT_OF_DATE_KHR) {
        recreateSwapChain();
        return;
    } else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
        LOG_ERROR("gfx::VulkanAPI", "Failed to acquire swap chain image");
        return;
    }

    //---------- Reset fence ----------//
    // Only reset fence if work will be submitted
    _inFlightFences[_currFrame]->reset();

    //---------- Record to command buffer ----------//
    VkCommandBuffer cmdBuf = _commandBuffers->begin(_currFrame);
    _renderPass->setFramebuffer(_framebuffers[_imageIndex]);
    _renderPass->begin();
    //_pipeline->begin();
    //_vertexBuffer->bind();
    //_indexBuffer->bind();
    // vkCmdDrawIndexed(cmdBuf, _indexBuffer->getCount(), 1, 0, 0, 0);
    //_pipeline->end();
}

void VulkanAPI::endFrame() {
    _renderPass->end();
    _commandBuffers->end(_currFrame);
    VkCommandBuffer cmdBuf = _commandBuffers->getHandles()[_currFrame];

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
        LOG_ERROR("gfx::VulkanAPI", "Failed to submit draw command buffer!");

    //---------- Submit do present queue ----------//
    VkSwapchainKHR swapChains[] = {_swapChain->getHandle()};
    VkPresentInfoKHR presentInfo{};
    presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
    presentInfo.waitSemaphoreCount = 1;
    presentInfo.pWaitSemaphores = signalSemaphores;
    presentInfo.swapchainCount = 1;
    presentInfo.pSwapchains = swapChains;
    presentInfo.pImageIndices = &_imageIndex;

    VkResult result = vkQueuePresentKHR(_device->getPresentQueue(), &presentInfo);
    if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || _windowResized) {
        _windowResized = false;
        recreateSwapChain();
    } else if (result != VK_SUCCESS) {
        LOG_ERROR("gfx::VulkanAPI", "Failed to present swap chain image!");
        return;
    }

    //---------- Next frame ----------//
    _currFrame = (_currFrame + 1) % MAX_FRAMES_IN_FLIGHT;
}

void VulkanAPI::renderMesh(StringId meshSid) {}

void VulkanAPI::renderQuad() {}

void VulkanAPI::renderQuad3() {}

void VulkanAPI::renderCube() {}

void VulkanAPI::renderFramebufferToQuad(std::shared_ptr<Framebuffer> framebuffer) {}

void VulkanAPI::generateCubemap(StringId textureSid, mat4 rotationMatrix) {}

void VulkanAPI::generateProcessedCubemap(GenerateProcessedCubemapInfo gpcInfo) {}

void VulkanAPI::generateProcessedTexture(GenerateProcessedTextureInfo gptInfo) {}

void* VulkanAPI::getImGuiImage(StringId sid) const { return nullptr; }

std::shared_ptr<vk::Instance> VulkanAPI::getInstance() const { return _instance; }

std::shared_ptr<vk::PhysicalDevice> VulkanAPI::getPhysicalDevice() const { return _physicalDevice; }

std::shared_ptr<vk::Device> VulkanAPI::getDevice() const { return _device; }

std::shared_ptr<vk::CommandBuffers> VulkanAPI::getCommandBuffers() const { return _commandBuffers; }

std::shared_ptr<vk::CommandPool> VulkanAPI::getCommandPool() const { return _commandPool; }

std::shared_ptr<vk::RenderPass> VulkanAPI::getRenderPass() const { return _renderPass; }

std::shared_ptr<vk::DescriptorPool> VulkanAPI::getUiDescriptorPool() const { return _uiDescriptorPool; }

void VulkanAPI::recreateSwapChain() {
    while (_window->getWidth() == 0 && _window->getHeight() == 0) {
        // Wait until window is no longer minimized
        _window->update();
    }

    vkDeviceWaitIdle(_device->getHandle());
    _framebuffers.clear();
    _swapChain.reset();

    // Create swap chain
    _swapChain = std::make_shared<vk::SwapChain>(_device, _surface);

    // Create framebuffers
    gfx::Framebuffer::CreateInfo framebufferInfo{};
    framebufferInfo.width = _swapChain->getImages()[0]->getWidth();
    framebufferInfo.height = _swapChain->getImages()[0]->getHeight();
    for (std::shared_ptr<Image> image : _swapChain->getImages()) {
        gfx::Framebuffer::Attachment attachment{};
        attachment.image = std::dynamic_pointer_cast<gfx::Image>(image);
        framebufferInfo.attachments = {attachment};
        _framebuffers.push_back(std::make_shared<vk::Framebuffer>(framebufferInfo));
    }
    for (std::shared_ptr<vk::Framebuffer> fb : _framebuffers)
        fb->create(_renderPass);
}

void VulkanAPI::onWindowResize(evt::Event e) { _windowResized = true; }

} // namespace atta::graphics
