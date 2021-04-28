#ifndef MOVEMENTSYSTEM_H
#define MOVEMENTSYSTEM_H

#include <vector>
#include "../ecs/ecs.h"
#include "../components/rigidbody_component.h"
#include "../components/transform_component.h"

class MovementSystem: public System
{
    public:
        MovementSystem()
        {
            RequireComponent<TransformComponent>();
            RequireComponent<RigidbodyComponent>();
        }

        void Update(const double deltaTime)
        {
            for (const Entity entity: GetSystemEntities())
            {
                const RigidbodyComponent rigidbody = entity.GetComponent<RigidbodyComponent>();
                TransformComponent& transform = entity.GetComponent<TransformComponent>();

                transform.position.x += rigidbody.velocity.x * deltaTime;
                transform.position.y += rigidbody.velocity.y * deltaTime;
            }
        }
};

#endif
