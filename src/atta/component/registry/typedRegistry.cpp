//--------------------------------------------------
// Atta Component Module
// typedRegistry.cpp
// Date: 2021-01-01
// By Breno Cunha Queiroz
//--------------------------------------------------
#include <atta/component/registry/typedRegistry.h>

#include "/home/breno/Github/brenocq-atta/ants/src/antComponent.h"
#include "/home/breno/Github/brenocq-atta/ants/src/worldComponent.h"
#include <atta/component/components/prototype.h>

namespace atta::component {

// template <typename T>
// ATTA_GPU_CONST ComponentId idGpu;

// template <>
// ATTA_GPU_CONST ComponentId idGpu<PrismaticJoint> = 0;
// template <>
// ATTA_GPU_CONST ComponentId idGpu<RevoluteJoint> = 1;
// template <>
// ATTA_GPU_CONST ComponentId idGpu<RigidBody2D> = 2;
// template <>
// ATTA_GPU_CONST ComponentId idGpu<RigidJoint> = 3;
// template <>
// ATTA_GPU_CONST ComponentId idGpu<CameraSensor> = 4;
// template <>
// ATTA_GPU_CONST ComponentId idGpu<InfraredSensor> = 5;
// template <>
// ATTA_GPU_CONST ComponentId idGpu<BoxCollider> = 6;
// template <>
// ATTA_GPU_CONST ComponentId idGpu<BoxCollider2D> = 7;
// template <>
// ATTA_GPU_CONST ComponentId idGpu<CircleCollider2D> = 8;
// template <>
// ATTA_GPU_CONST ComponentId idGpu<CylinderCollider> = 9;
// template <>
// ATTA_GPU_CONST ComponentId idGpu<DirectionalLight> = 10;
// template <>
// ATTA_GPU_CONST ComponentId idGpu<EnvironmentLight> = 11;
// template <>
// ATTA_GPU_CONST ComponentId idGpu<Material> = 12;
// template <>
// ATTA_GPU_CONST ComponentId idGpu<Mesh> = 13;
// template <>
// ATTA_GPU_CONST ComponentId idGpu<Name> = 14;
// template <>
// ATTA_GPU_CONST ComponentId idGpu<PointLight> = 15;
template <>
ATTA_GPU_CONST ComponentId idGpu<Prototype> = 16;
// template <>
// ATTA_GPU_CONST ComponentId idGpu<Relationship> = 17;
// template <>
// ATTA_GPU_CONST ComponentId idGpu<RigidBody> = 18;
// template <>
// ATTA_GPU_CONST ComponentId idGpu<Script> = 19;
// template <>
// ATTA_GPU_CONST ComponentId idGpu<SphereCollider> = 20;
// template <>
// ATTA_GPU_CONST ComponentId idGpu<Transform> = 21;
template <>
ATTA_GPU_CONST ComponentId idGpu<AntComponent> = 22;
template <>
ATTA_GPU_CONST ComponentId idGpu<WorldComponent> = 23;

} // namespace atta::component
