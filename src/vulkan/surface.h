#ifndef SURFACE_H
#define SURFACE_H

#include "instance.h"

class Surface
{
	public:
	Surface(Instance* instance);
	~Surface();

	VkSurfaceKHR handle() const { return _surface; }
	Instance* instance() const { return _instance; }

	private:
	Instance* _instance;
	VkSurfaceKHR _surface;
};

#endif// SURFACE_H
