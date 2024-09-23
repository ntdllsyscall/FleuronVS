#pragma once
#include "../common.h"


struct Window
{
    int width;
    int height;
    GLFWwindow* ptrWindow;
    const char* glfwVersion;
    const char* title;
};


void fl_createWindow(int width, int height, const char* title);
void fl_initWindowingSystem();
void fl_resize_callback(GLFWwindow* window, int width, int height);
// This is a user defined function, it is called every frame
void Update();
void Start();