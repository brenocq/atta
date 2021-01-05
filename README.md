# Atta - Robot Simulator
<p align="center">
 <img src="./img/2020-08-16.gif" height="200">
 <img src="./img/2020-09-12.png" height="200">
 <img src="./img/2020-12-04.gif" height="200">
</p>

Atta is a robot simulator for 2D and 3D applications made with Vulkan (optional Ray Tracing for Nvidia GPUs). Now I am developing mainly the physics engine and CPU ray tracing renderer.
This simulator also has an implementation of a user interface library based on a widget tree, but it is in its early stages.
After finishing the core (renderers, gui library, physics engine), I will start to implement specific robotic simulation code, such as motors, common sensors, common algorithms.
For now, only Linux is supported, but I plan to add support for Windows in the following months.

## Installation (Linux)
#### Download Vulkan SDK
First we need to download the vulkan SDK, I'm using the release 1.2.154.0.
You can download the vulkan SDK [here](https://vulkan.lunarg.com/sdk/home).

Extract the files to some folder.
``` shell
tar -xzf vulkansdk-linux-x86_64-xxx.tar.gz
```

#### Installing
```shell
git clone https://github.com/Brenocq/RobotSimulator.git
# [!!] Change Atta/run.sh and Atta/debug.sh vulkan source to your path 
# [!!] Change Atta/CMakeLists.txt vulkan path to your path
./install.sh
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
 - RayTracing (Vulkan): 85%
 - RayTracing (CPU): 5%
 - Graphics user interface: 20%
 - Physics engine: 10%
 - Threading: 10%

## References
- [Alexander Overvoorde's Vulkan Tutorial](https://vulkan-tutorial.com/)
- [Vulkan documentation](https://www.khronos.org/registry/vulkan/specs/1.2-extensions/html/index.html)
- [GPSnoopy's Ray Tracing implementation](https://github.com/GPSnoopy/RayTracingInVulkan)
- [Nvidia Vulkan Ray Tracing Tutorial](https://nvpro-samples.github.io/vk_raytracing_tutorial_KHR/)
- [Game Physics Engine Development](https://www.amazon.com/Game-Physics-Engine-Development-Commercial-Grade/dp/0123819768)
- [Real-Time Collision Detection](https://www.amazon.com/Real-Time-Collision-Detection-Interactive-Technology/dp/1558607323)
- [Physically Based Rendering](http://www.pbr-book.org/)

## License
This project is licensed under the MIT License - check [LICENSE](LICENSE) for details.
