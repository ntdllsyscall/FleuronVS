#include <stdio.h>
#include <engine.h>
#include <hWindow/window.h>
#include <math.h>

double current;
double previous;

mesh triangle;
float vert[] =
{
    -0.5f, -0.5f, 0.0f,
     0.5f, -0.5f, 0.0f,
     0.0f,  0.5f, 0.0f
};
int n = -1;      // The variable to store the ID of the mesh -1 means invalid ID

int main(void)
{
    triangle.iSize = 0;
    triangle.indices = NULL;
    triangle.vertices = vert;
    triangle.vSize = sizeof(vert);

    for (int i = 0; i < sizeof(vert) / sizeof(float); i++)
    {
        triangle.vertices[i] = vert[i];
    }

    initEngine(800, 800, "Test");
    glfwSetTime(0);
    current = glfwGetTime();

    fl_compileVertexShader("C:\\Programming\\C\\FleuronVS\\shaders\\vertex.glsl");
    fl_compileFragmentShader("C:\\Programming\\C\\FleuronVS\\shaders\\fragment.glsl");
    fl_createProgram();


    fl_uploadModel(triangle, &n, true);

    startEngine();




    return 0;

}

void Update()
{
    previous = current;
    current = glfwGetTime();
    glClear(GL_COLOR_BUFFER_BIT);

    glUseProgram(fleuron.renderer.shaders.program);
    glBindVertexArray(fleuron.renderer.buffers.meshes[n].vao.ID);

    glDrawArrays(GL_TRIANGLES, 0, 3);

    glClearColor(sin(glfwGetTime()), 1 - sin(glfwGetTime()), 0.6 - sin(glfwGetTime()), 1);
    //printf("FPS: %f\n", 1 / (current - previous));

}