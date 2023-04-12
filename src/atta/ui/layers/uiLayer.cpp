//--------------------------------------------------
// Atta UI Module
// uiLayer.cpp
// Date: 2021-09-01
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/ui/layers/uiLayer.h>
// ImGui backends
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#include <GLFW/glfw3.h>
#include <ImGuizmo.h>
#include <atta/graphics/apis/vulkan/vulkanAPI.h>
#include <atta/graphics/interface.h>
#include <backends/imgui_impl_vulkan.h>
#include <implot.h>

namespace atta::ui {

UILayer::UILayer() : graphics::Layer(StringId("GraphicsLayerUI")) {}

void UILayer::onAttach() {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImPlot::CreateContext();

    ImGuiIO& io = ImGui::GetIO();
    (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;     // Enable Docking

    // Don't save imgui.ini
    io.IniFilename = NULL;
#ifdef ATTA_OS_WEB
    // Disable file-system access when building with Emscripten (no fopen() of the imgui.init file)
    io.IniFilename = NULL;
#else
    // Multiple viewports not supported for the web
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable; // Enable Multi-Viewport
#endif

    // initOpenGL();
    initVulkan();
    setTheme();
}

void UILayer::onDetach() {
    // ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplVulkan_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void UILayer::onRender() {}

void UILayer::onUIRender() {}

void UILayer::begin() {
    // ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplVulkan_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    ImGuizmo::BeginFrame();
}

void UILayer::end() {
    ImGui::Render();
    // ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(),
                                    std::dynamic_pointer_cast<gfx::VulkanAPI>(gfx::getGraphicsAPI())->getCommandBuffers()->getCurrent());

    ImGuiIO& io = ImGui::GetIO();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
        GLFWwindow* backupCurrentContext = glfwGetCurrentContext();
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
        glfwMakeContextCurrent(backupCurrentContext);
    }
}

void UILayer::initOpenGL() {
    GLFWwindow* window = (GLFWwindow*)gfx::getWindow()->getHandle();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 100");
}

void UILayer::initVulkan() {
    GLFWwindow* window = (GLFWwindow*)gfx::getWindow()->getHandle();
    ImGui_ImplGlfw_InitForVulkan(window, true);
    std::shared_ptr<gfx::VulkanAPI> vulkanAPI = std::dynamic_pointer_cast<gfx::VulkanAPI>(gfx::getGraphicsAPI());
    ImGui_ImplVulkan_InitInfo info{};
    info.Instance = vulkanAPI->getInstance()->getHandle();
    info.PhysicalDevice = vulkanAPI->getPhysicalDevice()->getHandle();
    info.Device = vulkanAPI->getDevice()->getHandle();
    info.QueueFamily = vulkanAPI->getDevice()->getGraphicsQueueFamily();
    info.Queue = vulkanAPI->getDevice()->getGraphicsQueue();
    info.PipelineCache = nullptr;
    info.DescriptorPool = vulkanAPI->getUiDescriptorPool()->getHandle();
    info.Allocator = nullptr;
    info.MinImageCount = 2;
    info.ImageCount = 2;
    info.CheckVkResultFn = nullptr;
    ImGui_ImplVulkan_Init(&info, vulkanAPI->getRenderPass()->getHandle());

    // Upload Fonts
    {
        std::shared_ptr<gfx::vk::CommandPool> commandPool = vulkanAPI->getCommandPool();
        VkCommandBuffer commandBuffer = commandPool->beginSingleTimeCommands();
        ImGui_ImplVulkan_CreateFontsTexture(commandBuffer);
        commandPool->endSingleTimeCommands(commandBuffer);
    }
    ImGui_ImplVulkan_DestroyFontUploadObjects();
}

void UILayer::setTheme() {
    ImGui::StyleColorsDark();
    auto& colors = ImGui::GetStyle().Colors;
    colors[ImGuiCol_WindowBg] = ImVec4{0.1f, 0.105f, 0.11f, 1.0f};

    // Headers
    colors[ImGuiCol_Header] = ImVec4{0.25f, 0.255f, 0.26f, 1.0f};
    colors[ImGuiCol_HeaderHovered] = ImVec4{0.3f, 0.305f, 0.31f, 1.0f};
    colors[ImGuiCol_HeaderActive] = ImVec4{0.15f, 0.1505f, 0.151f, 1.0f};

    // Buttons
    colors[ImGuiCol_Button] = ImVec4{0.2f, 0.205f, 0.21f, 1.0f};
    colors[ImGuiCol_ButtonHovered] = ImVec4{0.3f, 0.305f, 0.31f, 1.0f};
    colors[ImGuiCol_ButtonActive] = ImVec4{0.15f, 0.1505f, 0.151f, 1.0f};

    // Frame BG
    colors[ImGuiCol_FrameBg] = ImVec4{0.2f, 0.205f, 0.21f, 1.0f};
    colors[ImGuiCol_FrameBgHovered] = ImVec4{0.3f, 0.305f, 0.31f, 1.0f};
    colors[ImGuiCol_FrameBgActive] = ImVec4{0.15f, 0.1505f, 0.151f, 1.0f};

    // Tabs
    colors[ImGuiCol_Tab] = ImVec4{0.15f, 0.1505f, 0.151f, 1.0f};
    colors[ImGuiCol_TabHovered] = ImVec4{0.38f, 0.3805f, 0.381f, 1.0f};
    colors[ImGuiCol_TabActive] = ImVec4{0.28f, 0.2805f, 0.281f, 1.0f};
    colors[ImGuiCol_TabUnfocused] = ImVec4{0.15f, 0.1505f, 0.151f, 1.0f};
    colors[ImGuiCol_TabUnfocusedActive] = ImVec4{0.2f, 0.205f, 0.21f, 1.0f};

    // Title
    colors[ImGuiCol_TitleBg] = ImVec4{0.15f, 0.1505f, 0.151f, 1.0f};
    colors[ImGuiCol_TitleBgActive] = ImVec4{0.15f, 0.1505f, 0.151f, 1.0f};
    colors[ImGuiCol_TitleBgCollapsed] = ImVec4{0.15f, 0.1505f, 0.151f, 1.0f};
}

} // namespace atta::ui
