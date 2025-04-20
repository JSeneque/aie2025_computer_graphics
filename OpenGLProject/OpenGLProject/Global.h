#pragma once
#include <glm/glm.hpp>

#define MAX_LIGHTS 4

struct Light {
    glm::vec3 direction;
    glm::vec3 colour;

    Light() : direction(0, 0, 0), colour(1, 1, 1) {}

    Light(glm::vec3 pos, glm::vec3 col, float intensity)
    {
        direction = pos;
        colour = col * intensity;
    }
};
