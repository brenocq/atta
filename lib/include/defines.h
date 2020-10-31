#ifndef DEFINES_H
#define DEFINES_H
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vector>

//---------------------------------//
//------- VULKAN PARAMETERS -------//
//---------------------------------//
#define ENABLE_VALIDATION_LAYERS true
#define ENABLE_USER_INTERFACE true// TODO
#define ENABLE_RAYTRACING true// TODO

const std::vector<const char*> validationLayers = {
    "VK_LAYER_KHRONOS_validation"
};
const std::vector<const char*> deviceExtensions = {
    VK_KHR_SWAPCHAIN_EXTENSION_NAME,
	VK_NV_RAY_TRACING_EXTENSION_NAME,
};
const std::vector<const char*> instanceExtensions = {
};


const int MAX_FRAMES_IN_FLIGHT = 2;


//---------------------------------//
//------ Terminal Color Code ------//
//---------------------------------//
// https://stackoverflow.com/questions/9158150/colored-output-in-c/9158263
// The following are UBUNTU/LINUX, and MacOS ONLY terminal color codes.
#define RESET   	"\033[0m"
#define BLACK   	"\033[0m\033[30m"      	/* Black */
#define RED     	"\033[0m\033[31m"      	/* Red */
#define GREEN   	"\033[0m\033[32m"      	/* Green */
#define YELLOW  	"\033[0m\033[33m"      	/* Yellow */
#define BLUE    	"\033[0m\033[34m"      	/* Blue */
#define MAGENTA 	"\033[0m\033[35m"      	/* Magenta */
#define CYAN    	"\033[0m\033[36m"      	/* Cyan */
#define WHITE   	"\033[0m\033[37m"      	/* White */
#define BOLDBLACK   "\033[1m\033[30m"      	/* Bold Black */
#define BOLDRED     "\033[1m\033[31m"      	/* Bold Red */
#define BOLDGREEN   "\033[1m\033[32m"      	/* Bold Green */
#define BOLDYELLOW  "\033[1m\033[33m"      	/* Bold Yellow */
#define BOLDBLUE    "\033[1m\033[34m"      	/* Bold Blue */
#define BOLDMAGENTA "\033[1m\033[35m"      	/* Bold Magenta */
#define BOLDCYAN    "\033[1m\033[36m"      	/* Bold Cyan */
#define BOLDWHITE   "\033[1m\033[37m"      	/* Bold White */

#endif// DEFINES_H
