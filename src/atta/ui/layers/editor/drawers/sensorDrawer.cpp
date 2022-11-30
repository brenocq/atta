//--------------------------------------------------
// Atta UI Module
// sensorDrawer.cpp
// Date: 2022-03-05
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/component/components/prototype.h>
#include <atta/component/components/transform.h>
#include <atta/component/interface.h>
#include <atta/graphics/drawer.h>
#include <atta/sensor/interface.h>
#include <atta/ui/layers/editor/drawers/sensorDrawer.h>

namespace atta::ui {

void SensorDrawer::update() { updateCameras(); }

void SensorDrawer::updateCameras() {
    graphics::Drawer::clear<graphics::Drawer::Line>("atta::sensor::Camera"_ssid);
    std::vector<sensor::CameraInfo>& cameras = sensor::getCameraInfos();
    for (uint32_t i = 0; i < cameras.size(); i++) {
        if (!cameras[i].initialized)
            continue;
        if (component::Entity(cameras[i].entity).isPrototype())
            continue;
        component::CameraSensor* cameraComponent = cameras[i].component;
        std::shared_ptr<graphics::Camera> camera = cameras[i].camera;
        vec3 pos = camera->getPosition();
        vec3 front = camera->getFront();
        vec3 up = camera->getUp();
        vec3 left = camera->getLeft();
        float fov = cameraComponent->fov;
        float ratio = cameraComponent->width / (float)cameraComponent->height;

        // Calculate line scale from entity scale
        component::Entity entity = cameras[i].entity;
        auto t = entity.get<component::Transform>();
        mat4 worldModel = t->getWorldTransform(entity);
        vec3 scaleVec = worldModel.getScale();
        float scale = (scaleVec.x + scaleVec.y + scaleVec.z) / 3.0f;

        // Calculate lines
        vec3 plane = pos + front * scale;
        vec3 midLeft = left * tan(radians(fov / 2)) * scale;
        vec3 midUp = up * tan(radians(fov / (2 * ratio))) * scale;
        vec3 tl = midLeft + midUp;
        vec3 tr = -midLeft + midUp;
        vec3 bl = midLeft - midUp;
        vec3 br = -midLeft - midUp;

        graphics::Drawer::add(graphics::Drawer::Line(pos, plane + tl, {1, 1, 0, 1}, {1, 1, 0, 1}), "atta::sensor::Camera"_ssid);
        graphics::Drawer::add(graphics::Drawer::Line(pos, plane + tr, {1, 1, 0, 1}, {1, 1, 0, 1}), "atta::sensor::Camera"_ssid);
        graphics::Drawer::add(graphics::Drawer::Line(pos, plane + bl, {1, 1, 0, 1}, {1, 1, 0, 1}), "atta::sensor::Camera"_ssid);
        graphics::Drawer::add(graphics::Drawer::Line(pos, plane + br, {1, 1, 0, 1}, {1, 1, 0, 1}), "atta::sensor::Camera"_ssid);

        graphics::Drawer::add(graphics::Drawer::Line(plane + tl, plane + tr, {1, 1, 0, 1}, {1, 1, 0, 1}), "atta::sensor::Camera"_ssid);
        graphics::Drawer::add(graphics::Drawer::Line(plane + tr, plane + br, {1, 1, 0, 1}, {1, 1, 0, 1}), "atta::sensor::Camera"_ssid);
        graphics::Drawer::add(graphics::Drawer::Line(plane + br, plane + bl, {1, 1, 0, 1}, {1, 1, 0, 1}), "atta::sensor::Camera"_ssid);
        graphics::Drawer::add(graphics::Drawer::Line(plane + bl, plane + tl, {1, 1, 0, 1}, {1, 1, 0, 1}), "atta::sensor::Camera"_ssid);
    }
}

} // namespace atta::ui
