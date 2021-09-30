//--------------------------------------------------
// Atta IO System
// linuxSerial.cpp
// Date: 2021-09-29
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifdef ATTA_OS_LINUX
#include <atta/ioSystem/serial/linuxSerial.h>
#include <fcntl.h>
#include <errno.h>
#include <cstring>
#include <unistd.h>

namespace atta::io
{
	LinuxSerial::LinuxSerial(Serial::CreateInfo info):
		Serial(info)
	{
		if(_timeout > 25.5f)
		{
			LOG_WARN("io::LinuxSerial", "Maximum linux serial timeout is 25.5 seconds, using 25.5 seconds as timeout");
			_timeout = 25.5f;
		}
	}

	LinuxSerial::~LinuxSerial()
	{
		close(_fd);
	}

	bool LinuxSerial::start()
	{
		//---------- Open ----------//
		// Open serial file descriptor
		_fd = open(_deviceName.c_str(), O_RDWR);
		if(_fd < 0)
		{
			LOG_WARN("io::LinuxSerial", "[*w](start)[] Could not open [w]$0[]. Error($1): $2", _deviceName, errno, std::strerror(errno));
			return false;
		}

		//---------- Configure ----------//
		// Initialize termios struct
		if(tcgetattr(_fd, &_tty) != 0)
		{
			LOG_WARN("io::LinuxSerial", "[*w](start)[] Could not initialize termios struct for [w]$0[]. Error($1): $2", _deviceName, errno, std::strerror(errno));
			return false;
		}

		//----- Control modes -----//
		// Enable/disable parity bit
		if(_parityBit)
			_tty.c_cflag |= PARENB;
		else
			_tty.c_cflag &= ~PARENB;

		// Enable/disable two stop bits (if disabled, only 1 stop bit)
		if(_twoStopBits)
			_tty.c_cflag |= CSTOPB;
		else
			_tty.c_cflag &= ~CSTOPB;

		// Set to use 8 bits as data length
		_tty.c_cflag &= ~CSIZE;
		_tty.c_cflag |= CS8;

		// Enable/disable RTS/CTS hardware flow control
		if(_rtsCts)
			_tty.c_cflag |= CRTSCTS;
		else
			_tty.c_cflag &= ~CRTSCTS;

		// Enable read and ignore ctrl lines
		_tty.c_cflag |= CREAD | CLOCAL;

		//----- Local modes -----//
		// Disable canonical mode to receive raw data
		_tty.c_lflag &= ~ICANON;

		// Disable echo
		_tty.c_lflag &= ~ECHO;
		_tty.c_lflag &= ~ECHOE;
		_tty.c_lflag &= ~ECHONL;

		// Disable signal chars
		_tty.c_lflag &= ~ISIG;

		//----- Input -----//
		// Disable software flow control
		_tty.c_iflag &= ~(IXON | IXOFF | IXANY);

		// Disable special handling of bytes
		_tty.c_iflag &= ~(IGNBRK|BRKINT|PARMRK|ISTRIP|INLCR|IGNCR|ICRNL);

		//----- Output -----//
		// Disable special handling of bytes
		_tty.c_oflag &= ~OPOST;
		_tty.c_oflag &= ~ONLCR;

		//----- Time -----//
		_tty.c_cc[VTIME] = uint8_t(_timeout*10);
		_tty.c_cc[VMIN] = 0;

		//----- Speed -----//
		// Also set termios struct
		if(!setBaudRate(_baudRate))
		{
			LOG_WARN("io::LinuxSerial", "[*w](start)[] Could not configure termios struct for [w]$0[]. Error($1): $2", _deviceName, errno, std::strerror(errno));
			return false;
		}

		LOG_SUCCESS("io::LinuxSerial", "[*w](start) [w]$0[] initialized", _deviceName);
		return true;
	}

	int LinuxSerial::receive(uint8_t* buf, uint16_t size)
	{
		return read(_fd, buf, size);
	}

	void LinuxSerial::transmit(uint8_t* buf, uint16_t size)
	{
		write(_fd, buf, size);
	}

	bool LinuxSerial::setBaudRate(unsigned baudRate)
	{
		switch(baudRate)
		{
			case 0: cfsetspeed(&_tty, B0); break;
			case 50: cfsetspeed(&_tty, B50); break;
			case 75: cfsetspeed(&_tty, B75); break;
			case 110: cfsetspeed(&_tty, B110); break;
			case 134: cfsetspeed(&_tty, B134); break;
			case 150: cfsetspeed(&_tty, B150); break;
			case 200: cfsetspeed(&_tty, B200); break;
			case 300: cfsetspeed(&_tty, B300); break;
			case 600: cfsetspeed(&_tty, B600); break;
			case 1200: cfsetspeed(&_tty, B1200); break;
			case 1800: cfsetspeed(&_tty, B1800); break;
			case 2400: cfsetspeed(&_tty, B2400); break;
			case 4800: cfsetspeed(&_tty, B4800); break;
			case 9600: cfsetspeed(&_tty, B9600); break;
			case 19200: cfsetspeed(&_tty, B19200); break;
			case 38400: cfsetspeed(&_tty, B38400); break;
			case 57600: cfsetspeed(&_tty, B57600); break;
			case 115200: cfsetspeed(&_tty, B115200); break;
			case 230400: cfsetspeed(&_tty, B230400); break;
			case 460800: cfsetspeed(&_tty, B460800); break;
			default:
				LOG_WARN("io::LinuxSerial", "[*w](setBaudRate)[] Invalid baudRate number $0", baudRate);
		}

		if(tcsetattr(_fd, TCSANOW, &_tty) != 0)
		{
			LOG_WARN("io::LinuxSerial", "[*w](setBaudRate)[] Could not configure termios struct for [w]$0[]. Error($1): $2", _deviceName, errno, std::strerror(errno));
			return false;
		}

		_baudRate = baudRate;
		return true;
	}
}

#endif// ATTA_OS_LINUX
