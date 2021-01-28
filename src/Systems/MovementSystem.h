#ifndef MOVEMENTSYSTEM_H
#define MOVEMENTSYSTEM_H

#include <vector>

#include "../ECS/ECS.h"

class MovementSystem: public System
{
    public:
        MovementSystem()
        {
            // TODO:
            // RequireComponent<TransformComponent>();
            // RequireComponent<VelocityComponent>();
        }

        void Update(double deltaTime)
        {
            // TODO:
            // Loop all entities that the system is interested in
            // for (auto entitiy: GetEntities())
            // {
                // TODO: Update entity position based on its velocity.
            // }
        }
};

#endif
