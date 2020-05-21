#ifndef SIMULATOR_H
#define SIMULATOR_H

#include <string>
#include <sstream>
#include "window.h"
#include "instance.h"
#include "surface.h"
#include "device.h"
#include "commandPool.h"
#include "swapChain.h"

class Simulator
{
	public:
	Simulator();
	~Simulator();

	void initWindow();
	void initInstance();
	void initSurface();
	void initDevice();
	void initSwapChain();
	void run();
	void drawFrame();

	private:
	VkPhysicalDevice choosePhysicalDevice();

	std::string getVersion(const uint32_t version);
	std::string getVersion(const uint32_t version, const uint32_t vendorId);
	void printPhysicalDevices();

	Window* _window;
	Instance* _instance;
	Surface* _surface;
	Device* _device;
	CommandPool* _commandPool;
	SwapChain* _swapChain;
};

#endif// SIMULATOR_H
