# Atta - Robot Simulator
<p align="center">
 <img src="./img/2020-08-16.gif" height="200">
 <img src="./img/2020-09-12.png" height="200">
 <img src="./img/2020-12-04.gif" height="200">
</p>

Atta is a robot simulator for 2D and 3D applications. For now, the core is still under development, which includes:
 - **Ray Tracing:** My implementation of a physically based ray tracer based on the pbrt-v3 using the Vulkan khronos ray tracing extension for ray calculations. In the future I will also implement it on CPU.
 - **GUI:** The graphics user interface is being developed using a widget tree model (only basic widgets are finished).
 - **Physics Engine:** It is in its early stages. (I am debbuging it)
 - **Robotics:** After finishing the basics of the three above, I will start to implement the sensors/actuators/common algorithms.
 - **Examples:** There is an example folder where I am developing some exemples of simulations using the Atta.

Today only Linux is supported, but I plan to add support for Windows in the following months.\
This project is not yet well documented, after finishing some examples I am planning to create an website explaning its details and design decisions.

This week I am focusing on developing the GPU ray tracing renderer.

## Installation (Linux)
#### Download Vulkan SDK
First we need to download the vulkan SDK, I'm using the release 1.2.162.1.
You can download the Vulkan SDK [here](https://vulkan.lunarg.com/sdk/home).

Extract the files to some folder.
``` shell
tar -xzf vulkansdk-linux-x86_64-xxx.tar.gz
```

#### Installing
```shell
git clone https://github.com/Brenocq/RobotSimulator.git

# I recommend adding the two lines below to your .bashrc (you can also source/export manually every time)
# source <yourPathToVulkanFolder>/setup-env.sh
# export Vulkan_INCLUDE_DIR="<yourPathToVulkanFolder>/x86_64/include"

./install_linux.sh
```

## Running
```shell
./run.sh # Release
./debug.sh # Debug with gdb
```

## Progress
These values are unreliable and come from what I think is going on in the code (things can potentially change as new ideas emerge).

#### Core
 - Vulkan specific: 100%
 - Renderers
	 - RayTracing (GPU/vulkan): 35%
	 - RayTracing (CPU): 5%
	 - Rasterization: 50%
	 - 2D: 60%
 - GUI: 20%
 - Physics engine: 60%
 - Multithreading: 30%

## References
- [(Book) Physically Based Rendering](http://www.pbr-book.org/)
- [(Book) Game Physics Engine Development](https://www.amazon.com/Game-Physics-Engine-Development-Commercial-Grade/dp/0123819768)
- [(Book) Real-Time Collision Detection](https://www.amazon.com/Real-Time-Collision-Detection-Interactive-Technology/dp/1558607323)
- [(Book) Computer Graphics Principles and Practice](http://cgpp.net/about.xml)
- [(Repo) GPSnoopy's Ray Tracing implementation](https://github.com/GPSnoopy/RayTracingInVulkan)

## License
This project is licensed under the MIT License - check [LICENSE](LICENSE) for details.
