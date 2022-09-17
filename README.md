<div align="center">
  <h1>Atta</h1>
  <div>

[![Linux](https://github.com/Brenocq/Atta/actions/workflows/linux.yml/badge.svg)](https://github.com/Brenocq/Atta/actions/workflows/linux.yml)
[![MacOS](https://github.com/Brenocq/Atta/actions/workflows/macos.yml/badge.svg)](https://github.com/Brenocq/Atta/actions/workflows/macos.yml)
[![Windows](https://github.com/Brenocq/Atta/actions/workflows/windows.yml/badge.svg)](https://github.com/Brenocq/Atta/actions/workflows/windows.yml)
[![Web](https://github.com/brenocq/atta/actions/workflows/web.yml/badge.svg)](https://github.com/brenocq/atta/actions/workflows/web.yml)
[![License: MIT](https://img.shields.io/badge/License-MIT-blue.svg)](LICENSE)

  </div>

  <div>
    <a href="https://github.com/brenocq-atta/robotic-arm-ga"><img src="https://storage.googleapis.com/atta-images/docs/robotic-arm-ga/robotic-arm-ga.gif" height="200"></a>
    <a href="https://github.com/brenocq-atta/boids"><img src="https://storage.googleapis.com/atta-images/docs/boids/boids-basic.gif" height="200"></a>
    <a href="https://github.com/brenocq-atta/bee-hive-finding"><img src="https://storage.googleapis.com/atta-images/docs/bee-hive-finding/bee-hive-finding.gif" height="200"></a>
  </div>

  [Website](https://atta.brenocq.com) · [**Projects**](https://atta.brenocq.com/projects) · [Planning](https://github.com/users/brenocq/projects/14)
</div>


Atta is a robotics simulator, mainly focused on multi-robot systems with tons of similar robots. The objectives for this simulator are:
  - :books: Easy to get started
  - :shipit: Distribute processing across CPUs / GPUs / Clusters
  - :running: Fast development (with hot-reload)
  - :electric_plug: Interface between simulated and physical robots
  - :open_file_folder: Easy to share simulation with others ([published projects](https://atta.brenocq.com/projects))
  - :computer: Cross-platform (linux/macos/windows/web)

Check out the atta website for a [getting started guide](https://atta.brenocq.com/docs).

## Progress
Check the atta progress by **clicking on the buttons below**. [Issues](https://github.com/brenocq/atta/issues) with bugs and [discussions](https://github.com/brenocq/atta/discussions) with new ideas are very welcome :bowtie:.

<div align="center">

[![ComponentModule](https://storage.googleapis.com/atta-images/main/component_system_button_github_progress.png?)](https://github.com/brenocq/atta/projects/8)
[![EventModule](https://storage.googleapis.com/atta-images/main/event_system_button_github_progress.png?)](https://github.com/brenocq/atta/projects/13)
[![FileModule](https://storage.googleapis.com/atta-images/main/file_system_button_github_progress.png?)](https://github.com/brenocq/atta/projects/14)
[![GraphicsModule](https://storage.googleapis.com/atta-images/main/graphics_system_button_github_progress.png?)](https://github.com/brenocq/atta/projects/16)
[![IOModule](https://storage.googleapis.com/atta-images/main/io_system_button_github_progress.png?)](https://github.com/brenocq/atta/projects/17)
[![MemoryModule](https://storage.googleapis.com/atta-images/main/memory_system_button_github_progress.png?)](https://github.com/brenocq/atta/projects/15)
[![PhysicsModule](https://storage.googleapis.com/atta-images/main/physics_system_button_github_progress.png?)](https://github.com/brenocq/atta/projects/6)
[![ResourceModule](https://storage.googleapis.com/atta-images/main/resource_system_button_github_progress.png?)](https://github.com/brenocq/atta/projects/12)
[![ScriptModule](https://storage.googleapis.com/atta-images/main/script_system_button_github_progress.png?)](https://github.com/brenocq/atta/projects/11)
[![SensorModule](https://storage.googleapis.com/atta-images/main/sensor_system_button_github_progress.png?)](https://github.com/brenocq/atta/projects/10)
[![UIModule](https://storage.googleapis.com/atta-images/main/ui_system_button_github_progress.png?)](https://github.com/brenocq/atta/projects/9)
[![Docs](https://storage.googleapis.com/atta-images/main/docs_button_github_progress.png?)](https://github.com/brenocq/atta/projects/18)

</div>

_Obs: There is a github workflow to update the progress bar of each icon automatically heh_

## Architecture
Atta is composed of decoupled modules, the current modules are:

- **Component Module**: Manage entities and components
- **Event Module**: Publish and subscribe to atta internal events
- **File Module**: OS agnostic interface to serialize and deserialize data to files
- **Graphics Module**: Online and offline rendering with different levels of realism
- **IO Module**:  OS agnostic interface to peripherals (USB, socket, bluetooth, serial, ...)
- **Memory Module**: Manage atta internal memory to allow fast allocation and deallocation with minimum memory fragmentation
- **Physics Module**: Manage supported physics engines, simulate physics, and solve physics queries (collision, ray casting, ...)
- **Resources Module**: Load, save, and manage memory for resources like meshes, textures, video, ...
- **Script Module**: Compile and link user scripts automatically (hot reloading)
- **Sensor Module**: Update simulated/real sensors at each step
- **UI Module**: User interface graphics layer and UI rendering helpers

Arrows show dependencies between modules. Green boxes show which features are implemented.
<p align="center">
 <img src="https://storage.googleapis.com/atta-images/main/arch-2022-09-17.png" height="600">
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
Atta should build without errors when the **compiller supports C++17** (g++ >= 9.0).

If you found any errors, please do not hesitate to [create an issue](https://github.com/brenocq/atta/issues/new?assignees=brenocq&labels=fix&template=bug_report.md&title=) :wink:.

```bash
git clone git@github.com:brenocq/atta.git
cd atta
./scripts/build.sh --help
./scripts/build.sh
./build/release/bin/atta_test
./build/release/bin/atta
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
- [(Book) Introduction to MultiAgent Systems](https://www.amazon.com/Introduction-MultiAgent-Systems-Michael-Wooldridge/dp/0470519460)

## License
This project is licensed under the MIT License - check [LICENSE](LICENSE) for details.
