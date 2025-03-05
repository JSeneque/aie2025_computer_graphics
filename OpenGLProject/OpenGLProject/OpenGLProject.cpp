// OpenGLProject.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "Application.h"
#include <chrono>

int main()
{
    std::chrono::high_resolution_clock timer;
    auto start = timer.now();
    double deltaTime = 0.0f;

    Application* app = new Application;
    if (app->Startup()) {
        while (app->Update(deltaTime))
        {
            app->Draw();

            auto stop = timer.now();
            deltaTime = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start).count() / 10.0f;
            std::cout << deltaTime << std::endl;
            start = timer.now();
        }
        app->Shutdown();
    }

    delete app;

    return 0;
}



