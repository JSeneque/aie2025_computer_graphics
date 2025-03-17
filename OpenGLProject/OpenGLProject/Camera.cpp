#include "Application.h"
#include "Camera.h"
#include <glm/ext.hpp>


glm::mat4 Camera::GetViewMatrix()
{
    // calculate theta and phi in radians
    float thetaR = glm::radians(theta);
    float phiR = glm::radians(phi);

    // calculate the forward vector
    glm::vec3 forward(cos(phiR) * cos(thetaR), sin(phiR), cos(phiR) * sin(thetaR));

    // look ahead using our current position in a forward direction
    return glm::lookAt(m_position, m_position + forward, glm::vec3(0, 1, 0));
}

glm::mat4 Camera::GetProjectionMatrix(float w, float h)
{
    return glm::perspective(glm::pi<float>() * 0.25f, w / h, 0.1f, 1000.0f);
}

void Camera::Update(float dt, GLFWwindow* window)
{
    // calculate theta and phi in radians
    float thetaR = glm::radians(theta);
    float phiR = glm::radians(phi);
    // calculate the forward vector
    glm::vec3 forward(cos(phiR) * cos(thetaR), sin(phiR), cos(phiR) * sin(thetaR));
    // calculate the right vector
    glm::vec3 right(-sin(thetaR), 0, cos(thetaR));
    // up vector
    glm::vec3 up(0, 1, 0);

    // capture the movement keys
    if (glfwGetKey(window, GLFW_KEY_W)) 
        m_position += forward * dt;
    if (glfwGetKey(window, GLFW_KEY_S)) 
        m_position -= forward * dt;
    if (glfwGetKey(window, GLFW_KEY_A)) 
        m_position -= right * dt;
    if (glfwGetKey(window, GLFW_KEY_D)) 
        m_position += right * dt;
    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT)) 
        m_position += up * dt;
    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL)) 
        m_position -= up * dt;

    glm::vec2 mouseDelta = Application::Get()->GetMouseDelta();

    const float turnSpeed = 0.1f;
    // if the right button is down, increment theta and phi
    if (glfwGetMouseButton(window, 1))
    {
        theta += turnSpeed * mouseDelta.x;
        phi -= turnSpeed * mouseDelta.y;
    }
}
