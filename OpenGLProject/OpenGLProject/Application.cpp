#include "Application.h"
#include "Gizmos.h"
#include "Instance.h"
#include "Scene.h"
#include "glm/ext.hpp"
#include "imgui_glfw3.h"

Application* Application::s_instance = nullptr;

Application::~Application()
{
    aie::Gizmos::destroy();

    delete m_scene;
}

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

    aie::ImGui_Init(window, true);

    // set the camera's starting position
    camera = new Camera(glm::vec3(-0.05, 1.67, 1.42), -83.8, -29.9f);

    s_instance = this;
    glfwSetCursorPosCallback(window, &Application::SetMousePosition);

    view = glm::lookAt(glm::vec3(10, 10, 10), glm::vec3(0), glm::vec3(0, 1, 0));
    projection = glm::perspective(glm::pi<float>() * 0.25f, 16 / 9.f, 0.1f, 1000.0f);

    //shader.loadShader(aie::eShaderStage::VERTEX, "../bin/Shaders/Vertex/Simple.vert");
    //shader.loadShader(aie::eShaderStage::FRAGMENT, "../bin/Shaders/Fragment/Simple.frag");
    m_phongShader.loadShader(aie::eShaderStage::VERTEX, "../bin/Shaders/Vertex/phong.vert");
    m_phongShader.loadShader(aie::eShaderStage::FRAGMENT, "../bin/Shaders/Fragment/phong.frag");

    directionalLight.direction = { -0.577, -0.577,-0.577 };
    directionalLight.colour = { 1,1,1 };
    m_ambientLight = { 0,0,0};

    if (!m_phongShader.link())
    {
        printf("Shader Error: %s\n", m_phongShader.getLastError());
        return false;
    }
    
    soulspearMesh.InitialiseFromFile("../bin/Models/stanford/soulspear.obj");
    soulspearMesh.LoadMaterial("../bin/Models/stanford/soulspear.mtl");
    soulspearMesh.LoadTexture("../bin/Models/stanford/soulspear_diffuse.tga");

    glm::mat4 soulspearTransform = { 0.5, 0, 0, 0,
                       0, 0.5, 0, 0,
                       0, 0, 0.5, 0,
                       0, 0, 0, 1 };

    //m_spearInstance = new Instance(soulspearTransform, &soulspearMesh, &m_phongShader, false);
    //Light light;
    //light.colour = {1,1,1};
    //light.direction = glm::vec3(1, -1, 1);

    m_scene = new Scene(camera, glm::vec2(windowWidth, windowHeight), &directionalLight, m_ambientLight);
    
    m_scene->AddInstance(new Instance(soulspearTransform, &soulspearMesh, &m_phongShader));
    glClearColor(0.25f, 0.25f, 0.25f, 1);
    glEnable(GL_DEPTH_TEST);
}

bool Application::Update(float dt)
{
    aie::ImGui_NewFrame();

    if(glfwWindowShouldClose(window) || glfwGetKey(window, GLFW_KEY_ESCAPE))
    {
        return false;
    }

    deltaTime += dt;

    camera->Update(dt, window);

    m_lastMousePosition = m_mousePosition;

    // Put your updates here
    float time = glfwGetTime();
    
    ImGui::Begin("Light Settings");
    ImGui::DragFloat3("Ambient Light", &m_ambientLight[0], 0.01f, -1.0f, 1.0f, "%.2f");
    ImGui::DragFloat3("Sunlight Direction", &directionalLight.direction[0], 0.01f, -1.0f, 1.0f);
    ImGui::DragFloat3("Sunlight Colour", &directionalLight.colour[0], 0.01f, 0.0f, 2.0f);
    ImGui::End();

    
    //ImGui::Begin("Model Setting");
    //ImGui::Checkbox("Show Soulspear", &showSoulspear);
    //ImGui::End();

    soulspearMesh.Update(dt);

    // listens for inputs
    glfwPollEvents();

    return true;

}

void Application::Draw()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    aie::Gizmos::clear();

    aie::Gizmos::addTransform(glm::mat4(1));

    //aie::Gizmos::draw(projection * view);
    glm::mat4 pv = camera->GetProjectionMatrix(windowWidth, windowHeight) * camera->GetViewMatrix();

    m_scene->Draw();
    // bind phong shader program
    //m_phongShader.bind();
    
    // bind transform and lighting
    
    //m_phongShader.bindUniform("AmbientColour", m_ambientLight);
    //m_phongShader.bindUniform("LightColour", directionalLight.colour);
    //m_phongShader.bindUniform("CameraPosition", camera->GetPosition());
    
    //m_phongShader.bindUniform("LightDirection", directionalLight.direction);
    
    

    //m_phongShader.bindUniform("pointLight1.position", pointLight1.position);
    //m_phongShader.bindUniform("pointLight1.colour", pointLight1.color);
    //m_phongShader.bindUniform("pointLight1.intensity", pointLight1.intensity);
    //m_phongShader.bindUniform("pointLight2.position", pointLight2.position);
    //m_phongShader.bindUniform("pointLight2.colour", pointLight2.color);
    //m_phongShader.bindUniform("pointLight2s.intensity", pointLight2.intensity);

    //if (showSoulspear)
    //{
        // bind transform
        //auto pvm = pv * soulspearTransform;
        //m_phongShader.bindUniform("ProjectionViewModel", pvm);
        //m_phongShader.bindUniform("ModelMatrix", soulspearTransform);
        //soulspearMesh.ApplyMaterial(&m_phongShader);
       // soulspearMesh.Draw();
        
    //}
    //m_spearInstance->Draw(camera, (float)windowWidth, (float)windowHeight, m_ambientLight, &directionalLight);

    glm::vec4 white{ 1 };
    glm::vec4 black{ 0, 0, 0, 1 };

    for (int i = 0; i < 21; ++i)
    {
        aie::Gizmos::addLine(glm::vec3{ -10 + i, 0, 10 }, glm::vec3{ -10 + i, 0, -10 }, i == 10 ? white : black);
        aie::Gizmos::addLine(glm::vec3{ 10, 0, -10 + i }, glm::vec3{ -10, 0, -10 + i }, i == 10 ? white : black);
    }

    aie::Gizmos::draw(pv);

    ImGui::Render();

    // swap double buffer windows
    glfwSwapBuffers(window);
}

void Application::Shutdown()
{
    delete camera;
    
    aie::ImGui_Shutdown();
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
