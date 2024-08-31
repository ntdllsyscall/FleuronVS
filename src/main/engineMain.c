#include <stdio.h>
#include <engine.h>
#include <hWindow/window.h>
#include <math.h>

double current;
double previous;

mesh triangle;
float vert[] =
{
     -0.5f, -0.5f, -0.5f,
     0.5f, -0.5f, -0.5f,
     0.5f,  0.5f, -0.5f,
     0.5f,  0.5f, -0.5f,
    -0.5f,  0.5f, -0.5f,
    -0.5f, -0.5f, -0.5f,

    -0.5f, -0.5f,  0.5f,
     0.5f, -0.5f,  0.5f,
     0.5f,  0.5f,  0.5f,
     0.5f,  0.5f,  0.5f,
    -0.5f,  0.5f,  0.5f,
    -0.5f, -0.5f,  0.5f,

    -0.5f,  0.5f,  0.5f,
    -0.5f,  0.5f, -0.5f,
    -0.5f, -0.5f, -0.5f,
    -0.5f, -0.5f, -0.5f,
    -0.5f, -0.5f,  0.5f,
    -0.5f,  0.5f,  0.5f,

     0.5f,  0.5f,  0.5f,
     0.5f,  0.5f, -0.5f,
     0.5f, -0.5f, -0.5f,
     0.5f, -0.5f, -0.5f,
     0.5f, -0.5f,  0.5f,
     0.5f,  0.5f,  0.5f,

    -0.5f, -0.5f, -0.5f,
     0.5f, -0.5f, -0.5f,
     0.5f, -0.5f,  0.5f,
     0.5f, -0.5f,  0.5f,
    -0.5f, -0.5f,  0.5f,
    -0.5f, -0.5f, -0.5f,

    -0.5f,  0.5f, -0.5f,
     0.5f,  0.5f, -0.5f,
     0.5f,  0.5f,  0.5f,
     0.5f,  0.5f,  0.5f,
    -0.5f,  0.5f,  0.5f,
    -0.5f,  0.5f, -0.5f
};

int ind[] =
{
    0, 1, 2, 3, 7, 1, 5, 4, 7, 6, 2, 4, 0, 1
};

int n = -1;      // The variable to store the ID of the mesh -1 means invalid ID
object cube;

int main(void)
{
    triangle.iSize = 0;
    triangle.indices = NULL;
    triangle.vertices = vert;
    triangle.vSize = sizeof(vert);
    //triangle.indices = ind;
    //triangle.iSize = sizeof(ind);

    for (int i = 0; i < sizeof(vert) / sizeof(float); i++)
    {
        triangle.vertices[i] = vert[i];
    }

    initEngine(800, 600, "Test", "C:\\Programming\\C\\FleuronVS\\shaders\\vertex.glsl", "C:\\Programming\\C\\FleuronVS\\shaders\\fragment.glsl");
    glfwSetTime(0);
    current = glfwGetTime();


    fl_uploadModel(triangle, &n, true);


    cube.p_modelIndex = &n;
    cube.transform.position[0] = 0;
    cube.transform.position[1] = 0;
    cube.transform.position[2] = -21.4;

    cube.transform.rotation.angle = 0.0472f;
    cube.transform.rotation.axis[0] = 0.21;
    cube.transform.rotation.axis[1] = 0.4;
    cube.transform.rotation.axis[2] = 0.43;

    fl_pushObject(&cube);


    startEngine();


    return 0;

}

void Update()
{
    previous = current;
    current = glfwGetTime();
    

    
    cube.transform.rotation.angle = sin(glfwGetTime());

    
    //printf("FPS: %f\n", 1 / (current - previous));

}