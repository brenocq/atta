//--------------------------------------------------
// Atta UI Module
// sensorDrawer.cpp
// Date: 2022-03-05
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/graphics/drawer.h>
#include <atta/sensor/manager.h>
#include <atta/ui/layers/editor/drawers/sensorDrawer.h>

namespace atta::ui {

void SensorDrawer::update() { updateCameras(); }

void SensorDrawer::updateCameras() {
    graphics::Drawer::clear<graphics::Drawer::Line>("atta::sensor::Camera"_ssid);
    std::vector<sensor::Manager::CameraInfo>& cameras = sensor::Manager::getCameraInfos();
    for (uint32_t i = 0; i < cameras.size(); i++) {
        component::Camera* cameraComponent = cameras[i].component;
        std::shared_ptr<graphics::Camera> camera = cameras[i].camera;
        vec3 pos = camera->getPosition();
        vec3 front = camera->getFront();
        vec3 up = camera->getUp();
        vec3 left = camera->getLeft();
        float fov = cameraComponent->fov;
        float ratio = cameraComponent->width / (float)cameraComponent->height;

        vec3 plane = pos + front;
        vec3 midLeft = left * tan(radians(fov / 2));
        vec3 midUp = up * tan(radians(fov / (2 * ratio)));
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
