#pragma once
#include <list>
#include <vector>
#include "Global.h"

class Instance;
class Camera;

class Scene
{
public:
    Scene(Camera* camera, glm::vec2 windowSize, Light* light, glm::vec3 ambientLight);
    ~Scene();
    
    void AddInstance(Instance* instance);
    Camera* GetCamera();
    glm::vec2 GetWindowSize() const;
    void Draw();
    Light* GetLight();
    int GetNumLights();
    glm::vec3* GetPointLightColours();
    glm::vec3* GetPointLightPosition();

    void AddPointLight(Light light);
    
    std::vector<Light>& GetPointLights();
public:
    Camera* m_camera;
    glm::vec2 m_windowSize;
    Light*  m_light;
    Light m_sunLight;
    std::vector<Light> m_pointLights;
    glm::vec3 m_pointLightPositions[MAX_LIGHTS];
    glm::vec3 m_pointLightColours[MAX_LIGHTS];
    glm::vec3 m_ambientLight;
    std::list<Instance*> m_instances;
};
