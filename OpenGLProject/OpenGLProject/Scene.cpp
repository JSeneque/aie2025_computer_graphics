#include "Scene.h"
#include "Instance.h"

Scene::Scene(Camera* camera, glm::vec2 windowSize, Light* light, glm::vec3 ambientLight): m_camera(camera), m_windowSize(windowSize), m_light(light), m_ambientLight(ambientLight)
{
    
}

Scene::~Scene()
{
    for (auto it = m_instances.begin(); it != m_instances.end(); ++it)
    {
        delete* it;
    }
}

void Scene::AddInstance(Instance* instance)
{
    m_instances.push_back(instance);
}


Camera* Scene::GetCamera()
{
    return m_camera;
}

glm::vec2 Scene::GetWindowSize() const
{
    return m_windowSize;
}

void Scene::Draw()
{
    for (auto it = m_instances.begin(); it != m_instances.end(); it++)
    {
        Instance* instance = *it;
        instance->Draw(this);
    }
}

Light* Scene::GetLight()
{
    return m_light;
}
