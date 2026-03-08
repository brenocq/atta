<!--
title: Simple Script
description: Introduction to atta scripts
image: https://storage.googleapis.com/atta-images/docs/tutorial/simple-script/simple-script.gif
build: https://atta-repos.s3.us-east-1.amazonaws.com/tutorial/simple-script
-->
# Simple Script

<div align="center">
  <img src="https://storage.googleapis.com/atta-images/docs/tutorial/simple-script/simple-script.gif" height="400">
</div>

This projects uses atta scripts to animate two point lights around a cube. There is one **script** to animate the cube and another one to animate the lights. It is also shown when each of the **project script** methods are called.

# Entity Scripts
There are two entity scripts. The `cubeScript` is attached to the cube, and the `lightScript` to each is attached to the lights.
To assign a script to an entity, add a `ScriptComponent` to the entity and choose the script in the combobox.

// TODO image entity components

To a script be recognized by atta, its target must be created in the `CMakeLists.txt` file.

```cmake
...
find_package(atta 0.3.6 REQUIRED)
...
atta_add_target(projectScript "src/projectScript.cpp")
```

The entity script `update()` method is called every simulation step. The simulation only step if it is running.

Check the source code to see how the entity scripts are created.

# Project Script
There is only one project script per project, and it has functions that are independent of entities.

Check the source code to see the methods available for the project script.
