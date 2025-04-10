#pragma once
#include <glm/glm.hpp>

#include "Application.h"
#include "Camera.h"
#include "Shader.h"
#include "Global.h"


class Mesh;
class Camera;

class Instance
{
public:
    Instance(glm::mat4 transform, Mesh* mesh, aie::ShaderProgram* shader, bool isHidden);
    void Draw(Camera* camera, float windowWidth, float windowHeight, glm::vec3 ambientLight, Light* light);
protected:
    glm::mat4 m_transform;
    Mesh* m_mesh;
    aie::ShaderProgram* m_shader;
    bool m_isHidden = false;
    
};
