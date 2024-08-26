#pragma once
#include "common.h"
#include "hWindow/window.h"
#include "graphics/renderer.h"
#include "hWindow/input.h"



struct FleuronEngineBase
{
    struct Window window;
    struct InputBase input;
    struct RendererBase renderer;
};

extern struct FleuronEngineBase fleuron;
    

// This function will be user called in order to:
// Create a Window and initialize GLFW
// Set the context, callbacks and other things for OpenGL
void initEngine(int width, int height, const char* title);
// Called at the end of the program or when fatal errors happen
void cleanUp();
void startEngine();