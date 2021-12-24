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
 │   ├─ geneComponent.h <--
 │   └─ geneComponent.cpp <--
 ├─ CMakeLists.txt <--
 └─ myProject.atta
```

The `geneComponent.h` will be:

```cpp
// geneComponent.h
#ifndef GENE_COMPONENT_H
#define GENE_COMPONENT_H
#include <atta/componentSystem/components/component.h>
#include <atta/componentSystem/componentRegistry.h>

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

# Use atta 0.0.0.3 package
# This is necessary to use headers from atta dependencies
find_package(atta 0.0.0.3 REQUIRED)
include_directories(${atta_INCLUDE_DIRS})

# Create gene component target
add_library(geneComponent SHARED
    src/geneComponent.cpp
)
```

That's it! If try to add a component now, you will see your GeneComponent there.

## Scripts
Scripts are pieces of code that will be executed by atta. There are two types of scripts:
- **Project script**: There exist only one instance of this script in the whole simulation, and it can perform global computations, render UI, access peripheral, etc.
- **Script**: The other type of script is attached to one entity and ideally just reads and updates the its entity's components. When this type of script only reads and writes to components, it is possible to massively parallelize its execution, distributing the computation across GPUs or different computers.

### Script

Let's create our first script, this script will animate the point light to make it rotate around the cube. Create two new files:

```
MyProject
 ├─ src/
 │   ├─ geneComponent.h
 │   ├─ geneComponent.cpp
 │   ├─ lightScript.h <--
 │   └─ lightScript.cpp <--
 ├─ CMakeLists.txt
 └─ myProject.atta
```

The `lightScript.h` will be:

```cpp
// lightScript.h
#ifndef LIGHT_SCRIPT_H
#define LIGHT_SCRIPT_H
#include <atta/pch.h>
#include <atta/scriptSystem/script.h>

class LightScript : public atta::Script
{
public:
    // When we start the simulation, this function is called each step for each entity with this script
    // dt is the time in seconds between this and the last step
    void update(atta::Entity entity, float dt) override;
};

ATTA_REGISTER_SCRIPT(LightScript)

#endif// LIGHT_SCRIPT_H
```

And the `lightScript.cpp` will be:

```cpp
// lightScript.cpp
#include "lightScript.h"

void LightScript::update(Entity entity, float dt)
{
    LOG_DEBUG("LightScript", "Update function called");// Tag and message to print

    // Get TransformComponent
    // Update TransformComponent to make the light rotate
}
```

Now we need to update the `CMakeLists.txt` so that this script is compiled, add:

```cmake
# CMakeLists.txt

# Create light script target
add_library(lightScript SHARED
    src/lightScript.cpp
)
```

We can now add a atta::ScriptComponent to the light entity. Because there exists only the `LightScript` on this project, the combo box will show only this script.

// TODO: gif adding the script component

If we press de start button, we will see `[LightScript] Update function called` printed many times on the terminal.

// TODO: gif pressing start

Now lets update the point light position by changing its atta::TransformComponent.

```cpp
// lightScript.cpp
#include "lightScript.h"
#include <atta/componentSystem/components/transformComponent.h>

void LightScript::update(Entity entity, float dt)
{
    // Get TransformComponent
    atta::TransformComponent* t = entity.getComponent<atta::TransformComponent>();

    // Update TransformComponent to make the light rotate
    static float time = 0;
    time += dt*10.0f;// You may want to change this value if the light is rotating too fast or too slow
    t->position.x = cos(time);
    t->position.y = sin(time);
}
```

> **Warning**
>
> Using static variables is only OK if the script code will only be executed on CPU.
> If you plan to use GPUs or clusters, try to avoid static variables.

Atta should have automatically recompiled your code after saving it. Press play (if you're not already running) to see the light rotate.

// TODO: Gif of light rotating

In fact, try changing the code and saving the file while the simulation is running, you should see the simulation update a few milisseconds after each save.

We could also change the point light intensity:
```cpp
// lightScript.cpp
#include "lightScript.h"
#include <atta/componentSystem/components/transformComponent.h>

void LightScript::update(Entity entity, float dt)
{
    // Get components
    atta::TransformComponent* t = entity.getComponent<atta::TransformComponent>();
    atta::PointLightComponent* pl = entity.getComponent<atta::PointLightComponent>();

    // Update TransformComponent to make the light rotate
    static float time = 0;
    time += dt*10.0f;// You may want to change this value if the light is rotation too fast or too slow
    t->position.x = cos(time);
    t->position.y = sin(time);

    // Update PointLightComponent to make the light rotate
    pl->intensity.x = cos(time);
    pl->intensity.y = sin(time);
}
```

### Project Script
There is only one project script per project, and it has functions that are independent of entities.
Some of the functions that exists are:
 - `onStart()`:** Called every time the **start button** is pressed.
 - `onStop()`:** Called every time the **stop button** is pressed.
 - `onPause()`:** Called every time the **pause button** is pressed.
 - `onContinue()`:** Called every time the **continue button** is pressed.
 - `onUpdateBefore(float delta)`:** Called every step before all scripts are executed.
 - `onUpdateAfter(float delta)`:** Called every step after all scripts are executed.
 - `onUIRender()`:** Called by the UI system, can be used to render user interfaces.
 - `onAttaLoop()`:** Executed inside the atta main loop.

Lets now create the project script to test some of these functions.

```
MyProject
 ├─ src/
 │   ├─ geneComponent.h
 │   ├─ geneComponent.cpp
 │   ├─ lightScript.h
 │   ├─ lightScript.cpp
 │   ├─ projectScript.h <--
 │   └─ projectScript.cpp <--
 ├─ CMakeLists.txt
 └─ myProject.atta
```

The `projectScript.h` will be:

```cpp
// projectScript.h
#ifndef PROJECT_SCRIPT_H
#define PROJECT_SCRIPT_H
#include <atta/pch.h>
#include <atta/scriptSystem/projectScript.h>

class ProjectScript : public atta::ProjectScript
{
public:
	void onStart() override;
	void onStop() override;
	void onPause() override;
	void onContinue() override;
};

ATTA_REGISTER_PROJECT_SCRIPT(ProjectScript)

#endif// PROJECT_SCRIPT_H
```

And the `projectScript.cpp` will be:

```cpp
// projectScript.cpp
#include "projectScript.h"

void ProjectScript::onStart() { LOG_DEBUG("ProjectScript", "Start button pressed"); }
void ProjectScript::onStop() { LOG_DEBUG("ProjectScript", "Stop button pressed"); }
void ProjectScript::onPause() { LOG_DEBUG("ProjectScript", "Pause button pressed"); }
void ProjectScript::onContinue() { LOG_DEBUG("ProjectScript", "Continue button pressed"); }

```

Now we need to update the `CMakeLists.txt` so that this script is compiled, add:

```cmake
# CMakeLists.txt

# Create project script target
add_library(projectScript SHARED
    src/projectScript.cpp
)
```

Now the messages should you printed as you press the buttons.

// TODO: gif pressing the buttons and messages being printed

#### Custom project UI

To render custom UI we will use the `onUIRender()` function, we can use any ImGui command to create the window, buttons, text, etc.

Add the method declaration to `projectScript.h`.
```cpp
// projectScript.h
    ...
	void onContinue() override;
	void onUIRender() override;
    ...
```

And add ImGui include and the method definition on `projectScript.cpp`.

```cpp
// projectScript.cpp
#include "projectScript.h"
#include <imgui.h>

...

void ProjectScript::onUIRender()
{
    ImGui::Begin("My custom window");
    ImGui::Text("Some text");
    ImGui::Text("More text");
    if(ImGui::Button("Test button"))
    {
        LOG_DEBUG("ProjectScript", "Button clicked!");
    }
    ImGui::End();
}

...
```

After saving the file the window should have appeared on atta.

// TODO: Gif testing the window

## Examples
That's it! You know all the basics to use atta. We recommend that you check out the examples now.

