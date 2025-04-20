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
    for ( int i = 0; i < MAX_LIGHTS && i < m_pointLights.size(); ++i)
    {
        m_pointLightPositions[i] = m_pointLights[i].direction;
        m_pointLightColours[i] = m_pointLights[i].colour;
    }
    
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

int Scene::GetNumLights()
{
    return m_pointLights.size();
}

glm::vec3* Scene::GetPointLightColours()
{
    return &m_pointLightColours[0];
}

glm::vec3* Scene::GetPointLightPosition()
{
    return &m_pointLightPositions[0];
}

void Scene::AddPointLight(Light light)
{
    m_pointLights.push_back(light);
}

std::vector<Light>& Scene::GetPointLights()
{
    return m_pointLights;
}
