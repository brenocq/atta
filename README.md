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

Atta is a robot simulator for 2D and 3D applications. For now, the core is still under development.

I am creating a website in parallel to document the source code and project examples, but it is not yet available.

## Features
- [ ] Doxygen documentation
- [ ] Build
  - [x] Linux
  - [x] Windows
  - [x] MacOS
  - [x] Browser
  - [ ] Android
- [ ] Component System
  - [x] Register custom components
  - [ ] Register custom Views
  - [x] Single entity operations
  - [x] Factory and clones
- [x] Event System
  - [x] Base events
  - [x] Publish
  - [x] Subscribe
- [ ] File System
  - [ ] Watchers
  	- [x] Linux file watcher
  	- [ ] Windows file watcher
  	- [ ] MacOS file watcher
  - [x] Serializers
  - [x] Project Serializer
  - [x] Open/Save/Close project
- [ ] Graphics System
  - [ ] Renderer APIs
    - [x] OpenGL
    - [ ] Vulkan
  - [ ] Renderers
    - [x] Fast Renderer
    - [x] Phong Renderer
    - [ ] PBR Renderer
    - [ ] RayTracing Renderer
  - [ ] Draw command list
- [ ] IO System
  - [ ] Camera
    - [x] Camera Linux
    - [ ] Camera Windows
    - [ ] Camera MacOS
  - [ ] Serial
    - [x] Serial Linux
    - [ ] Serial Windows
    - [ ] Serial MacOS
  - [ ] Socket
- [x] Memory System
  - [x] Allocators
    - [x] Pool allocator
    - [x] Bitmap pool allocator
    - [x] Stack allocator
    - [x] Malloc allocator
  - [x] Allocated object
- [ ] Physics System
  - [x] RigidBody2D
  - [ ] RigidBody3D
  - [ ] Colliders
  - [ ] Box2D
  - [ ] PhysX
- [x] Resource System
  - [x] Load models
  - [x] Load textures
- [ ] Script System
  - [ ] Compiler
  	- [x] Linux Compiler
  	- [ ] Windows Compiler
  	- [ ] Linux Compiler
  - [ ] Linker
  	- [x] Linux Linker
  	- [ ] Windows Linker
  	- [ ] MacOS Linker
  - [x] Hot reloading
- [ ] Sensor System
  - [x] RGB camera
  - [ ] RGBD camera
  - [ ] Lidar
  - [ ] Ultrasonic
  - [ ] Infrared
  - [ ] Touch sensor
  - [ ] IMU
  - [ ] Accelerometer
  - [ ] Gyroscope
  - [ ] Compass
- [ ] UI System
  - [ ] Plot multiple lines
  - [ ] Plot 2d points
  - [ ] Plot 3d points
  - [ ] Command list
- [ ] Algorithms
  - [ ] Computer Vision
  	- [ ] Fast
  	- [ ] Brief
  	- [ ] ORB
  	- [ ] Fundamental matrix
  	- [ ] Homography matrix
  	- [x] JPEG
  - [x] Linear Algebra
  	- [x] LU
  	- [x] QR
  	- [x] SVD
- [x] Core
  - [x] Math
    - [x] Bounds
    - [x] Ray
    - [x] Point
    - [x] Vector
    - [x] Matrix
    - [x] Quaternion
  - [x] Assert
  - [x] Logging
  - [x] StringId

## Why Atta?
"Atta" comes from the scientific name of a type of leaf-cutting ant that can build nests of up to millions of individuals capable of working together to perform complex tasks.
This project aims to simulate complex systems like this, mainly composed of robots.

## Architecture
<p align="center">
 <img src="https://storage.googleapis.com/atta-images/main/github_arch-2021-11-28.png" height="600">
</p>


## Build & test
Be sure to clone with the `--recursive` flag. If you accidentally cloned without this flag, please run `git submodule update --init --recursive` to clone the submodules.\
Atta should build without errors when the compiller supports C++17.\
If you found any errors when building, do not hesitate to create an issue.

```
$ git clone --recursive git@github.com:brenocq/atta.git
$ cd atta
$ mkdir build && cd build
$ cmake ..
$ make -j
$ ./bin/atta_test
$ ./bin/atta
```

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
