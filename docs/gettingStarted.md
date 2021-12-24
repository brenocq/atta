# Getting Started
Atta is a simulator focused on simulating multi-robot systems, but it can also be used to simulate other types of systems and interface with physical hardware. If you haven't installed atta on your machine yet, follow the instructions on the [installation page](installation.md).

## Overview
When you open atta for the first time, we will find this screen.

You will have your viewports at the middle, the logging window at the bottom, and scene tree at the right, and the top bar at the top.

## Projects
Projects are used to organize source files, 3D models, and other files your simulation might need.

### Creating your first project
To create you first project with atta, go to `File > Save as` and save the `.atta` file to your project directory.

// TODO: Gif saving the first project

After saving you should have a file structure like:

```
MyProject
 └─ myProject.atta
```

As projects grow in complexity, they usually have a structure like

```
MyProject
 ├─ .git/
 ├─ build/
 │   └─ ...
 ├─ resources/
 │   ├─ robotArm.obj
 │   ├─ robotArm.mtl
 │   ├─ robotArm.png
 │   ├─ robotBody.obj
 │   ├─ robotBody.mtl
 │   └─ robotBody.png
 ├─ src/
 │   ├─ component1.h
 │   ├─ component1.cpp
 │   ├─ component2.h
 │   ├─ component2.cpp
 │   ├─ projectScript.h
 │   ├─ projectScript.cpp
 │   ├─ robotScript.h
 │   └─ robotScript.cpp
 ├─ README.md
 ├─ CMakeLists.txt
 └─ myProject.atta
```

- `build/` folder is automatically created by atta when your project code is compiled.
- `resource/` folder stores all 3D models and images that will be used in the simulation. Atta automatically search for models and images in this folder.
- `src/` folder contains all source code. As you'll see shortly, there can be multiple scripts, one of them being the project script. Custom components are also defined here.
- `CMakeLists.txt` defines which files are needed to build each cmake target. A cmake target can be for a script or a component.
- `.atta` file stores all your project information: viewports, entities, components, etc.

## Viewports
Your simulation is rendered to the viewport, and you can have as many viewports as you wish. The atta::Viewport is composed of a atta::Camera, and a atta::Renderer.

To move your viewports camera:
 - While pressing the `mouse wheel`
 - **Rotate** the camera moving the mouse
 - **Move** with `AWSD`. `QE` to move up and down

// TODO: Gif moving the camera

You can see all your viewports and change their properties under `Viewports > [ViewportName]`. A window will appear where you can change the viewport name, renderer, and camera.

// TODO: Gif open viewport window

Switching to a faster renderer while the simulation is running can be a good idea to decrease rendering overhead. Current renderers are:
 - **FastRenderer:** Render only the albedo for each mesh, this is the fastest renderer you can use, and usually the best choice when doing 2D simulations.
 - **PhongRenderer:** This renderer implements the phong shading model, which can handle lights and materials, but is not very realistic.
 - **PbrRenderer:** The most reallistic of these renderers, but slower. We Usually choose this one to the viewport when performing 3D simulations, but it may not be the best renderer for a simulated robot camera. It's based on the physical model of how light works (Physically Based Rendering).

There are two types of camera:
 - **PerspectiveCamera**: This is the camera we're used to, and it's the choice for 3D simulations.
 - **OrthographicCamera**: This camera uses a projection type that is mostly used for 2D simulations.

// TODO: Image difference perspective/orhotgraphic

There are also two types of camera controls:
 - **FirstPerson**: The mouse and keyboard (AWSDQE) inputs are used to move and rotate the camera. Mainly used with perspective camera.
 - **Planar**: Only the mouse is used to drag the camera. Mainly used with orthographic projection in 2D simulations.

// TODO: Gif moving firstPerson/moving planar

## Entities
Atta has an Entity Component System (ECS) implemented so that is is easy to extend entities and makes it possible to take advantage of how the data is organized in the memory avoid cache misses.

Each entity is composed of many components, and each component is a piece of data that tells something about the entity. For example, we can have a component that have the 3D position of the entity in the world, another component that tells which is the entity 3D model, another that tells the entity name. 

// TODO: Image with entity and components associated to it

On the scene tree we can see all entities that were created in the project. In this example we have only two entities, the `Cube` entity and the `Point light` entity.

// TODO: Scene tree image

We can create new entities by clicking in the `Create entity` button. If we right-click an entity, we can copy it or delete it.

## Components

Combining components make it possible to create point lights, metal spheres, camera sensor, etc. Some of the components implemented in atta are:
 - atta::TransformComponent
 - atta::NameComponent
 - atta::MaterialComponent
 - atta::MeshComponent
 - atta::PointLightComponent
 - atta::ScriptComponent
 - atta::PrototypeComponent
 - atta::CameraComponent

Combing components we can create, for example:
 - **Point light**: atta::TransformComponent + atta::PointLightComponent
 - **Metal Sphere**: atta::TransformComponent + atta::MeshComponent + atta::MaterialComponent
 - **Camera Sensor**: atta::TransformComponent + atta::CameraComponent

We can inspect which components an entity have by clicking on it on the scene tree.

// TODO: gif clicking entities

We can edit each entity component and add more components as we wish.

// TODO: gif changing components

### Custom Components
Atta has many components already implemented, but you may want to implement one yourself. For example, if you want to simulate a genetic algorihm, you will want each robot to have a GeneComponent.

#### Creating basic custom component
This is the first time we will create a code for atta, and **for now this only works on Linux**.

First, lets create the source files, your file structure should be like this:

```
MyProject
 ├─ src/
 │   ├─ geneComponent.h
 │   └─ geneComponent.cpp
 ├─ CMakeLists.txt
 └─ myProject.atta
```

The `geneComponent.h` will be:

```cpp
// geneComponent.h
#ifndef GENE_COMPONENT_H
#define GENE_COMPONENT_H
#include <atta/componentSystem/components/component.h>
#include <atta/componentSystem/componentRegistry.h>
#include <atta/core/math/math.h>

struct GeneComponent final : public atta::Component
{
    float firstGene;
    float secondGene;
};
ATTA_REGISTER_COMPONENT(GeneComponent)
template<> atta::ComponentRegistry::Description& atta::TypedComponentRegistry<GeneComponent>::getDescription();

#endif// GENE_COMPONENT_H
```

And the `geneComponent.cpp` will be:

```cpp
// geneComponent.cpp
#include "geneComponent.h"

template<>
atta::ComponentRegistry::Description& atta::TypedComponentRegistry<GeneComponent>::getDescription()
{
    // We will define this component description so that atta knows how to interpret the bytes
    static ComponentRegistry::Description desc =
        {
            "Gene",// Component name
            { // Component attributes (type, byte offset, name)
                { atta::ComponentRegistry::AttributeType::FLOAT32, offsetof(GeneComponent, firstGene), "firstGene" },
                { atta::ComponentRegistry::AttributeType::FLOAT32, offsetof(GeneComponent, secondGene), "secondGene" },
            }
        };

    return desc;
}

```

We also need to change the `CMakeLists.txt` so that atta will know how to compile this component

```cmake
# CMakeLists.txt
cmake_minimum_required(VERSION 3.12)
project(myProject VERSION 1.0.0 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# Create gene component target
add_library(geneComponent SHARED
    src/geneComponent.cpp
)
```

That's it! If try to add a component now, you will see your GeneComponent there.

## Scripts

### Script

### Project Script

#### Custom project UI

## Examples
