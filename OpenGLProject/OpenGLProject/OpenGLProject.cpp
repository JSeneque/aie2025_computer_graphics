// OpenGLProject.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "Application.h"


int main()
{
    Application* app = new Application;
    if (app->Startup()) {
        while (app->Update())
        {
            app->Draw();
        }
        app->Shutdown();
    }

    delete app;

    return 0;
}



