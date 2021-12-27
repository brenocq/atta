# Atta

[![Linux](https://github.com/Brenocq/Atta/actions/workflows/linux.yml/badge.svg)](https://github.com/Brenocq/Atta/actions/workflows/linux.yml)
[![MacOS](https://github.com/Brenocq/Atta/actions/workflows/macos.yml/badge.svg)](https://github.com/Brenocq/Atta/actions/workflows/macos.yml)
[![Windows](https://github.com/Brenocq/Atta/actions/workflows/windows.yml/badge.svg)](https://github.com/Brenocq/Atta/actions/workflows/windows.yml)
[![Web](https://github.com/brenocq/atta/actions/workflows/web.yml/badge.svg)](https://github.com/brenocq/atta/actions/workflows/web.yml)
[![License: MIT](https://img.shields.io/badge/License-MIT-blue.svg)](LICENSE)

<p align="center">
 <img src="https://storage.googleapis.com/atta-images/evolution/2021-09-24.gif" height="200">
 <img src="https://storage.googleapis.com/atta-images/evolution/2021-09-08.gif" height="200">
</p>

Atta is a robot simulator for 2D and 3D applications mainly focused on multi-robot systems with tons of similar robots. The objectives for this simulator are:
  - Fast to develop new simulations
  - Easy to understand
  - Distribute the processing across CPUs / GPUs / Clusters
  - Interface between simulated and physical robots
  - Cross-platform (including web support to easily share the simulations)

Check out the atta website for a [getting started guide](https://atta.brenocq.com/docs). You can also test the web build online [here](https://atta.brenocq.com/build).

## Progress
Check the atta progress by **clicking on the buttons below**. [Issues](https://github.com/brenocq/atta/issues) with bugs and [discussions](https://github.com/brenocq/atta/discussions) with new ideas are very welcome :)

[![ComponentSystem](https://storage.googleapis.com/atta-images/main/component_system_button_github_progress.png?)](https://github.com/brenocq/atta/projects/8)
[![EventSystem](https://storage.googleapis.com/atta-images/main/event_system_button_github_progress.png?)](https://github.com/brenocq/atta/projects/13)
[![FileSystem](https://storage.googleapis.com/atta-images/main/file_system_button_github_progress.png?)](https://github.com/brenocq/atta/projects/14)
[![GraphicsSystem](https://storage.googleapis.com/atta-images/main/graphics_system_button_github_progress.png?)](https://github.com/brenocq/atta/projects/16)
[![IOSystem](https://storage.googleapis.com/atta-images/main/io_system_button_github_progress.png?)](https://github.com/brenocq/atta/projects/17)
[![MemorySystem](https://storage.googleapis.com/atta-images/main/memory_system_button_github_progress.png?)](https://github.com/brenocq/atta/projects/15)
[![PhysicsSystem](https://storage.googleapis.com/atta-images/main/physics_system_button_github_progress.png?)](https://github.com/brenocq/atta/projects/6)
[![ResourceSystem](https://storage.googleapis.com/atta-images/main/resource_system_button_github_progress.png?)](https://github.com/brenocq/atta/projects/12)
[![ScriptSystem](https://storage.googleapis.com/atta-images/main/script_system_button_github_progress.png?)](https://github.com/brenocq/atta/projects/11)
[![SensorSystem](https://storage.googleapis.com/atta-images/main/sensor_system_button_github_progress.png?)](https://github.com/brenocq/atta/projects/10)
[![UISystem](https://storage.googleapis.com/atta-images/main/ui_system_button_github_progress.png?)](https://github.com/brenocq/atta/projects/9)
[![Docs](https://storage.googleapis.com/atta-images/main/docs_button_github_progress.png?)](https://github.com/brenocq/atta/projects/18)

## Architecture
Atta is composed of decoupled system, the current systems are:

- **Component System**: Manage entities and components
- **Event System**: Publish and subscribe to atta internal events
- **File System**: OS agnostic interface to serialize and deserialize data to files
- **Graphics System**: Online and offline rendering with different levels of realism
- **IO System**:  OS agnostic interface to peripherals (USB, socket, bluetooth, serial, ...)
- **Memory System**: Manage atta internal memory to allow fast allocation and deallocation with minimum memory fragmentation
- **Physics System**: Manage supported physics engines, simulate physics, and solve physics queries (collision, ray casting, ...)
- **Resources System**: Load, save, and manage memory for resources like meshes, textures, video, ...
- **Script System**: Compile and link user scripts automatically (hot reloading)
- **Sensor System**: Update simulated/real sensors at each step
- **UI System**: User interface graphics layer and UI rendering helpers

Arrows show dependencies between systems. Green boxes show which systems/features are implemented.
<p align="center">
 <img src="https://storage.googleapis.com/atta-images/main/github_arch-2021-11-28.png" height="600">
</p>

## Why Atta?
"Atta" comes from the scientific name of a type of leaf-cutting ant that can build nests of up to millions of individuals capable of working together to perform complex tasks.
This project aims to simulate complex systems like this, mainly composed of robots.

## Build & test
#### Dependencies
To build atta properly, you need to have cmake installed and glfw dependencies (necessary to create windows).

Dependencies for some operating systems:

**Windows:**
```bash
> choco install cmake
> vcpkg install cpprestsdk cpprestsdk:x64-windows
```

**MacOS:**
```bash
$ brew install cmake ninja boost openssl 
```

**Ubuntu:**
```bash
$ sudo apt-get install cmake ninja-build xorg-dev openssl libssl-dev libboost-all-dev
```

#### Clone
This step is operating system indepentent. Be sure to have your **github ssh key properly configured**, you will be asked to insert the password many times. Be sure to **clone with the `--recursive` flag**. If you accidentally cloned without this flag, please run `git submodule update --init --recursive` to clone the submodules.

Atta should build without errors when the compiller supports C++17.
If you found any errors, please do not hesitate to create an issue.

```
$ git clone --recursive git@github.com:brenocq/atta.git
$ cd atta
$ mkdir build && cd build
$ cmake ..
$ make -j
$ ./bin/atta_test
$ ./bin/atta
```

## Discussions
If you want to contribute, have ideias, or have questions about atta, feel free to [start a discussion](https://github.com/brenocq/atta/discussions).

## References
- [(Book) Physically Based Rendering](http://www.pbr-book.org/)
- [(Book) Game Physics Engine Development](https://www.amazon.com/Game-Physics-Engine-Development-Commercial-Grade/dp/0123819768)
- [(Book) Real-Time Collision Detection](https://www.amazon.com/Real-Time-Collision-Detection-Interactive-Technology/dp/1558607323)
- [(Book) Computer Graphics Principles and Practice](http://cgpp.net/about.xml)
- [(Book) Programming Massively Parallel Processors](https://www.amazon.com/Programming-Massively-Parallel-Processors-Hands/dp/0128119861)
- [(Book) Numerical Recipes: The Art of Scientific Computing](http://numerical.recipes/com/storefront.html)
- [(Book) Game Programming Patterns](https://gameprogrammingpatterns.com)
- [(Book) Game Engine Architecture](https://www.gameenginebook.com)
- [(Book) C++ Templates: The Complete Guide](http://www.tmplbook.com)

## License
This project is licensed under the MIT License - check [LICENSE](LICENSE) for details.
