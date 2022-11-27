//--------------------------------------------------
// Atta Component Module
// camera.cpp
// Date: 2021-11-23
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/component/components/camera.h>
#include <atta/event/events/uiCameraComponent.h>
#include <atta/event/interface.h>
#include <atta/sensor/interface.h>
#include <imgui.h>

namespace atta::component {

template <>
ComponentDescription& TypedComponentRegistry<Camera>::getDescription() {
    static ComponentDescription desc = {
        "Camera",
        {
            {AttributeType::UINT32, offsetof(Camera, width), "width"},
            {AttributeType::UINT32, offsetof(Camera, height), "height"},
            {AttributeType::FLOAT32, offsetof(Camera, fov), "fov", 0.0f, 120.0f},
            {AttributeType::FLOAT32, offsetof(Camera, far), "far", 0.0f, 10000.0f, 0.5f},
            {AttributeType::FLOAT32, offsetof(Camera, near), "near", 0.0f, 10000.0f, 0.5f},
            {AttributeType::FLOAT32, offsetof(Camera, fps), "fps", 0.0f, 120.0f},
            {AttributeType::UINT32, offsetof(Camera, cameraType), "cameraType", {}, {}, {}, {"Orthographic", "Perspective"}},
            {AttributeType::UINT32, offsetof(Camera, rendererType), "rendererType", {}, {}, {}, {"Fast", "Phong", "Pbr"}},
            {AttributeType::FLOAT32, offsetof(Camera, frameTime), "frameTime"},
        },
        1024,                                      // Max instances
        {},                                        // Serialize
        {},                                        // Deserialize
        {                                          // renderUI
         {"", [=](void* data, std::string imguiId) // Define how the component will be rendered
          {
              const std::vector<AttributeDescription> aDescs = TypedComponentRegistry<Camera>::getDescription().attributeDescriptions;

              if (ImGui::Button(("View image" + imguiId + "image").c_str())) {
                  event::UiCameraComponent event;
                  event.component = static_cast<Camera*>(data);
                  event.uiEvent = event::UiCameraComponent::UiEvent::VIEW_BUTTON_CLICKED;
                  event::publish(event);
              }

              for (unsigned i = 0; i < aDescs.size(); i++) {
                  // Calculate data and size
                  float size = (i != aDescs.size() - 1) ? aDescs[i + 1].offset - aDescs[i].offset : sizeof(Camera) - aDescs[i].offset;
                  void* attribData = (void*)((uint8_t*)data + aDescs[i].offset);

                  ComponentRegistry::renderUIAttribute(aDescs[i], attribData, size, imguiId + aDescs[i].name);
              }
          }}}};

    return desc;
}

const uint8_t* Camera::getFrame() {
    std::vector<sensor::CameraInfo>& cameraInfos = sensor::getCameraInfos();
    for (auto& cameraInfo : cameraInfos)
        if (cameraInfo.component == this)
            return cameraInfo.data.data();
    ASSERT(false, "(component::Camera) Could not get camera frame from sensor::Manager.");
}

} // namespace atta::component
