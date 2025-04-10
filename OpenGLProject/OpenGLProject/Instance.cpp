#include "Instance.h"

#include "Application.h"

Instance::Instance(glm::mat4 transform, Mesh* mesh, aie::ShaderProgram* shader, bool isHidden) : m_transform(transform), m_mesh(mesh), m_shader(shader), m_isHidden(isHidden)
{
}

void Instance::Draw(Camera* camera, float windowWidth, float windowHeight, glm::vec3 ambientLight, Light* light)
{
    // set the shader pipeline
    m_shader->bind();

    // bind transform and other uniforms
    //auto pvm = camera->GetProjectionMatrix(windowWidth, windowHeight) * camera->GetViewMatrix() * m_transform;
    glm::mat4 pv = camera->GetProjectionMatrix(windowWidth, windowHeight) * camera->GetViewMatrix();
    auto pvm = pv * m_transform;
    
    m_shader->bindUniform("ProjectionViewModel", pvm);
    m_shader->bindUniform("ModelMatrix", m_transform);
    m_shader->bindUniform("LightColour", light->colour);
    m_shader->bindUniform("LightDirection", light->direction);
    m_shader->bindUniform("CameraPosition", camera->GetPosition());

    //soulspearMesh.ApplyMaterial(&m_phongShader);
    // soulspearMesh.Draw();
    // draw the mesh
    m_mesh->Draw();
}
