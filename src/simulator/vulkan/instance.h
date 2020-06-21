#ifndef INSTANCE_H
#define INSTANCE_H

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include <string.h>
#include "defines.h"

class Instance
{
	public:
	Instance();
	~Instance();

	private:
	void printExtensionSupport();
	bool checkValidationLayerSupport();

    VkInstance _instance;
};

#endif// INSTANCE_H
