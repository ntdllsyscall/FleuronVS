#include <common.h>
#include <engine.h>
#include <ui/ui.h>


struct FleuronEngineBase fleuron;


void initEngine(int width, int height, const char* title)
{
    fleuron.window.glfwVersion = glfwGetVersionString();
    printf("%s\n", fleuron.window.glfwVersion);

    fl_initWindowingSystem();
    fl_createWindow(width, height, title);
    fl_initRenderer();
    fl_initUI();

    return;
}

void startEngine()
{
    // Render loop
    while (!glfwWindowShouldClose(fleuron.window.ptrWindow))
    {
        
        Update();

        // check and call events and swap the buffers
        glfwPollEvents();

        fl_UI();
        

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
