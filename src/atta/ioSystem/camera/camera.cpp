//--------------------------------------------------
// Atta IO System
// camera.cpp
// Date: 2021-09-26
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/ioSystem/camera/camera.h>
#include <atta/ioSystem/camera/linuxCamera.h>

namespace atta::io
{
    Camera::Camera(CreateInfo info):
        _deviceName(info.deviceName), 
        _pixelFormat(info.pixelFormat), _resolution(info.resolution),
        _fps(info.fps), _debugName(info.debugName)
    {

    }

    std::vector<std::string> Camera::getAvailableDeviceNames()
    {
        std::vector<std::string> deviceNames;

#ifdef ATTA_OS_LINUX
        for(auto& entry : fs::directory_iterator("/dev"))
        {
            std::string name = entry.path().string();
            if(name.find("video") != std::string::npos)
            {
                CreateInfo info;
                info.deviceName = name;
                info.debugName = StringId("io::Camera evalVideo");
                LinuxCamera camera(info);

                // Camera::start will return true only if it is possible to read the camera data
                if(camera.isValidDevice())
                    deviceNames.push_back(name);
            }
        }
#endif
        std::sort(deviceNames.begin(), deviceNames.end());
        return deviceNames;
    }
}
