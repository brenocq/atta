//--------------------------------------------------
// Atta Component Module
// camera.cpp
// Date: 2021-11-23
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/component/components/camera.h>
#include <atta/event/events/uiCameraComponent.h>
#include <atta/event/manager.h>
#include <atta/sensor/manager.h>
#include <imgui.h>

namespace atta::component {

template <>
ComponentDescription& TypedComponentRegistry<Camera>::getDescription() {
    static ComponentDescription desc = {
        "Camera",
        {{AttributeType::UINT32, offsetof(Camera, width), "width"},
         {AttributeType::UINT32, offsetof(Camera, height), "height"},
         {AttributeType::FLOAT32, offsetof(Camera, fov), "fov", 0.0f, 120.0f},
         {AttributeType::FLOAT32, offsetof(Camera, far), "far", 0.0f, 10000.0f, 0.5f},
         {AttributeType::FLOAT32, offsetof(Camera, near), "near", 0.0f, 10000.0f, 0.5f},
         {AttributeType::FLOAT32, offsetof(Camera, fps), "fps", 0.0f, 120.0f},
         {AttributeType::UINT32, offsetof(Camera, cameraType), "cameraType", {}, {}, {}, {"Orthographic", "Perspective"}},
         {AttributeType::UINT32, offsetof(Camera, rendererType), "rendererType", {}, {}, {}, {"Fast", "Phong", "Pbr"}}},
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
                  event::Manager::publish(event);
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

const std::vector<uint8_t>& Camera::getFrame() {
    std::vector<sensor::Manager::CameraInfo>& cameraInfos = sensor::Manager::getCameraInfos();
    for (auto& cameraInfo : cameraInfos)
        if (cameraInfo.component == this)
            return cameraInfo.data;
    LOG_ERROR("component::Camera", "Could not get camera frame from SensorManager");
    return {};
}

} // namespace atta::component
