//--------------------------------------------------
// Atta Peripheral
// camera.cpp
// Date: 2021-06-16
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/peripheral/camera.h>
#include <atta/helpers/log.h>

#include <errno.h>
#include <cstring>
//#include <sys/types.h>
#include <sys/stat.h>// Check device file
#include <fcntl.h>// Low level file i/o
#include <sys/ioctl.h>// i/o control
#include <sys/mman.h>
#include <unistd.h>
#include <linux/videodev2.h>

namespace atta::peripheral
{
	Camera::Camera(CreateInfo info):
		_deviceName(info.deviceName),
		_pixelFormat(info.pixelFormat), _resolution(info.resolution), _fps(info.fps)
	{
		openDevice();
	}

	Camera::~Camera()
	{
		// Unmap buffers
		for(size_t i = 0; i<_buffers.size(); i++)
			if(munmap(_buffers[i].start, _buffers[i].length) == -1)
				Log::warning("peripheral::Camera", "Could not unmap buffer");

		// Close camera file descriptor
        if(close(_fd) == -1)
			Log::warning("peripheral::Camera", "Could not close camera file descriptor");

        _fd = -1;
	}

	void Camera::start()
	{
		initDevice();
		startCapturing();
		Log::info("peripheral::Camera", "Camera $0 initialized", _deviceName);
	}

	bool Camera::setFormat(PixelFormat pixelFormat, Resolution resolution)
	{
		struct v4l2_format fmt;
		memset(&(fmt), 0, sizeof(fmt));
		fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
		fmt.fmt.pix.width = resolution.width;
		fmt.fmt.pix.height = resolution.height;

		if(pixelFormat == PIXEL_FORMAT_MJPEG)
			fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_MJPEG;
		else if(pixelFormat == V4L2_PIX_FMT_YUYV)
			fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_MJPEG;
		else
		{
			Log::warning("peripheral::Camera", "[w](setFormat)[] Invalid pixel format");
			return false;
		}
		fmt.fmt.pix.field = V4L2_FIELD_NONE;

		// Use default image format
		if(ioctl(_fd, VIDIOC_S_FMT, &fmt) == -1)
		{
			Log::warning("peripheral::Camera", "[w](setFormat)[] Could not set new format");
			return false;
		}

		_pixelFormat = pixelFormat;
		_resolution = resolution;

		return true;
	}

	bool Camera::setFps(unsigned fps)
	{
		struct v4l2_streamparm param;
		memset(&param, 0, sizeof(param));
		param.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
		param.parm.capture.timeperframe.numerator = 1;
		param.parm.capture.timeperframe.denominator = fps;  

		if(ioctl(_fd, VIDIOC_S_PARM, &param) == -1)
		{
			Log::warning("peripheral::Camera", "[w](setFps)[] Unable to change fps");
			return false;
		}

		if(param.parm.capture.timeperframe.numerator)
		{
			double newFps = param.parm.capture.timeperframe.denominator
						 / param.parm.capture.timeperframe.numerator;
			if((double)fps != newFps)
			{
				Log::warning("peripheral::Camera", "[w](setFps)[] Unsupported fps $0. New fps is $1", fps, newFps);
				return false;
			}
			//else
			//{
			//	Log::debug("peripheral::Camera", "[w](setFps)[] New fps is $0 ($1/$2)", fps, 
			//	param.parm.capture.timeperframe.denominator,
			//	param.parm.capture.timeperframe.numerator);
			//}
		}
		return true;
	}

	std::vector<Camera::FormatInfo> Camera::getAvailableFormats()
	{
		struct v4l2_fmtdesc fmtEnum;
		memset(&(fmtEnum), 0, sizeof(fmtEnum));
		fmtEnum.index = 0;
		fmtEnum.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;

		std::vector<Camera::FormatInfo> availableFormats;

		while(true)
		{
			// Request supported format
			int ret = ioctl(_fd, VIDIOC_ENUM_FMT, &fmtEnum);
			if(ret<0 && errno == EINVAL) break;
			if(ret<0)
			{
				Log::error("peripheral::Camera", "Could not enumerate available formats");
				exit(1);
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
					Log::error("peripheral::Camera", "Could not enumerate frame sizes");
					exit(1);
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
							Log::error("peripheral::Camera", "Could not enumerate frame intervals");
							exit(1);
						}
						if(frmIvalEnum.type == V4L2_FRMIVAL_TYPE_DISCRETE)
							fmtInfo.fps.back().push_back((unsigned)(frmIvalEnum.discrete.denominator/(float)frmIvalEnum.discrete.numerator));

						frmIvalEnum.index++;
					}
				}

				frmSizeEnum.index++;
			}

			fmtEnum.index++;
			availableFormats.push_back(fmtInfo);
		}

		return availableFormats;
	}

	void Camera::openDevice()
	{
        struct stat st;
		// Get information about the file
        if(stat(_deviceName.c_str(), &st) == -1)
		{
			Log::warning("peripheral::Camera", "Cannot identify '$0' ($1)",
				_deviceName, errno);
            //exit(1);
        }

		// Check if it is a device (video is a character device file)
        if(!S_ISCHR(st.st_mode))
		{
			Log::error("peripheral::Camera", "'$0' is not a device ($1)",
				_deviceName, errno);
            exit(1);
        }

		// Open file descriptor with read/write and 
        _fd = open(_deviceName.c_str(), O_RDWR /* required */ | O_NONBLOCK, 0);
        if(_fd == -1)
		{
			Log::error("peripheral::Camera", "Could not open camera file '$0' ($1)",
				_deviceName, errno);
            exit(1);
        }
	}

	void Camera::initDevice()
	{
        struct v4l2_capability cap;
        struct v4l2_cropcap cropcap;
        struct v4l2_crop crop;
        struct v4l2_format fmt;

		//---------- Get device capabilities ----------//
        if(ioctl(_fd, VIDIOC_QUERYCAP, &cap) == -1)
		{
			Log::error("peripheral::Camera", "'$0' is not a V4L2 device", _deviceName);
			exit(1);
        }

		// Check if it is a device capable of video capturing
        if(!(cap.capabilities & V4L2_CAP_VIDEO_CAPTURE))
		{
			Log::error("peripheral::Camera", "'$0' is not a video capture device", _deviceName);
			exit(1);
        }

		//---------- Image format ----------//
		memset(&(fmt), 0, sizeof(fmt));
		fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
		fmt.fmt.pix.width = _resolution.width;
		fmt.fmt.pix.height = _resolution.height;

		if(_pixelFormat == PIXEL_FORMAT_MJPEG)
			fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_MJPEG;
		else if(_pixelFormat == V4L2_PIX_FMT_YUYV)
			fmt.fmt.pix.pixelformat = V4L2_PIX_FMT_MJPEG;
		else
			Log::warning("peripheral::Camera", "Invalid pixel format");

		fmt.fmt.pix.field = V4L2_FIELD_NONE;

		if(ioctl(_fd, VIDIOC_S_FMT, &fmt) == -1)
			Log::warning("peripheral::Camera", "Could not set format, using default");

		memset(&(fmt), 0, sizeof(fmt));
        fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
		// Use default image format
		if(ioctl(_fd, VIDIOC_G_FMT, &fmt) == -1)
		{
			Log::error("peripheral::Camera", "Could not get format");
			exit(1);
		}

		//Log::debug("peripheral::Camera", "Allocating width:$0 height:$1 total:$2", 
		//	fmt.fmt.pix.width, fmt.fmt.pix.height, fmt.fmt.pix.sizeimage);

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
				Log::error("peripheral::Camera", "'$0' does not support memory mapping", _deviceName);
				exit(1);
			} 
			else 
			{
				Log::error("peripheral::Camera", "Failed to run VOIDC_REQBUFS");
				exit(1);
			}
        }

        if(req.count<2)
		{
			Log::error("peripheral::Camera", "Insufficient buffer memory on '$0'", _deviceName);
			exit(1);
        }

		//Log::debug("peripheral::Camera", "We will allocate $0", req.count);

		_buffers.resize(req.count);
		for(size_t i=0; i<_buffers.size(); i++)	
		{
			struct v4l2_buffer buf;
			memset(&(buf), 0, sizeof(buf));
			buf.type        = V4L2_BUF_TYPE_VIDEO_CAPTURE;
			buf.memory      = V4L2_MEMORY_MMAP;
			buf.index       = i;

			if(ioctl(_fd, VIDIOC_QUERYBUF, &buf) == -1)
			{
				Log::error("peripheral::Camera", "Failed to run VIDIOC_QUERYBUF");
				exit(1);
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
				Log::error("peripheral::Camera", "Failed to run mmap");
				exit(1);
			}
		}
	}

	void Camera::startCapturing()
	{
		// Enqueue buffer to receive images from driver
		for(size_t i = 0; i<_buffers.size(); i++)
		{
			struct v4l2_buffer buf;
			memset(&(buf), 0, sizeof(buf));
			buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
			buf.memory = V4L2_MEMORY_MMAP;
			buf.index = i;

			if(ioctl(_fd, VIDIOC_QBUF, &buf) == -1)
			{
				Log::error("peripheral::Camera", "Failed to run VIDIOC_QBUF");
				exit(1);
			}
		}

		// Start producing video signal
        enum v4l2_buf_type type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
		if(ioctl(_fd, VIDIOC_STREAMON, &type) == -1)
		{
			Log::error("peripheral::Camera", "Failed to run VIDIOC_STREAMON");
			exit(1);
		}
	}

	void Camera::readFrame()
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
			Log::warning("peripheral::Camera", "Error when trying to receive camera frame");
			return;
		}

		if(r == 0)
		{
			Log::warning("peripheral::Camera", "Timeout when trying to receive camera frame");
			return;
		}

		// There is something to read in the camera fd, reading frame now
        struct v4l2_buffer buf;
		memset(&(buf), 0, sizeof(buf));
		buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
		buf.memory = V4L2_MEMORY_MMAP;

		if(ioctl(_fd, VIDIOC_DQBUF, &buf) == -1)
		{
			switch (errno) {
			case EAGAIN:
				Log::warning("peripheral::Camera", "Failed to run VIDIOC_DQBUF (EAGAIN)");
				return;
			case EIO:
					/* Could ignore EIO, see spec. */
					/* fall through */
			default:
				Log::warning("peripheral::Camera", "Failed to run VIDIOC_DQBUF");
				return;
			}
		}

		if(buf.index >= _buffers.size())
			Log::warning("peripheral::Camera", "Invalid buffer index when trying to receive camera frame");

		uint8_t* start = (uint8_t*)_buffers[buf.index].start;
		size_t size = buf.bytesused;
		_image = std::vector<uint8_t>(start, start+size);

		if(ioctl(_fd, VIDIOC_QBUF, &buf) == -1)
		{
			Log::warning("peripheral::Camera", "Failed to run VIDIOC_QBUF");
			return;
		}
	}
}
