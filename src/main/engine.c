#include <common.h>
#include <engine.h>
#include <ui/ui.h>


struct FleuronEngineBase fleuron;


void initEngine(int width, int height, const char* title, const char* vertexShaderSrc, const char* fragmentShaderSrc)
{
    fleuron.window.glfwVersion = glfwGetVersionString();
    printf("%s\n", fleuron.window.glfwVersion);

    fl_initWindowingSystem();
    fl_createWindow(width, height, title);

    fl_initRenderer(vertexShaderSrc, fragmentShaderSrc);
    fl_initUI();

    return;
}

void startEngine()
{
    int err = 0;
    // Render loop
    while (!glfwWindowShouldClose(fleuron.window.ptrWindow))
    {
        if ((err = glGetError()) != 0)
        {
            printf("[!] ERROR: %d\n", err);
            system("pause");
        }
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(sin(glfwGetTime()), 1 - sin(glfwGetTime()), 0.6 - sin(glfwGetTime()), 1);
        
        Update();
        

        // check and call events and swap the buffers
        glfwPollEvents();
        
        fl_renderObjectTable();
        
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
    fl_uiCleanUp();

    for (int i = 0; i < fleuron.renderer.buffers.sizeInElements; i++)
    {
        free((void*)(fleuron.renderer.buffers.meshes + i));
    }
    for (int i = 0; i < fleuron.renderer.objectTable.sizeInElements; i++)
    {
        if (fleuron.renderer.objectTable.objects->isDynamic)
        {
            free((void*)(fleuron.renderer.objectTable.objects));
        }
        fleuron.renderer.objectTable.objects = fleuron.renderer.objectTable.objects->next;
    }



    fflush(stdin);
    fflush(stdout);
    fflush(stderr);



    return;
}
