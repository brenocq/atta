//--------------------------------------------------
// Atta UI Module
// manager.cpp
// Date: 2021-09-28
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/component/components/cameraSensor.h>
#include <atta/component/components/material.h>
#include <atta/component/components/mesh.h>
#include <atta/component/components/polygonCollider2D.h>
#include <atta/component/typedComponentRegistry.h>
#include <atta/event/events/uiCameraComponent.h>
#include <atta/event/interface.h>
#include <atta/file/interface.h>
#include <atta/graphics/interface.h>
#include <atta/resource/interface.h>
#include <atta/ui/interface.h>
#include <atta/ui/manager.h>
#include <atta/ui/widgets/component.h>

// clang-format off
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#if ATTA_VULKAN_SUPPORT
#include <backends/imgui_impl_vulkan.h>
#include <atta/graphics/apis/vulkan/vulkanAPI.h>
#endif
#include <ImGuizmo.h>
#include <implot.h>
#include <implot3d.h>
// clang-format on

namespace atta::ui {

Manager& Manager::getInstance() {
    static Manager instance;
    return instance;
}

void Manager::startUpImpl() {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImPlot::CreateContext();
    ImPlot3D::CreateContext();

    ImGuiIO& io = ImGui::GetIO();
    (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;     // Enable Docking

    io.FontDefault = io.Fonts->AddFontFromFileTTF(file::solveResourcePath("fonts/NotoSans-Regular.ttf").string().c_str(), 16.0f);

    // Don't save imgui.ini
    io.IniFilename = NULL;
#ifdef ATTA_OS_WEB
    // Disable file-system access when building with Emscripten (no fopen() of the imgui.init file)
    io.IniFilename = NULL;
#else
    // Multiple viewports not supported for the web
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable; // Enable Multi-Viewport
#endif

    switch (gfx::getGraphicsAPI()->getType()) {
        case gfx::GraphicsAPI::OPENGL:
            initOpenGL();
            break;
        case gfx::GraphicsAPI::VULKAN:
            initVulkan();
            break;
    }
    setTheme();

    gfx::setUiRenderViewportsFunc([&]() { _editor.renderViewports(); });
    gfx::setUiRenderFunc([&]() { render(); });
    gfx::setUiShutDownFunc([&]() { shutDownImpl(); });
    gfx::setUiStartUpFunc([&]() { startUpImpl(); });

    registerCustomComponentUIs();

    // Start up editor window
    _editor.startUp();
}

void Manager::shutDownImpl() {
    // Make sure all rendering operations are done
    gfx::getGraphicsAPI()->waitDevice();

    // Destroy editor
    _editor.shutDown();

    // Destroy plotting contexts
    ImPlot3D::DestroyContext();
    ImPlot::DestroyContext();

    // Destroy ImGui context
    switch (gfx::getGraphicsAPI()->getType()) {
        case gfx::GraphicsAPI::OPENGL:
            ImGui_ImplOpenGL3_Shutdown();
            break;
        case gfx::GraphicsAPI::VULKAN:
#if ATTA_VULKAN_SUPPORT
            ImGui_ImplVulkan_Shutdown();
#endif
            break;
    }
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

bool renderComboImage(std::string attribute, StringId& image) {
    bool isImage = (image != StringId());
    bool updated = false;
    std::string imguiId = attribute;
    if (isImage) {
        std::string selectedName = image.getString();
        if (ImGui::BeginCombo(("##ComboImage" + imguiId).c_str(), selectedName.c_str())) {
            std::vector<StringId> rImages = resource::getResources<resource::Image>();
            for (StringId rImage : rImages) {
                std::string imageStr = rImage.getString();
                if (imageStr == "")
                    imageStr = "##";
                const bool selected = (rImage == image);
                if (ImGui::Selectable(imageStr.c_str(), selected)) {
                    image = rImage;
                    updated = true;
                }
                if (selected)
                    ImGui::SetItemDefaultFocus();
            }
            ImGui::EndCombo();
        }
    }

    if (ImGui::Checkbox(("Is image##IsImage" + imguiId).c_str(), &isImage)) {
        image = isImage ? "textures/white.png"_sid : StringId();
        updated = true;
    }
    return updated;
}

void Manager::registerCustomComponentUIs() {
    //---------- Mesh ----------//
    ui::registerComponentUI<cmp::Mesh>([](cmp::Entity entity, cmp::Component* comp) {
        // Get mesh resource
        cmp::Mesh* mesh = static_cast<cmp::Mesh*>(comp);
        res::Mesh* m = res::get<res::Mesh>(mesh->sid.getString());
        if (m == nullptr)
            return;

        std::string selectedName = mesh->sid.getString();

        // Selection
        if (ImGui::BeginCombo("##Combo", selectedName.c_str())) {
            std::vector<StringId> rMeshes = res::getResources<res::Mesh>();
            for (StringId rMesh : rMeshes) {
                std::string meshStr = rMesh.getString();
                if (meshStr == "")
                    meshStr = "##";
                const bool selected = (rMesh == mesh->sid);
                if (ImGui::Selectable(meshStr.c_str(), selected))
                    mesh->sid = rMesh;
                if (selected)
                    ImGui::SetItemDefaultFocus();
            }
            ImGui::EndCombo();
        }
    });

    //---------- Camera Sensor ----------//
    ui::registerComponentUI<cmp::CameraSensor>([](cmp::Entity entity, cmp::Component* comp) {
        if (ImGui::Button("View image")) {
            evt::UiCameraComponent event;
            event.component = static_cast<cmp::CameraSensor*>(comp);
            event.uiEvent = evt::UiCameraComponent::UiEvent::VIEW_BUTTON_CLICKED;
            evt::publish(event);
        }

        const std::vector<cmp::AttributeDescription> aDescs =
            cmp::TypedComponentRegistry<cmp::CameraSensor>::getInstance().getDescription().attributeDescriptions;
        renderAttributes(aDescs, comp, sizeof(cmp::CameraSensor));
    });

    //---------- Material ----------//
    ui::registerComponentUI<cmp::Material>([](cmp::Entity entity, cmp::Component* comp) {
        cmp::Material* material = static_cast<cmp::Material*>(comp);
        res::Material* m = res::get<res::Material>(material->sid.getString());
        if (m == nullptr)
            return;

        std::string selectedName = material->sid.getString();

        //---------- Selection ----------//
        if (ImGui::BeginCombo("##Combo", selectedName.c_str())) {
            std::vector<StringId> rMaterials = res::getResources<res::Material>();
            for (StringId rMaterial : rMaterials) {
                std::string materialStr = rMaterial.getString();
                if (materialStr == "")
                    materialStr = "##";
                const bool selected = (rMaterial == material->sid);
                if (ImGui::Selectable(materialStr.c_str(), selected))
                    material->sid = rMaterial;
                if (selected)
                    ImGui::SetItemDefaultFocus();
            }
            ImGui::EndCombo();
        }

        //---------- Create ----------//
        static bool warnDuplicated = false;
        ImGui::SameLine();
        if (ImGui::Button("+")) {
            ImGui::OpenPopup("Create material");
            warnDuplicated = false;
        }

        if (ImGui::BeginPopupModal("Create material")) {
            ImGui::Text("What will be the material name?");

            // Name input
            static char buf[64] = "Material";
            ImGui::InputText("##MaterialName", buf, sizeof(buf) / sizeof(char));

            // Warn if duplicated
            if (warnDuplicated)
                ImGui::Text("A material with this name already exists.");

            // Cancel button
            if (ImGui::Button("Cancel"))
                ImGui::CloseCurrentPopup();

            // Create button
            ImGui::SameLine();
            if (ImGui::Button("Create")) {
                std::string bufStr(buf);
                std::vector<StringId> matSids = resource::getResources<resource::Material>();

                // Check if is duplicated
                warnDuplicated = false;
                for (StringId matSid : matSids)
                    if (matSid.getString() == bufStr) {
                        warnDuplicated = true;
                        break;
                    }

                // If not duplicated, create
                if (!warnDuplicated) {
                    resource::Material* created = resource::create<resource::Material>(bufStr, resource::Material::CreateInfo{});
                    if (created)
                        material->sid = created->getId();
                    ImGui::CloseCurrentPopup();
                }
            }
            ImGui::EndPopup();
        }

        //---------- Delete ----------//
        ImGui::SameLine();
        if (ImGui::Button("-"))
            ImGui::OpenPopup("Delete material");
        if (ImGui::BeginPopupModal("Delete material")) {
            ImGui::Text("Are you sure you want to delete this material?");
            ImGui::Text("All component::Material that use this material\n"
                        "will lose the reference.");
            if (ImGui::Button("Cancel")) {
                ImGui::CloseCurrentPopup();
            }
            ImGui::SameLine();
            if (ImGui::Button("Delete")) {
                resource::destroy<resource::Material>(material->sid.getString());
                material->sid = "Material";
                ImGui::CloseCurrentPopup();
            }
            ImGui::EndPopup();
        }

        //---------- Edit ----------//
        ImGui::Text("Color");
        if (!m->colorIsImage()) {
            vec3 color = m->getColor();
            bool updated = false;
            if (ImGui::SliderFloat("R##ColorR", &color.x, 0.0f, 1.0f))
                updated = true;
            if (ImGui::SliderFloat("G##ColorG", &color.y, 0.0f, 1.0f))
                updated = true;
            if (ImGui::SliderFloat("B##ColorB", &color.z, 0.0f, 1.0f))
                updated = true;
            if (updated)
                m->setColor(color);
        }
        StringId colorImage = m->getColorImage();
        if (renderComboImage("Color", colorImage))
            m->setColorImage(colorImage);

        ImGui::Text("Roughness");
        if (!m->roughnessIsImage()) {
            float roughness = m->getRoughness();
            if (ImGui::SliderFloat("##Roughness", &roughness, 0.0f, 1.0f))
                m->setRoughness(roughness);
        }
        StringId roughnessImage = m->getRoughnessImage();
        if (renderComboImage("Roughness", roughnessImage))
            m->setRoughnessImage(roughnessImage);

        ImGui::Text("Metallic");
        if (!m->metallicIsImage()) {
            float metallic = m->getMetallic();
            if (ImGui::SliderFloat("##Metallic", &metallic, 0.0f, 1.0f))
                m->setMetallic(metallic);
        }
        StringId metallicImage = m->getMetallicImage();
        if (renderComboImage("Metallic", metallicImage))
            m->setMetallicImage(metallicImage);

        ImGui::Text("AO");
        if (!m->aoIsImage()) {
            float ao = m->getAo();
            if (ImGui::SliderFloat("##AO", &ao, 0.0f, 1.0f))
                m->setAo(ao);
        }
        StringId aoImage = m->getAoImage();
        if (renderComboImage("AO", aoImage))
            m->setAoImage(aoImage);
    });

    //---------- Polygon Collider 2D ----------//
    ui::registerComponentUI<cmp::PolygonCollider2D>([](cmp::Entity entity, cmp::Component* comp) {
        // Render offset
        const std::vector<cmp::AttributeDescription> aDescs =
            cmp::TypedComponentRegistry<cmp::PolygonCollider2D>::getInstance().getDescription().attributeDescriptions;
        void* attribData = (void*)((uint8_t*)comp + aDescs[0].offset);
        float size = aDescs[1].offset - aDescs[0].offset;
        renderAttribute(aDescs[0], attribData, size);

        // Render polygon points
        ImGui::Text("Points");
        std::vector<vec2>* points = (std::vector<vec2>*)((uint8_t*)comp + aDescs[1].offset);
        for (int i = 0; i < points->size(); i++) {
            ImGui::PushID(i);
            {
                // Edit point
                ImGui::DragFloat2("##PolygonCollider2DPoint", (float*)&((*points)[i]));

                // Delete point
                ImGui::SameLine();
                if (ImGui::Button("-##PolygonCollider2DDeletePoint"))
                    points->erase(points->begin() + i);

                // Rearrange point
                if (i > 0) {
                    ImGui::SameLine();
                    if (ImGui::Button("^")) {
                        std::swap((*points)[i], (*points)[i - 1]);
                    }
                }
                if (i < points->size() - 1) {
                    ImGui::SameLine();
                    if (ImGui::Button("v")) {
                        std::swap((*points)[i], (*points)[i + 1]);
                    }
                }
            }
            ImGui::PopID();
        }
        // Add point
        if (ImGui::Button("+##PolygonCollider2DAddPoint"))
            points->push_back({});
    });
}

void Manager::registerComponentUIImpl(cmp::ComponentId cid, const ComponentUIFunc& renderFunc) { _componentRenderFuncs[cid] = renderFunc; }

std::optional<ComponentUIFunc> Manager::getComponentUIImpl(cmp::ComponentId cid) {
    if (_componentRenderFuncs.find(cid) != _componentRenderFuncs.end())
        return _componentRenderFuncs[cid];
    return std::nullopt;
}

const std::vector<std::shared_ptr<ui::Viewport>>& Manager::getViewportsImpl() const { return _editor.getViewports(); }
void Manager::openViewportModalImpl(StringId sid) { _editor.openViewportModal(sid); }
void Manager::clearViewportsImpl() { _editor.clearViewports(); }
void Manager::addViewportImpl(std::shared_ptr<ui::Viewport> viewport) { _editor.addViewport(viewport); }

bool Manager::getViewportRenderingImpl() const { return _editor.getViewportRendering(); }
void Manager::setViewportRenderingImpl(bool viewportRendering) { _editor.setViewportRendering(viewportRendering); }

unsigned Manager::getViewportDockIdImpl() { return _editor.getViewportDockId(); }

void Manager::setTheme() {
    ImGuiStyle& style = ImGui::GetStyle();

    // Sizes main
    style.ScrollbarRounding = 0.0f;
    style.TabRounding = 0.0f;
    style.WindowPadding = ImVec2(8.0f, 5.0f);
    style.FramePadding = ImVec2(4.0f, 3.0f);
    style.ItemSpacing = ImVec2(5.0f, 4.0f);
    style.ItemInnerSpacing = ImVec2(3.0f, 3.0f);
    style.ScrollbarSize = 12.0f;
    style.GrabMinSize = 5.0f;
    // Sizes borders
    style.WindowBorderSize = 1.0f;
    style.TabBorderSize = 0.0f;
    style.TabBarBorderSize = 1.0f;
    style.TabBarOverlineSize = 0.0f;
    // Sizes rounding
    style.FrameRounding = 3.0f;
    style.ScrollbarRounding = 3.0f;
    style.GrabRounding = 3.0f;
    style.TabRounding = 3.0f;
    // Sizes widgets
    style.SeparatorTextBorderSize = 1.0f;
    style.SeparatorTextAlign = ImVec2(0.5f, 0.5f);
    style.SeparatorTextPadding = ImVec2(0.0f, 0.0f);
    // Sizes docking
    style.DockingSeparatorSize = 2.0f;
    // Sizes misc
    style.DisplaySafeAreaPadding = ImVec2(0.0f, 0.0f);

    constexpr ImVec4 bg = ImVec4(0.20f, 0.20f, 0.20f, 1.0f);
    constexpr ImVec4 titleActive = ImVec4(0.12f, 0.12f, 0.12f, 1.00f);
    constexpr ImVec4 titleHovered = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);
    constexpr ImVec4 titleSelected = ImVec4(0.08f, 0.08f, 0.08f, 1.00f);
    constexpr ImVec4 bgActive = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
    constexpr ImVec4 bgHovered = ImVec4(0.12f, 0.12f, 0.12f, 1.00f);
    constexpr ImVec4 bgSelected = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);
    constexpr ImVec4 disabled = ImVec4(0.15f, 0.15, 0.15f, 1.0f);
    constexpr ImVec4 active = ImVec4(0.30f, 0.30f, 0.30f, 1.0f);
    constexpr ImVec4 hovered = ImVec4(0.35f, 0.35f, 0.35f, 1.0f);
    constexpr ImVec4 selected = ImVec4(0.40f, 0.40f, 0.40f, 1.0f);

    // Colors
    ImVec4* colors = style.Colors;
    colors[ImGuiCol_Text] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
    colors[ImGuiCol_TextDisabled] = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
    colors[ImGuiCol_WindowBg] = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
    colors[ImGuiCol_ChildBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    colors[ImGuiCol_PopupBg] = ImVec4(0.20f, 0.20f, 0.20f, 0.98f);
    colors[ImGuiCol_Border] = ImVec4(0.0f, 0.0f, 0.0f, 1.00f);
    colors[ImGuiCol_BorderShadow] = ImVec4(0.04f, 0.04f, 0.04f, 1.00f);
    colors[ImGuiCol_FrameBg] = bgActive;
    colors[ImGuiCol_FrameBgHovered] = bgHovered;
    colors[ImGuiCol_FrameBgActive] = bgSelected;
    colors[ImGuiCol_TitleBg] = titleActive;
    colors[ImGuiCol_TitleBgActive] = titleSelected;
    colors[ImGuiCol_TitleBgCollapsed] = titleActive;
    colors[ImGuiCol_MenuBarBg] = active;
    colors[ImGuiCol_ScrollbarBg] = bg;
    colors[ImGuiCol_ScrollbarGrab] = active;
    colors[ImGuiCol_ScrollbarGrabHovered] = hovered;
    colors[ImGuiCol_ScrollbarGrabActive] = selected;
    colors[ImGuiCol_CheckMark] = ImVec4(0.39f, 0.39f, 0.39f, 1.00f);
    colors[ImGuiCol_SliderGrab] = active;
    colors[ImGuiCol_SliderGrabActive] = selected;
    colors[ImGuiCol_Button] = active;
    colors[ImGuiCol_ButtonHovered] = hovered;
    colors[ImGuiCol_ButtonActive] = selected;
    colors[ImGuiCol_Header] = bgActive;
    colors[ImGuiCol_HeaderHovered] = bgHovered;
    colors[ImGuiCol_HeaderActive] = bgSelected;
    colors[ImGuiCol_Separator] = active;
    colors[ImGuiCol_SeparatorHovered] = hovered;
    colors[ImGuiCol_SeparatorActive] = selected;
    colors[ImGuiCol_ResizeGrip] = active;
    colors[ImGuiCol_ResizeGripHovered] = hovered;
    colors[ImGuiCol_ResizeGripActive] = selected;
    colors[ImGuiCol_Tab] = disabled;
    colors[ImGuiCol_TabHovered] = hovered;
    colors[ImGuiCol_TabSelected] = selected;
    colors[ImGuiCol_TabSelectedOverline] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    colors[ImGuiCol_TabDimmed] = disabled;
    colors[ImGuiCol_TabDimmedSelected] = selected;
    colors[ImGuiCol_DockingPreview] = ImVec4(1.00f, 1.00f, 1.00f, 0.20f);
    colors[ImGuiCol_DockingEmptyBg] = ImVec4(1.00f, 1.00f, 1.00f, 0.20f);
    colors[ImGuiCol_PlotLines] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
    colors[ImGuiCol_PlotLinesHovered] = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
    colors[ImGuiCol_TableHeaderBg] = ImVec4(0.16f, 0.16f, 0.16f, 1.00f);
    colors[ImGuiCol_TableBorderStrong] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
    colors[ImGuiCol_TableBorderLight] = ImVec4(0.04f, 0.04f, 0.04f, 1.00f);
    colors[ImGuiCol_TableRowBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    colors[ImGuiCol_TableRowBgAlt] = ImVec4(1.00f, 1.00f, 1.00f, 0.02f);
    colors[ImGuiCol_TextSelectedBg] = ImVec4(1.00f, 1.00f, 1.00f, 0.20f);
    colors[ImGuiCol_DragDropTarget] = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
    colors[ImGuiCol_NavCursor] = ImVec4(0.39f, 0.39f, 0.39f, 1.00f);
    colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
    colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
    colors[ImGuiCol_ModalWindowDimBg] = ImVec4(1.00f, 1.00f, 1.00f, 0.08f);
}

void Manager::initOpenGL() {
    GLFWwindow* window = (GLFWwindow*)gfx::getWindow()->getHandle();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 100");
}

void Manager::initVulkan() {
#if ATTA_VULKAN_SUPPORT
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
    info.MinImageCount = 3;
    info.ImageCount = 3;
    info.CheckVkResultFn = nullptr;
    info.RenderPass = vulkanAPI->getRenderPass()->getHandle();
    ImGui_ImplVulkan_Init(&info);

    // Upload Fonts
    if (!ImGui_ImplVulkan_CreateFontsTexture())
        LOG_WARN("ui::Manager", "Failed to created ImGui font texture");
#endif
}

void Manager::render() {
    // Begin
    switch (gfx::getGraphicsAPI()->getType()) {
        case gfx::GraphicsAPI::OPENGL:
            ImGui_ImplOpenGL3_NewFrame();
            break;
        case gfx::GraphicsAPI::VULKAN:
#if ATTA_VULKAN_SUPPORT
            ImGui_ImplVulkan_NewFrame();
#endif
            break;
    }
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    ImGuizmo::BeginFrame();

    // Render UI
    _editor.render();

    // End
    ImGui::Render();
    switch (gfx::getGraphicsAPI()->getType()) {
        case gfx::GraphicsAPI::OPENGL:
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
            break;
        case gfx::GraphicsAPI::VULKAN:
#if ATTA_VULKAN_SUPPORT
            ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(),
                                            std::dynamic_pointer_cast<gfx::VulkanAPI>(gfx::getGraphicsAPI())->getCommandBuffers()->getCurrent());
#endif
            break;
    }

    ImGuiIO& io = ImGui::GetIO();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
        GLFWwindow* backupCurrentContext = glfwGetCurrentContext();
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
        glfwMakeContextCurrent(backupCurrentContext);
    }
}

} // namespace atta::ui
