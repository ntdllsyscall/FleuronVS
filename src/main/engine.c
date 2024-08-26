#include "common.h"
#include <engine.h>


struct FleuronEngineBase fleuron;


void initEngine(int width, int height, const char* title)
{
    fleuron.window.glfwVersion = glfwGetVersionString();
    printf("%s\n", fleuron.window.glfwVersion);

    fl_initWindowingSystem();
    fl_createWindow(width, height, title);
    fl_initRenderer();

    return;
}

void startEngine()
{
    // Render loop
    while (!glfwWindowShouldClose(fleuron.window.ptrWindow))
    {
        // Process input

        Update();

        // check and call events and swap the buffers
        glfwPollEvents();
        glfwSwapBuffers(fleuron.window.ptrWindow);
    }
    cleanUp();
    exit(0);
}

void cleanUp()
{
    glfwDestroyWindow(fleuron.window.ptrWindow);
    glfwTerminate();

    return;
}
