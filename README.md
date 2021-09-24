# Atta

[![Linux](https://github.com/Brenocq/Atta/actions/workflows/linux.yml/badge.svg)](https://github.com/Brenocq/Atta/actions/workflows/linux.yml)
[![MacOS](https://github.com/Brenocq/Atta/actions/workflows/macos.yml/badge.svg)](https://github.com/Brenocq/Atta/actions/workflows/macos.yml)
[![Windows](https://github.com/Brenocq/Atta/actions/workflows/windows.yml/badge.svg)](https://github.com/Brenocq/Atta/actions/workflows/windows.yml)
[![License: MIT](https://img.shields.io/badge/License-MIT-blue.svg)](LICENSE)

<p align="center">
 <img src="https://storage.googleapis.com/atta-images/evolution/2020-08-16.png" height="200">
 <img src="https://storage.googleapis.com/atta-images/evolution/2021-09-24.gif" height="200">
 <img src="https://storage.googleapis.com/atta-images/evolution/2021-09-08.gif" height="200">
</p>

Atta is a robot simulator for 2D and 3D applications. For now, the core is still under development.

This project is not yet well documented, I am creating a website in parallel to document the source code and project examples, but it is not yet available.

**Now I am refactoring the entire atta software architecture to be able to perform high-performance simulations and be easier to add new features (decoupling components). Please check the 0.0.1.0 branch if you want to test the previous version**

## Why Atta?
"Atta" comes from the scientific name of a type of leaf-cutting ant that can build nests of up to millions of individuals capable of working together to perform complex tasks.
This project aims to simulate complex systems like this, mainly composed of robots.

## Architecture
<p align="center">
 <img src="https://storage.googleapis.com/atta-images/main/arch-2021-09-08.png" height="600">
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

## License
This project is licensed under the MIT License - check [LICENSE](LICENSE) for details.
