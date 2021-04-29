#ifndef ENGINE_RIGIDBODYCOMPONENT_H
#define ENGINE_RIGIDBODYCOMPONENT_H

#include <glm/vec2.hpp>

namespace engine::components
{
    struct rigidbody_component
    {
        glm::vec2 velocity;

        rigidbody_component(glm::vec2 velocity = glm::vec2(0.0, 0.0))
        {
            this->velocity = velocity;
        }
    };
}

#endif
