//--------------------------------------------------
// Atta IO System
// linuxCamera.cpp
// Date: 2021-09-26
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifdef ATTA_OS_LINUX
#include <atta/ioSystem/camera/linuxCamera.h>

#include <errno.h>
#include <cstring>
#include <sys/stat.h>// Check device file
#include <fcntl.h>// Low level file i/o
#include <sys/ioctl.h>// i/o control
#include <sys/mman.h>
#include <unistd.h>
#include <linux/videodev2.h>

#define V4L2_ERROR_MSG(cmd) LOG_WARN("io::LinuxCamera", "Failed to run $0 for device '$3'. Error $1: $2", cmd, errno, strerror(errno), _deviceName)

namespace atta::io
{
    LinuxCamera::LinuxCamera(Camera::CreateInfo info):
        Camera(info), _fd(-1)
    {
    }

    LinuxCamera::~LinuxCamera()
    {
        // Unmap buffers
        for(size_t i = 0; i<_buffers.size(); i++)
            if(munmap(_buffers[i].start, _buffers[i].length) == -1)
                LOG_WARN("io::LinuxCamera", "Could not unmap buffer");

        // Close camera file descriptor
        if(_fd != -1 && close(_fd) == -1)
            LOG_WARN("io::LinuxCamera", "Could not close camera file descriptor");
        _fd = -1;
    }

    bool LinuxCamera::start()
    {
        if(!openDevice())
            return false;
        if(!initDevice())
            return false;
        if(!startCapturing())
            return false;
        LOG_INFO("io::LinuxCamera", "Camera $0 initialized", _deviceName);
        return true;
    }

    bool LinuxCamera::stop()
    {
        if(_capturing)
        {
            enum v4l2_buf_type type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
            if(ioctl(_fd, VIDIOC_STREAMOFF, &type) == -1)
            {
                V4L2_ERROR_MSG("VIDIOC_STREAMOFF");
                return false;
            }
        }

        // Unmap buffers
        for(size_t i = 0; i<_buffers.size(); i++)
            if(munmap(_buffers[i].start, _buffers[i].length) == -1)
                LOG_WARN("io::LinuxCamera", "Could not unmap buffer");

        // Close camera file descriptor
        if(_fd != -1 && close(_fd) == -1)
            LOG_WARN("io::LinuxCamera", "Could not close camera file descriptor");
        _fd = -1;

        _capturing = false;
        return true;
    }

    bool LinuxCamera::isValidDevice()
    {
        struct v4l2_capability cap;

        // Open if not already open
        if(!openDevice())
            return false;

        //---------- Get device capabilities ----------//
        // Get capabilities
        if(ioctl(_fd, VIDIOC_QUERYCAP, &cap) == -1)
            return false;

        // Check if it is a device capable of video capturing
        if(!(cap.capabilities & V4L2_CAP_VIDEO_CAPTURE))
            return false;

        //---------- Check if can enumerate input ----------//
        struct v4l2_input input;
        int index;
        if(ioctl(_fd, VIDIOC_G_INPUT, &index) == -1)
        {
            V4L2_ERROR_MSG("VIDIOC_G_INPUT");
            return false;
        }

        memset(&input, 0, sizeof(input));
        input.index = index;

        if(ioctl(_fd, VIDIOC_ENUMINPUT, &input) == -1)
        {
            V4L2_ERROR_MSG("VIDIOC_ENUMINPUT");
            return false;
        }

        return true;
    }

    bool LinuxCamera::readFrame()
    {
        fd_set fds;

        // Set with file descriptor to check
        FD_ZERO(&fds);
        FD_SET(_fd, &fds);
        // Set timeout
        struct timeval tv;
        tv.tv_sec = 2;
        tv.tv_usec = 0;

        // Check if can read from fd
        int r = select(_fd + 1, &fds, NULL, NULL, &tv);

        if(r == -1)
        {
            LOG_WARN("io::LinuxCamera", "Error when trying to receive camera frame");
            return false;
        }

        if(r == 0)
        {
            LOG_WARN("io::LinuxCamera", "Timeout when trying to receive camera frame");
            return false;
        }

        // There is something to read in the camera fd, reading frame now
        struct v4l2_buffer buf;
        memset(&(buf), 0, sizeof(buf));
        buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        buf.memory = V4L2_MEMORY_MMAP;

        if(ioctl(_fd, VIDIOC_DQBUF, &buf) == -1)
        {
            V4L2_ERROR_MSG("VIDIOC_DQBUF");
            return false;
        }

        if(buf.index >= _buffers.size())
        {
            LOG_WARN("io::LinuxCamera", "Invalid buffer index when trying to receive camera frame");
            return false;
        }

        uint8_t* start = (uint8_t*)_buffers[buf.index].start;
        size_t size = buf.bytesused;
        _frame = std::vector<uint8_t>(start, start+size);
        if(_frame.size() == 0)
        {
            LOG_WARN("io::LinuxCamera", "Empty camera frame");
            return false;
        }

        if(ioctl(_fd, VIDIOC_QBUF, &buf) == -1)
        {
            V4L2_ERROR_MSG("VIDIOC_QBUF");
            return false;
        }
        return true;
    }

    bool LinuxCamera::setFormat(Camera::PixelFormat pixelFormat, Camera::Resolution resolution)
    {
        bool wasCapturing = _capturing;
        if(wasCapturing) stop();
        // TODO Not working yet

        //struct v4l2_format fmt;
        //memset(&(fmt), 0, sizeof(fmt));
        //fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        //fmt.fmt.pix.width = resolution.width;
        //fmt.fmt.pix.height = resolution.height;

        //if(pixelFormat == PIXEL_FORMAT_MJPEG)
        //    fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_MJPEG;
        //else if(pixelFormat == V4L2_PIX_FMT_YUYV)
        //    fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_MJPEG;
        //else
        //{
        //    LOG_WARN("io::LinuxCamera", "[w](setFormat)[] Invalid pixel format. Error $0: $1", errno, strerror(errno));
        //    return false;
        //}
        //fmt.fmt.pix.field = V4L2_FIELD_NONE;

        //// Use default image format
        //if(ioctl(_fd, VIDIOC_S_FMT, &fmt) == -1)
        //{
        //    LOG_WARN("io::LinuxCamera", "[w](setFormat)[] Could not set new format. Error $0: $1", errno, strerror(errno));
        //    return false;
        //}

        _pixelFormat = pixelFormat;
        _resolution = resolution;

        if(wasCapturing) start();
        return true;
    }

    bool LinuxCamera::setFps(unsigned fps)
    {
        struct v4l2_streamparm param;
        memset(&param, 0, sizeof(param));
        param.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        param.parm.capture.timeperframe.numerator = 1;
        param.parm.capture.timeperframe.denominator = fps;

        if(ioctl(_fd, VIDIOC_S_PARM, &param) == -1)
        {
            LOG_WARN("io::LinuxCamera", "[w](setFps)[] Unable to change fps");
            return false;
        }

        if(param.parm.capture.timeperframe.numerator)
        {
            double newFps = param.parm.capture.timeperframe.denominator
                / param.parm.capture.timeperframe.numerator;
            if((double)fps != newFps)
            {
                LOG_WARN("io::LinuxCamera", "[w](setFps)[] Unsupported fps $0. New fps is $1", fps, newFps);
                return false;
            }
            //else
            //{
            //	LOG_DEBUG("io::LinuxCamera", "[w](setFps)[] New fps is $0 ($1/$2)", fps,
            //	param.parm.capture.timeperframe.denominator,
            //	param.parm.capture.timeperframe.numerator);
            //}
        }
        return true;
    }

    std::vector<Camera::FormatInfo> LinuxCamera::getAvailableFormats()
    {
        if(_availableFormats.size() > 0)
            return _availableFormats;

        struct v4l2_fmtdesc fmtEnum;
        memset(&(fmtEnum), 0, sizeof(fmtEnum));
        fmtEnum.index = 0;
        fmtEnum.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

        while(true)
        {
            // Request supported format
            int ret = ioctl(_fd, VIDIOC_ENUM_FMT, &fmtEnum);
            if(ret<0 && errno == EINVAL) break;
            if(ret<0)
            {
                LOG_WARN("io::LinuxCamera", "Could not enumerate available formats");
                return {};
            }

            Camera::FormatInfo fmtInfo;
            std::string desc((char*)fmtEnum.description);
            if(fmtEnum.pixelformat == V4L2_PIX_FMT_MJPEG)
            {
                fmtInfo.pixelFormat = PIXEL_FORMAT_MJPEG;
                fmtInfo.pixelFormatName = "MJPEG ("+desc+")";
            }
            else if(fmtEnum.pixelformat == V4L2_PIX_FMT_YUYV)
            {
                fmtInfo.pixelFormat = PIXEL_FORMAT_YUYV;
                fmtInfo.pixelFormatName = "YUYV ("+desc+")";
            }
            else
            {
                fmtInfo.pixelFormat = PIXEL_FORMAT_UNKNOWN;
                fmtInfo.pixelFormatName = "UNKNOWN ("+desc+")";
            }

            // Request resolutions
            struct v4l2_frmsizeenum frmSizeEnum;
            memset(&(frmSizeEnum), 0, sizeof(frmSizeEnum));
            frmSizeEnum.index = 0;
            frmSizeEnum.pixel_format = fmtEnum.pixelformat;
            while(true)
            {
                int ret = ioctl(_fd, VIDIOC_ENUM_FRAMESIZES, &frmSizeEnum);
                if(ret<0 && errno == EINVAL) break;
                if(ret<0)
                {
                    LOG_WARN("io::LinuxCamera", "Could not enumerate frame sizes");
                    return {};
                }

                if(frmSizeEnum.type == V4L2_FRMSIZE_TYPE_DISCRETE)
                {
                    fmtInfo.resolutions.push_back({frmSizeEnum.discrete.width, frmSizeEnum.discrete.height});
                    fmtInfo.fps.push_back(std::vector<unsigned>());

                    struct v4l2_frmivalenum frmIvalEnum;
                    memset(&(frmIvalEnum), 0, sizeof(frmIvalEnum));
                    frmIvalEnum.index = 0;
                    frmIvalEnum.pixel_format = fmtEnum.pixelformat;
                    frmIvalEnum.width = frmSizeEnum.discrete.width;
                    frmIvalEnum.height = frmSizeEnum.discrete.height;
                    while(true)
                    {
                        int ret = ioctl(_fd, VIDIOC_ENUM_FRAMEINTERVALS, &frmIvalEnum);
                        if(ret<0 && errno == EINVAL) break;
                        if(ret<0)
                        {
                            LOG_WARN("io::LinuxCamera", "Could not enumerate frame intervals");
                            return {};
                        }
                        if(frmIvalEnum.type == V4L2_FRMIVAL_TYPE_DISCRETE)
                            fmtInfo.fps.back().push_back((unsigned)(frmIvalEnum.discrete.denominator/(float)frmIvalEnum.discrete.numerator));

                        frmIvalEnum.index++;
                    }
                }

                frmSizeEnum.index++;
            }

            fmtEnum.index++;
            _availableFormats.push_back(fmtInfo);
        }

        return _availableFormats;
    }

    bool LinuxCamera::openDevice()
    {
        // Ignore if already open
        if(_fd != -1) return true;

        struct stat st;
        // Get information about the file
        if(stat(_deviceName.c_str(), &st) == -1)
        {
            LOG_WARN("io::LinuxCamera", "Can not identify '$0'. Error $1: $2", _deviceName, errno, strerror(errno));
            return false;
        }

        // Check if it is a device (video is a character device file)
        if(!S_ISCHR(st.st_mode))
        {
            LOG_WARN("io::LinuxCamera", "'$0' is not a device ($1)", _deviceName, errno);
            return false;
        }

        // Open file descriptor with read/write and
        _fd = open(_deviceName.c_str(), O_RDWR /* required */ | O_NONBLOCK, 0);
        if(_fd == -1)
        {
            LOG_WARN("io::LinuxCamera", "Could not open camera file '$0'. Error $1: $2",
                    _deviceName, errno, strerror(errno));
            return false;
        }
        return true;
    }

    bool LinuxCamera::initDevice()
    {
        struct v4l2_capability cap;
        struct v4l2_cropcap cropcap;
        struct v4l2_crop crop;
        struct v4l2_format fmt;

        //---------- Get device capabilities ----------//
        if(ioctl(_fd, VIDIOC_QUERYCAP, &cap) == -1)
        {
            LOG_WARN("io::LinuxCamera", "'$0' is not a V4L2 device", _deviceName);
            return false;
        }

        // Check if it is a device capable of video capturing
        if(!(cap.capabilities & V4L2_CAP_VIDEO_CAPTURE))
        {
            LOG_WARN("io::LinuxCamera", "'$0' is not a video capture device", _deviceName);
            return false;
        }

        //---------- Image format ----------//
        // Try to force image format
        memset(&(fmt), 0, sizeof(fmt));
        fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        fmt.fmt.pix.width = _resolution.width;
        fmt.fmt.pix.height = _resolution.height;

        if(_pixelFormat == PIXEL_FORMAT_MJPEG)
            fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_MJPEG;
        //else if(_pixelFormat == V4L2_PIX_FMT_YUYV)
        //    fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_MJPEG;
        else
            LOG_WARN("io::LinuxCamera", "Invalid pixel format");
        fmt.fmt.pix.field = V4L2_FIELD_NONE;

        if(ioctl(_fd, VIDIOC_S_FMT, &fmt) == -1)
        {
            LOG_WARN("io::LinuxCamera", "Could not set format, using default. Error $0: $1", errno, strerror(errno));
            // Start with default image format
            memset(&(fmt), 0, sizeof(fmt));
            fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
            if(ioctl(_fd, VIDIOC_G_FMT, &fmt) == -1)
            {
                V4L2_ERROR_MSG("VIDIOC_G_FMT");
                return false;
            }
        }

        //---------- Allocate image buffers (mmap) ----------//
        struct v4l2_requestbuffers req;
        memset(&(req), 0, sizeof(req));
        req.count = 4;
        req.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        req.memory = V4L2_MEMORY_MMAP;

        if(ioctl(_fd, VIDIOC_REQBUFS, &req) == -1)
        {
            if(EINVAL == errno)
            {
                LOG_WARN("io::LinuxCamera", "'$0' does not support memory mapping", _deviceName);
                return false;
            } 
            else if(errno == EBUSY)
            {
                LOG_WARN("io::LinuxCamera", "'$0' video device is already in use", _deviceName);
                return false;
            }
            else 
            {
                V4L2_ERROR_MSG("VIDIOC_REQBUFS");
                return false;
            }
        }

        if(req.count < 2)
        {
            LOG_WARN("io::LinuxCamera", "Insufficient buffer memory on '$0'", _deviceName);
            return false;
        }

        _buffers.resize(req.count);
        for(size_t i = 0; i < _buffers.size(); i++)	
        {
            struct v4l2_buffer buf;
            memset(&(buf), 0, sizeof(buf));
            buf.type        = V4L2_BUF_TYPE_VIDEO_CAPTURE;
            buf.memory      = V4L2_MEMORY_MMAP;
            buf.index       = i;

            if(ioctl(_fd, VIDIOC_QUERYBUF, &buf) == -1)
            {
                V4L2_ERROR_MSG("VIDIOC_QUERYBUF");
                return false;
            }

            _buffers[i].length = buf.length;
            _buffers[i].start =
                mmap(NULL /* start anywhere */,
                        buf.length,
                        PROT_READ | PROT_WRITE /* required */,
                        MAP_SHARED /* recommended */,
                        _fd, buf.m.offset);

            if(_buffers[i].start == MAP_FAILED)
            {
                LOG_WARN("io::LinuxCamera", "Failed to run mmap");
                return false;
            }
        }
        return true;
    }

    bool LinuxCamera::startCapturing()
    {
        // Enqueue buffer to receive images from driver
        struct v4l2_buffer buf;
        for(size_t i = 0; i<_buffers.size(); i++)
        {
            buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
            buf.memory = V4L2_MEMORY_MMAP;
            buf.index = i;

            if(ioctl(_fd, VIDIOC_QBUF, &buf) == -1)
            {
                V4L2_ERROR_MSG("VIDIOC_QBUF");
                return false;
            }
        }

        // Start producing video signal
        enum v4l2_buf_type type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
        if(ioctl(_fd, VIDIOC_STREAMON, &type) == -1)
        {
            V4L2_ERROR_MSG("VIDIOC_STREAMON");
            return false;
        }

        _capturing = true;

        return true;
    }
}
#endif// ATTA_OS_LINUX
