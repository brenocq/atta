//--------------------------------------------------
// Atta IO Module
// linuxCamera.h
// Date: 2021-09-26
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_IO_CAMERA_LINUX_CAMERA_H
#define ATTA_IO_CAMERA_LINUX_CAMERA_H

#ifdef ATTA_OS_LINUX
#include <atta/io/camera/camera.h>

namespace atta::io {

class LinuxCamera : public Camera {
  public:
    LinuxCamera(Camera::CreateInfo info);
    ~LinuxCamera();

    bool start() override;
    bool stop() override;
    bool readFrame() override;

    //---------- Setters ----------//
    bool setFormat(Camera::PixelFormat pixelFormat, Camera::Resolution resolution) override;
    bool setFps(unsigned fps) override;

    //---------- Getters ----------//
    std::vector<Camera::FormatInfo> getAvailableFormats() override;
    bool isValidDevice() override;

  private:
    bool openDevice();
    bool initDevice();
    bool startCapturing();

    int _fd;

    // Buffers
    struct Buffer {
        void* start;
        size_t length;
    };
    std::vector<Buffer> _buffers;
    std::vector<Camera::FormatInfo> _availableFormats;
};

} // namespace atta::io

#endif // ATTA_OS_LINUX
#endif // ATTA_IO_CAMERA_LINUX_CAMERA_H
