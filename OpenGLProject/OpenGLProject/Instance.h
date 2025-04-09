#pragma once
#include <glm/glm.hpp>

#include "Camera.h"
#include "Shader.h"

class Mesh;
class Light;
class Camera;


class Instance
{
public:
    void Draw(Camera* camera, float windowWidth, float windowHeight, glm::vec3 ambientLight, Light* light);
protected:
    glm::mat4 m_transform;
    Mesh* m_mesh;
    aie::ShaderProgram* m_shader;
};
