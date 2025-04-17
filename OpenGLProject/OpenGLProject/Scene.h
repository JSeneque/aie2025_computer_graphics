#pragma once
#include <list>

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
public:
    Camera* m_camera;
    glm::vec2 m_windowSize;
    Light*  m_light;
    glm::vec3 m_ambientLight;
    std::list<Instance*> m_instances;
};
