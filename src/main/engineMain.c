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
object trig;
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

    initEngine(800, 800, "Test", "C:\\Programming\\C\\FleuronVS\\shaders\\vertex.glsl", "C:\\Programming\\C\\FleuronVS\\shaders\\fragment.glsl");
    glfwSetTime(0);
    current = glfwGetTime();

    // UI code here

    fl_uploadModel(triangle, &n, true);


    trig.p_modelIndex = &n;
    trig.transform.position[0] = 0;
    trig.transform.position[1] = 0;
    trig.transform.position[2] = 0;

    trig.transform.rotation.angle = 0.0472;
    trig.transform.rotation.axis[0] = 0;
    trig.transform.rotation.axis[1] = 0;
    trig.transform.rotation.axis[2] = 1;

    fl_pushObject(&trig);


    startEngine();


    return 0;

}

void Update()
{
    previous = current;
    current = glfwGetTime();
    glClear(GL_COLOR_BUFFER_BIT);

    fl_renderObjectTable();
    trig.transform.rotation.angle = sin(glfwGetTime());

    glClearColor(sin(glfwGetTime()), 1 - sin(glfwGetTime()), 0.6 - sin(glfwGetTime()), 1);
    //printf("FPS: %f\n", 1 / (current - previous));

}