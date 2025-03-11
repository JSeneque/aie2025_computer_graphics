#include "Application.h"
#include "Gizmos.h"
#include "glm/ext.hpp"



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

    view = glm::lookAt(glm::vec3(10, 10, 10), glm::vec3(0), glm::vec3(0, 1, 0));
    projection = glm::perspective(glm::pi<float>() * 0.25f, 16 / 9.f, 0.1f, 1000.0f);

    shader.loadShader(aie::eShaderStage::VERTEX, "../bin/Shaders/Vertex/Simple.vert");
    shader.loadShader(aie::eShaderStage::FRAGMENT, "../bin/Shaders/Fragment/Simple.Frag");

    if (!shader.link())
    {
        printf("Shader Error: %s\n", shader.getLastError());
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

    // listens for inputs
    glfwPollEvents();

    // Put your updates here

    return true;
}

void Application::Draw()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    aie::Gizmos::clear();

    aie::Gizmos::addTransform(glm::mat4(1));

    glm::vec4 white{ 1 };
    glm::vec4 black{ 0, 0, 0, 1 };

    for (int i = 0; i < 21; ++i)
    {
        aie::Gizmos::addLine(glm::vec3{ -10 + i, 0, 10 }, glm::vec3{ -10 + i, 0, -10 }, i == 10 ? white : black);
        aie::Gizmos::addLine(glm::vec3{ 10, 0, -10 + i }, glm::vec3{ -10, 0, -10 + i }, i == 10 ? white : black);
    }

    aie::Gizmos::draw(projection * view);

    // bind shader
    shader.bind();

    // bind transform
    auto pvm = projection * view * quadTransform;
    shader.bindUniform("ProjectionViewModel", pvm);
    shader.bindUniform("time", deltaTime);

    mesh.Draw();

    // swap double buffer windows
    glfwSwapBuffers(window);
}

void Application::Shutdown()
{
    aie::Gizmos::destroy();
    glfwDestroyWindow(window);
    glfwTerminate();
}
