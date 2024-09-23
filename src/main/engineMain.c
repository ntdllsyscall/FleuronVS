#include <stdio.h>
#include <engine.h>
#include <hWindow/window.h>
#include <math.h>

double current;
double previous;

int main(void)
{
    initEngine(800, 600, "Test", "C:\\Programming\\C\\FleuronVS\\shaders\\vertex.glsl", "C:\\Programming\\C\\FleuronVS\\shaders\\fragment.glsl");
    glfwSetTime(0);
    current = glfwGetTime();
    startEngine();

    return 0;

}

