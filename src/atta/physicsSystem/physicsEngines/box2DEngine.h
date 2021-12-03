//--------------------------------------------------
// Atta Physics System
// box2DEngine.h
// Date: 2021-11-29
// By Breno Cunha Queiroz
//--------------------------------------------------
#ifndef ATTA_PHYSICS_SYSTEM_PHYSICS_ENGINES_BOX2D_ENGINE_H
#define ATTA_PHYSICS_SYSTEM_PHYSICS_ENGINES_BOX2D_ENGINE_H
#include <atta/physicsSystem/physicsEngines/physicsEngine.h>
#include "box2d/box2d.h"

namespace atta
{
    class Box2DEngine : public PhysicsEngine
    {
    public:
		Box2DEngine();
		~Box2DEngine();

        void start() override;
        void step(float dt) override;
        void stop() override;

	private:
		std::shared_ptr<b2World> _world;
		std::vector<std::pair<EntityId, b2Body*>> _bodies;
    };
}

#endif// ATTA_PHYSICS_SYSTEM_PHYSICS_ENGINES_BOX2D_ENGINE_H
