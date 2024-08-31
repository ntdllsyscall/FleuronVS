#include <common.h>
#include <hWindow/window.h>
#include <engine.h>
#include <debug.h>



void fl_createWindow(int width, int height, const char* title)
{
    fleuron.window.height = height;
    fleuron.window.width = width;
    fleuron.window.title = title;


    if (fleuron.window.ptrWindow != NULL)
    {
        fl_error("Can't create multiple windows \n", FL_FATAL);
    }

    fleuron.window.ptrWindow = glfwCreateWindow(width, height, title, NULL, NULL);
    if (fleuron.window.ptrWindow == NULL)
    {
        fl_error("Could not create GLFW window \n", FL_FATAL);
    }
    glfwMakeContextCurrent(fleuron.window.ptrWindow);
    glfwSetFramebufferSizeCallback(fleuron.window.ptrWindow, fl_resize_callback);
    glfwSetKeyCallback(fleuron.window.ptrWindow, fl_key_callback);
    glfwSetCursorPosCallback(fleuron.window.ptrWindow, fl_cursor_position_callback);
    return;
}

void fl_initWindowingSystem()
{
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


    fleuron.window.width = 0;
    fleuron.window.height = 0;
    fleuron.window.ptrWindow = NULL;


    return;
}

void fl_resize_callback(GLFWwindow* window, int width, int height)
{
    fleuron.window.width = width;
    fleuron.window.height = height;
    glViewport(0, 0, width, height);
    glmc_perspective((45.0f)/ 57.2958, ((float)fleuron.window.width / (float)fleuron.window.height), 0.1f, 100.0f, fleuron.renderer.matrices.projection);
    glUniformMatrix4fv(fleuron.renderer.matrices.locations.projection, 1, GL_FALSE, (float*)fleuron.renderer.matrices.projection);
    return;
}