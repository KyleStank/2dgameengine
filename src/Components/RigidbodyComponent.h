#ifndef RIGIDBODYCOMPONENT_H
#define RIGIDBODYCOMPONENT_H

#include <glm/vec2.hpp>

struct RigidbodyComponent
{
    glm::vec2 velocity;

    RigidbodyComponent(glm::vec2 velocity = glm::vec2(0.0, 0.0))
    {
        this->velocity = velocity;
    }
};

#endif