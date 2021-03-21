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
}
#endif// ATTA_CORE_COMMON_H
