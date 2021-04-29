#ifndef ENGINE_MOVEMENTSYSTEM_H
#define ENGINE_MOVEMENTSYSTEM_H

#include <vector>
#include "../ecs.h"
#include "../components/rigidbody_component.h"
#include "../components/transform_component.h"

namespace engine::systems
{
    class movement_system: public ecs::system
    {
        public:
            movement_system()
            {
                require_component<components::transform_component>();
                require_component<components::rigidbody_component>();
            }

            void update(const double delta_time)
            {
                for (const ecs::entity entity: get_system_entities())
                {
                    const components::rigidbody_component rigidbody = entity.get_component<components::rigidbody_component>();
                    components::transform_component& transform = entity.get_component<components::transform_component>();

                    transform.position.x += rigidbody.velocity.x * delta_time;
                    transform.position.y += rigidbody.velocity.y * delta_time;
                }
            }
    };
}

#endif
