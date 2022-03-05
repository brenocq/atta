//--------------------------------------------------
// Atta UI System
// sensorDrawer.cpp
// Date: 2022-03-05
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/uiSystem/layers/editor/drawers/sensorDrawer.h>
#include <atta/graphicsSystem/drawer.h>
#include <atta/sensorSystem/sensorManager.h>

namespace atta::ui
{
    void SensorDrawer::update()
    {
        updateCameras();
    }

    void SensorDrawer::updateCameras()
    {
        Drawer::clear<Drawer::Line>("atta::sensor::Camera"_ssid);
        std::vector<SensorManager::CameraInfo>& cameras = SensorManager::getCameraInfos();
        for(uint32_t i = 0; i < cameras.size(); i++)
        {
            CameraComponent* cameraComponent = cameras[i].component;
            std::shared_ptr<Camera> camera = cameras[i].camera;
            vec3 pos = camera->getPosition();
            vec3 front = camera->getFront();
            vec3 up = camera->getUp();
            vec3 left = camera->getLeft();
            float fov = cameraComponent->fov;
            float ratio = cameraComponent->width/(float)cameraComponent->height;

            vec3 plane = pos+front;
            vec3 midLeft = left*tan(radians(fov/2));
            vec3 midUp = up*tan(radians(fov/(2*ratio)));
            vec3 tl = midLeft+midUp; 
            vec3 tr = -midLeft+midUp; 
            vec3 bl = midLeft-midUp; 
            vec3 br = -midLeft-midUp; 

            Drawer::add(Drawer::Line(pos, plane+tl, {1,1,0,1}, {1,1,0,1}), "atta::sensor::Camera"_ssid);
            Drawer::add(Drawer::Line(pos, plane+tr, {1,1,0,1}, {1,1,0,1}), "atta::sensor::Camera"_ssid);
            Drawer::add(Drawer::Line(pos, plane+bl, {1,1,0,1}, {1,1,0,1}), "atta::sensor::Camera"_ssid);
            Drawer::add(Drawer::Line(pos, plane+br, {1,1,0,1}, {1,1,0,1}), "atta::sensor::Camera"_ssid);

            Drawer::add(Drawer::Line(plane+tl, plane+tr, {1,1,0,1}, {1,1,0,1}), "atta::sensor::Camera"_ssid);
            Drawer::add(Drawer::Line(plane+tr, plane+br, {1,1,0,1}, {1,1,0,1}), "atta::sensor::Camera"_ssid);
            Drawer::add(Drawer::Line(plane+br, plane+bl, {1,1,0,1}, {1,1,0,1}), "atta::sensor::Camera"_ssid);
            Drawer::add(Drawer::Line(plane+bl, plane+tl, {1,1,0,1}, {1,1,0,1}), "atta::sensor::Camera"_ssid);
        }
    }
}
