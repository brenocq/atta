//--------------------------------------------------
// Entity Spawn
// projectScript.cpp
// Date: 2022-08-20
// By Breno Cunha Queiroz
//--------------------------------------------------
#include "projectScript.h"
#include <atta/component/components/material.h>
#include <atta/component/components/mesh.h>
#include <atta/component/components/name.h>
#include <atta/component/components/pointLight.h>
#include <atta/component/components/transform.h>
#include <atta/resource/interface.h>
#include <atta/resource/resources/material.h>

namespace cmp = atta::component;
namespace rsc = atta::resource;

void Project::onStart() {
    _redCube = cmp::createEntity();
    _redCube.add<cmp::Name>()->set("Red cube");
    _redCube.add<cmp::Transform>();
    _redCube.add<cmp::Mesh>()->set("meshes/cube.obj");

    rsc::Material* red = rsc::get<rsc::Material>("red");
    red->setColor({1, 0, 0});
    _redCube.add<cmp::Material>()->set(red);

    _pointLight = cmp::createEntity();
    _pointLight.add<cmp::Name>()->set("Point light");
    _pointLight.add<cmp::Transform>()->position = {2, 0, 2};
    _pointLight.add<cmp::PointLight>()->intensity = {5, 5, 5};

    for (int x = 0; x < 5; x++)
        for (int y = 0; y < 5; y++) {
            cmp::Entity sphere = cmp::createEntity();
            _spheres.push_back(sphere);
            sphere.add<cmp::Name>()->set("Sphere " + std::to_string(x) + " " + std::to_string(y));

            sphere.add<cmp::Transform>()->position = {(x - 2) / 10.0f, (y - 2) / 10.0f, 0.5f};
            sphere.get<cmp::Transform>()->scale = atta::vec3(0.05);

            sphere.add<cmp::Mesh>()->set("meshes/sphere.obj");
            sphere.add<cmp::Material>()->set("red");
        }
}

void Project::onStop() {
    cmp::deleteEntity(_redCube);
    cmp::deleteEntity(_pointLight);
    for (cmp::Entity sphere : _spheres)
        cmp::deleteEntity(sphere);
    _spheres.clear();
};
