#include "Instance.h"
#include "Camera.h"
#include <glm/ext/matrix_transform.hpp>
#include "Scene.h"
#include "Application.h"

Instance::Instance(glm::mat4 transform, Mesh* mesh, aie::ShaderProgram* shader) : m_transform(transform), m_mesh(mesh), m_shader(shader)
{
}

//void Instance::Draw(Camera* camera, float windowWidth, float windowHeight, glm::vec3 ambientLight, Light* light)
void Instance::Draw(Scene* scene)
{
    // set the shader pipeline
    m_shader->bind();

    // bind transform and other uniforms
    //auto pvm = camera->GetProjectionMatrix(windowWidth, windowHeight) * camera->GetViewMatrix() * m_transform;
    //glm::mat4 pv = camera->GetProjectionMatrix(windowWidth, windowHeight) * camera->GetViewMatrix();
    //auto pvm = pv * m_transform;
    auto pvm = scene->GetCamera()->GetProjectionMatrix(scene->GetWindowSize().x, scene->GetWindowSize().y)
                    *scene->GetCamera()->GetViewMatrix() * m_transform;
    
    m_shader->bindUniform("ProjectionViewModel", pvm);
    m_shader->bindUniform("ModelMatrix", m_transform);
    m_shader->bindUniform("LightColour", scene->GetLight()->colour);
    m_shader->bindUniform("LightDirection", scene->GetLight()->direction);
    m_shader->bindUniform("CameraPosition", scene->GetCamera()->GetPosition());

    m_mesh->ApplyMaterial(m_shader);
    // draw the mesh
    m_mesh->Draw();
}

glm::mat4 Instance::MakeTransform(glm::vec3 position, glm::vec3 eulerAngles, glm::vec3 scale)
{
    return glm::translate(glm::mat4(1.0f), position)
        * glm::rotate(glm::mat4(1), glm::radians(eulerAngles.z), glm::vec3(0, 0, 1))
        * glm::rotate(glm::mat4(1), glm::radians(eulerAngles.y), glm::vec3(0, 1, 0))
        * glm::rotate(glm::mat4(1), glm::radians(eulerAngles.x), glm::vec3(1, 0, 0))
        * glm::scale(glm::mat4(1), scale);
}
