#ifndef ENGINE_TRANSFORMCOMPONENT_H
#define ENGINE_TRANSFORMCOMPONENT_H

#include <glm/vec2.hpp>

namespace engine::components
{
    struct transform_component
    {
        glm::vec2 position;
        glm::vec2 scale;
        double rotation;

        transform_component(glm::vec2 position = glm::vec2(0, 0), glm::vec2 scale = glm::vec2(1, 1), double rotation = 0.0)
        {
            this->position = position;
            this->scale = scale;
            this->rotation = rotation;
        }
    };
}

#endif
