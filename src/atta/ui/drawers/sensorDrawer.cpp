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
#include <atta/ui/drawers/sensorDrawer.h>

namespace atta::ui {

void SensorDrawer::update() {
    updateCameras();
    updateInfrareds();
}

void SensorDrawer::updateCameras() {
    graphics::Drawer::clear<graphics::Drawer::Line>("atta::sensor::Camera"_ssid);

    if (!sensor::getShowCameras())
        return;

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
        vec3 scaleVec = t->getWorldTransform(entity).scale;
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

void SensorDrawer::updateInfrareds() {
    graphics::Drawer::clear<graphics::Drawer::Line>("atta::sensor::Infrared"_ssid);

    if (!sensor::getShowInfrareds())
        return;

    std::vector<sensor::InfraredInfo>& infrareds = sensor::getInfraredInfos();
    for (uint32_t i = 0; i < infrareds.size(); i++) {
        component::Entity entity = infrareds[i].entity;
        component::InfraredSensor* ir = infrareds[i].component;

        // Ignore prototypes
        if (component::Entity(entity).isPrototype())
            continue;
        // Ignore if does not have measurement yet
        if (ir->measurementTime < 0.0f)
            continue;

        // Calculate ray
        component::Transform worldTrans = cmp::Transform::getEntityWorldTransform(entity);
        vec3 rayDir = vec3(1.0f, 0.0f, 0.0f);
        worldTrans.orientation.rotateVector(rayDir);
        vec3 begin = worldTrans.position;

        // Calculate color
        bool hitted = ir->measurement + 0.000001f < ir->upperLimit;
        vec4 color = hitted ? vec4(1, 0, 0, 1) : vec4(1, 1, 0, 1);

        // Draw line
        graphics::Drawer::add(graphics::Drawer::Line(begin, begin + rayDir * ir->measurement, color, color), "atta::sensor::Infrared"_ssid);
    }
}

} // namespace atta::ui
