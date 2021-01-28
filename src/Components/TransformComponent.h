#ifndef TRANSFORMCOMPONENT_H
#define TRANSFORMCOMPONENT_H

#include <glm/vec2.hpp>

struct TransformComponent
{
    glm::vec2 position;
    glm::vec2 scale;
    double rotation;
};

#endif
