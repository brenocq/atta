#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "defines.h"
#include "vulkan/simulator.h"

int main() {

	Simulator* sim = new Simulator(); 
	sim->run();

	return EXIT_SUCCESS;
}
