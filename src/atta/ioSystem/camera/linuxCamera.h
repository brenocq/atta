//--------------------------------------------------
// Atta IO System
// linuxCamera.h
// Date: 2021-09-26
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_IO_SYSTEM_CAMERA_LINUX_CAMERA_H
#define ATTA_IO_SYSTEM_CAMERA_LINUX_CAMERA_H
#ifdef ATTA_OS_LINUX
#include <atta/ioSystem/camera/camera.h>

namespace atta::io
{
    class LinuxCamera : public Camera
    {
    public:
        LinuxCamera(Camera::CreateInfo info);
        ~LinuxCamera();

        bool start() override;
        void readFrame() override;

        //---------- Setters ----------//
        bool setFormat(Camera::PixelFormat pixelFormat, Camera::Resolution resolution) override;
        bool setFps(unsigned fps) override;

        //---------- Getters ----------//
        std::vector<Camera::FormatInfo> getAvailableFormats() override;
        bool isValidDevice() override;

    private:
        void openDevice();
        bool initDevice();
        bool startCapturing();

        int _fd;

        // Buffers
        struct Buffer {
            void *start;
            size_t length;
        };
        std::vector<Buffer> _buffers;
    };
}

#endif// ATTA_OS_LINUX
#endif// ATTA_IO_SYSTEM_CAMERA_LINUX_CAMERA_H
