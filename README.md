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

Atta is a robot engine mainly focused on multi-robot systems with tons of similar robots. The objectives for this simulator are:
  - Fast development (hot-reload)
  - Easy to get started
  - Distribute processing across CPUs / GPUs / Clusters
  - Interface between simulated and physical robots
  - Cross-platform (linux/macos/windows/web/android(?))
  - Link to test published projects online ([example](https://atta.brenocq.com/projects/brenocq-atta/tutorial))

Check out the atta website for a [getting started guide](https://atta.brenocq.com/docs). You can also test published projects online [here](https://atta.brenocq.com/build).

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

_Obs: There is a github workflow to update the progress bar of each icon automatically heh_

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
To build atta properly, you need to have cmake installed.
Dependencies for some operating systems:

**Windows:**
```bash
choco install cmake
```

**MacOS:**
```bash
brew install cmake
```

**Linux:**
```bash
sudo apt-get install cmake xorg-dev curl
```

#### Clone
This step is operating system indepentent. Be sure to have your **github ssh key properly configured**. Atta should build without errors when the **compiller supports C++17** (g++ >= 9.0).

If you found any errors, please do not hesitate to [create an issue](https://github.com/brenocq/atta/issues/new?assignees=brenocq&labels=fix&template=bug_report.md&title=).

```bash
git clone git@github.com:brenocq/atta.git
cd atta
./scripts/build.sh --help
./scripts/build.sh
./bin/atta_test
./bin/atta
```

_Obs: The build script should help the user with dependencies. If you found ploblems please let me know_

## Discussions
If you want to contribute, have ideas, or have questions about atta, feel free to [start a discussion](https://github.com/brenocq/atta/discussions).

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
