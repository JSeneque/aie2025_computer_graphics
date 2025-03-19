#include "Application.h"
#include "Gizmos.h"
#include "glm/ext.hpp"

Application* Application::s_instance = nullptr;

bool Application::Startup()
{
    // if the initialisation fails, return false
    if (!glfwInit())
        return false;

    window = glfwCreateWindow(windowWidth, windowHeight, "Computer Graphics", nullptr, nullptr);

    // check that window was create
    if (window == nullptr) {
        glfwTerminate();
        return false;
    }

    // tell glfw we are working with this winow
    glfwMakeContextCurrent(window);

    // if this fails, destroy window and terminate
    if (!gladLoadGL())
    {
        glfwDestroyWindow(window);
        // terminate
        glfwTerminate();
        return -3;
    }

    printf("GLVersion: %i.%i\n", GLVersion.major, GLVersion.minor);

    aie::Gizmos::create(10000, 10000, 0, 0);

    s_instance = this;
    glfwSetCursorPosCallback(window, &Application::SetMousePosition);

    view = glm::lookAt(glm::vec3(10, 10, 10), glm::vec3(0), glm::vec3(0, 1, 0));
    projection = glm::perspective(glm::pi<float>() * 0.25f, 16 / 9.f, 0.1f, 1000.0f);

    shader.loadShader(aie::eShaderStage::VERTEX, "../bin/Shaders/Vertex/Simple.vert");
    shader.loadShader(aie::eShaderStage::FRAGMENT, "../bin/Shaders/Fragment/Simple.frag");
    m_phongShader.loadShader(aie::eShaderStage::VERTEX, "../bin/Shaders/Vertex/phong.vert");
    m_phongShader.loadShader(aie::eShaderStage::FRAGMENT, "../bin/Shaders/Fragment/phong.frag");

    m_light.colour = { 1, 1, 0 };
    //m_ambientLight = { 0.25f, 0.25f, 0.25f };

    if (!shader.link())
    {
        printf("Shader Error: %s\n", shader.getLastError());
        return false;
    }

    if (!m_phongShader.link())
    {
        printf("Shader Error: %s\n", m_phongShader.getLastError());
        return false;
    }
    mesh.InitialiseFromFile("../bin/Models/stanford/bunny.obj");

    quadTransform = { 0.5, 0, 0, 0,
                       0, 0.5, 0, 0,
                       0, 0, 0.5, 0,
                       0, 0, 0, 1 };

    glClearColor(0.25f, 0.25f, 0.25f, 1);
    glEnable(GL_DEPTH_TEST);

    
}

bool Application::Update(float dt)
{
    if(glfwWindowShouldClose(window) || glfwGetKey(window, GLFW_KEY_ESCAPE))
    {
        return false;
    }

    deltaTime += dt;

    camera.Update(dt, window);

    m_lastMousePosition = m_mousePosition;

    // listens for inputs
    glfwPollEvents();

    // Put your updates here
    float time = glfwGetTime();

    // rotate the light
    m_light.direction = glm::normalize(glm::vec3(glm::cos(time * 2), glm::sin(time * 2), 0));

    return true;

    

    
}

void Application::Draw()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    aie::Gizmos::clear();

    aie::Gizmos::addTransform(glm::mat4(1));

    //aie::Gizmos::draw(projection * view);
    glm::mat4 pv = camera.GetProjectionMatrix(windowWidth, windowHeight) * camera.GetViewMatrix();

    // bind shader
    shader.bind();

    // bind transform
    //auto pvm = projection * view * quadTransform;
    auto pvm = pv * quadTransform;
    shader.bindUniform("ProjectionViewModel", pvm);
    //shader.bindUniform("Project", deltaTime);



    // bind phong shader program
    m_phongShader.bind();
    // bind light
        
    // bind transform and lighting
    m_phongShader.bindUniform("ProjectionViewModel", pvm);
    m_phongShader.bindUniform("ModelMatrix", quadTransform);
    m_phongShader.bindUniform("AmbientColour", m_ambientLight);
    m_phongShader.bindUniform("LightColour", m_light.colour);
    m_phongShader.bindUniform("LightDirection", m_light.direction);   
    





    mesh.Draw();

    glm::vec4 white{ 1 };
    glm::vec4 black{ 0, 0, 0, 1 };

    for (int i = 0; i < 21; ++i)
    {
        aie::Gizmos::addLine(glm::vec3{ -10 + i, 0, 10 }, glm::vec3{ -10 + i, 0, -10 }, i == 10 ? white : black);
        aie::Gizmos::addLine(glm::vec3{ 10, 0, -10 + i }, glm::vec3{ -10, 0, -10 + i }, i == 10 ? white : black);
    }

    aie::Gizmos::draw(pv);

    // swap double buffer windows
    glfwSwapBuffers(window);
}

void Application::Shutdown()
{
    aie::Gizmos::destroy();
    glfwDestroyWindow(window);
    glfwTerminate();
}

Application* Application::Get()
{
    if (s_instance != nullptr)
    {
        return  s_instance; 
    }
}

void Application::SetMousePosition(GLFWwindow* window, double x, double y)
{
    s_instance->m_mousePosition.x = (float)x;
    s_instance->m_mousePosition.y = (float)y;
}
