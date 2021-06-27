//--------------------------------------------------
// Atta Robot Simulator
// common.h
// Date: 2021-03-02
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_CORE_COMMON_H
#define ATTA_CORE_COMMON_H
namespace atta
{
	enum DimMode 
	{
		DIM_MODE_2D = 0,
		DIM_MODE_3D
	};

	enum GuiRenderer 
	{
		GUI_RENDERER_RAST = 0,
		GUI_RENDERER_RAY_TRACING_VULKAN,
		GUI_RENDERER_RAY_TRACING_CPU,
		GUI_RENDERER_2D,
	};

	enum PhysicsMode 
	{
		PHY_MODE_DISABLED = 0,
		PHY_MODE_2D,
		PHY_MODE_3D
	};

	enum RobotProcessing 
	{
		ROBOT_PROCESSING_SEQUENTIAL = 0,
		ROBOT_PROCESSING_PARALLEL_CPU,
		ROBOT_PROCESSING_PARALLEL_GPU
	};

	// Keyboard handler
	enum KeyboardAction
	{
		KEY_RELEASE = 0,
		KEY_PRESS = 1,
		KEY_REPEAT = 2,
	};

	enum KeyboardKey
	{
		KEY_A = 'A',
		KEY_B,
		KEY_C,
		KEY_D,
		KEY_E,
		KEY_F,
		KEY_G,
		KEY_H,
		KEY_I,
		KEY_J,
		KEY_K,
		KEY_L,
		KEY_M,
		KEY_N,
		KEY_O,
		KEY_P,
		KEY_Q,
		KEY_R,
		KEY_S,
		KEY_T,
		KEY_U,
		KEY_V,
		KEY_W,
		KEY_X,
		KEY_Y,
		KEY_Z,
	};

	// Mouse handler
	enum MouseAction
	{
		MOUSE_RELEASE = 0,
		MOUSE_PRESS
	};  

	enum MouseButton
	{
		MOUSE_LEFT = 0,
		MOUSE_RIGHT,
		MOUSE_MIDDLE
	};
}
#endif// ATTA_CORE_COMMON_H
