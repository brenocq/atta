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
- ✨ The cards below are automatically updated to reflect the [Issues](https://github.com/brenocq/atta/issues).
- 💡 Click on a card to explore the task progress
- 🤝 Your reactions guide development! Add a ❤️ to your favorite features.

<div align="center">
  <a href="https://github.com/brenocq/atta/issues?q=is%3Aissue%20label%3Astatus%3Aidea"><img src="https://github.com/brenocq/atta/blob/generated-svgs/status_idea.svg"/></a>
  <a href="https://github.com/brenocq/atta/issues?q=is%3Aissue%20label%3Astatus%3Atodo"><img src="https://github.com/brenocq/atta/blob/generated-svgs/status_todo.svg"/></a>
  <a href="https://github.com/brenocq/atta/issues?q=is%3Aissue%20label%3Astatus%3Adoing"><img src="https://github.com/brenocq/atta/blob/generated-svgs/status_doing.svg"/></a>
  <a href="https://github.com/brenocq/atta/issues?q=is%3Aissue%20label%3Astatus%3Areview"><img src="https://github.com/brenocq/atta/blob/generated-svgs/status_review.svg"/></a>
  <a href="https://github.com/brenocq/atta/issues?q=is%3Aissue%20label%3Astatus%3Adone"><img src="https://github.com/brenocq/atta/blob/generated-svgs/status_done.svg"/></a>
</div>

<div align="center">
  <a href="https://atta.brenocq.com/issue_0"><img src="https://github.com/brenocq/atta/blob/generated-svgs/issue_0.svg"/></a>
</div>
<div align="center">
  <a href="https://atta.brenocq.com/issue_1"><img src="https://github.com/brenocq/atta/blob/generated-svgs/issue_1.svg"/></a>
</div>
<div align="center">
  <a href="https://atta.brenocq.com/issue_2"><img src="https://github.com/brenocq/atta/blob/generated-svgs/issue_2.svg"/></a>
</div>
<div align="center">
  <a href="https://atta.brenocq.com/issue_3"><img src="https://github.com/brenocq/atta/blob/generated-svgs/issue_3.svg"/></a>
</div>
<div align="center">
  <a href="https://atta.brenocq.com/issue_4"><img src="https://github.com/brenocq/atta/blob/generated-svgs/issue_4.svg"/></a>
</div>
<div align="center">
  <a href="https://atta.brenocq.com/issue_5"><img src="https://github.com/brenocq/atta/blob/generated-svgs/issue_5.svg"/></a>
</div>
<div align="center">
  <a href="https://github.com/brenocq/atta/issues"><img src="https://github.com/user-attachments/assets/e3deab96-be62-42b4-886b-a8509ffe761a"/></a>
</div>

_If you are interested in knowing how these cards were made, check [#63](https://github.com/brenocq/atta/issues/63)_ 🙂

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

<!------------ Windows ------------>
<details><summary> Windows </summary>
<h4>Dependencies</h4>
To build atta properly, you need to have cmake installed.
<pre><code>choco install cmake</code></pre>
Also, be sure that your <strong>compiller supports C++17</strong> (g++ >= 9.0).

<h4>Run</h4>
<pre><code>git clone git@github.com:brenocq/atta.git
cd atta
mkdir build
cd build
cmake ..
</code></pre>

You can now use Visual Studio to open the <code>atta.sln</code> file.
</details>
<!------------ MacOS ------------>
<details><summary> MacOS </summary>
<h4>Dependencies</h4>
To build atta properly, you need to have cmake installed.
<pre><code>brew install cmake</code></pre>
Also, be sure that your <strong>compiller supports C++17</strong> (g++ >= 9.0).

<h4>Run</h4>
<pre><code>git clone git@github.com:brenocq/atta.git
cd atta
./scripts/build.sh --help
./scripts/build.sh
./build/release/bin/atta_test
./build/release/bin/atta
</code></pre>
</details>
<!------------ Linux ------------>
<details><summary> Linux </summary>
<h3>Dependencies</h3>
To build atta, you need:
<ul>
  <li>g++ >= 9.0</li>
  <li>cmake >= 3.14</li>
</ul>

<strong>Ubuntu:</strong>
<pre><code>sudo apt-get install g++ cmake git xorg-dev curl</code></pre>
<i>Note: If your ubuntu is old, you may need to install the latest cmake/g++ manually.</i>

<strong>Fedora:</strong>
<pre><code>sudo yum install g++ cmake git glfw-devel curl</code></pre>

<strong>Arch:</strong>
<pre><code>sudo pacman -Sy g++ cmake git glfw-x11 curl</code></pre>

<h3>Run</h3>
<pre><code>git clone git@github.com:brenocq/atta.git
cd atta
./build.sh --help
./build.sh
./build/release/bin/atta_test
./build/release/bin/atta
</code></pre>
</details>

If you found any errors, please do not hesitate to [create an issue](https://github.com/brenocq/atta/issues/new?assignees=brenocq&labels=fix&template=bug_report.md&title=) :wink:.

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
