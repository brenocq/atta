//--------------------------------------------------
// Atta Component Module
// cameraSensor.cpp
// Date: 2021-11-23
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/component/components/cameraSensor.h>
#include <atta/event/events/uiCameraComponent.h>
#include <atta/event/interface.h>
#include <atta/sensor/interface.h>
#include <imgui.h>

namespace atta::component {

template <>
ComponentDescription& TypedRegistry<CameraSensor>::getDescription() {
    static ComponentDescription desc = {
        "Camera Sensor",
        {
            {AttributeType::BOOL, offsetof(CameraSensor, enabled), "enabled"},
            {AttributeType::UINT32, offsetof(CameraSensor, width), "width"},
            {AttributeType::UINT32, offsetof(CameraSensor, height), "height"},
            {AttributeType::FLOAT32, offsetof(CameraSensor, fov), "fov", 0.0f, 120.0f},
            {AttributeType::FLOAT32, offsetof(CameraSensor, far), "far", 0.0f, 10000.0f, 0.5f},
            {AttributeType::FLOAT32, offsetof(CameraSensor, near), "near", 0.0f, 10000.0f, 0.5f},
            {AttributeType::FLOAT32, offsetof(CameraSensor, fps), "fps", 0.0f, 120.0f},
            {AttributeType::UINT32, offsetof(CameraSensor, cameraType), "cameraType", {}, {}, {}, {"Orthographic", "Perspective"}},
            {AttributeType::UINT32, offsetof(CameraSensor, rendererType), "rendererType", {}, {}, {}, {"Fast", "Phong", "Pbr"}},
            {AttributeType::FLOAT32, offsetof(CameraSensor, captureTime), "captureTime"},
        },
        1024,                                      // Max instances
        {},                                        // Serialize
        {},                                        // Deserialize
        {                                          // renderUI
         {"", [=](void* data, std::string imguiId) // Define how the component will be rendered
          {
              const std::vector<AttributeDescription> aDescs = TypedRegistry<CameraSensor>::getDescription().attributeDescriptions;

              if (ImGui::Button(("View image" + imguiId + "image").c_str())) {
                  event::UiCameraComponent event;
                  event.component = static_cast<CameraSensor*>(data);
                  event.uiEvent = event::UiCameraComponent::UiEvent::VIEW_BUTTON_CLICKED;
                  event::publish(event);
              }

              for (unsigned i = 0; i < aDescs.size(); i++) {
                  // Calculate data and size
                  float size = (i != aDescs.size() - 1) ? aDescs[i + 1].offset - aDescs[i].offset : sizeof(CameraSensor) - aDescs[i].offset;
                  void* attribData = (void*)((uint8_t*)data + aDescs[i].offset);

                  Registry::renderUIAttribute(aDescs[i], attribData, size, imguiId + aDescs[i].name);
              }
          }}}};

    return desc;
}

const uint8_t* CameraSensor::getImage() {
    if (captureTime < 0.0f)
        return nullptr;

    std::vector<sensor::CameraInfo>& cameraInfos = sensor::getCameraInfos();
    for (auto& cameraInfo : cameraInfos)
        if (cameraInfo.component == this)
            return cameraInfo.data.data();
    ASSERT(false, "(component::CameraSensor) Could not get camera frame from sensor::Manager.");
}

} // namespace atta::component
