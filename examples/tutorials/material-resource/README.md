<!--
title: Material Resource
description: Introduction to atta materials
image: https://storage.googleapis.com/atta-images/docs/tutorial/material-resource/2022-08-19.png
-->
# Material Resource

<div align="center">
  <img src="https://storage.googleapis.com/atta-images/docs/tutorial/material-resource/2022-08-19.png" height="400">
</div>

Entities are only rendered with a material if they also have `cmp::Transform` and `cmp::Mesh` components.

Materials are resources, and they can be shared among different entities.

All images in the `resources/` folders are automatically loaded by atta and can be used in the materials.

If you don't want to upload the resource files to github, check the `CMakeLists.txt` to see an example of using cmake to download the resources automatically when building.

Each material has 4 attributes:
 - `color`
 - `roughness`
 - `metallic`
 - `ao`

