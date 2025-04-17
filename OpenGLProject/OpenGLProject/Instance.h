#pragma once
#include <glm/glm.hpp>

#include "Application.h"
#include "Camera.h"
#include "Shader.h"
#include "Global.h"


class Mesh;
class Camera;
class Scene;

class Instance
{
public:
    Instance(glm::mat4 transform, Mesh* mesh, aie::ShaderProgram* shader);
    //void Draw(Camera* camera, float windowWidth, float windowHeight, glm::vec3 ambientLight, Light* light);
    void Draw(Scene* scene);
    glm::mat4 MakeTransform(glm::vec3 position, glm::vec3 eulerAngles, glm::vec3 scale);
    
protected:
    glm::mat4 m_transform;
    Mesh* m_mesh;
    aie::ShaderProgram* m_shader;
};
