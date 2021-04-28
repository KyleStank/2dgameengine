#ifndef ENGINE_RIGIDBODYCOMPONENT_H
#define ENGINE_RIGIDBODYCOMPONENT_H

#include <glm/vec2.hpp>

namespace engine
{
    struct RigidbodyComponent
    {
        glm::vec2 velocity;

        RigidbodyComponent(glm::vec2 velocity = glm::vec2(0.0, 0.0))
        {
            this->velocity = velocity;
        }
    };
}

#endif
