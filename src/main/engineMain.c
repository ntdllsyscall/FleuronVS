#include <stdio.h>
#include <engine.h>
#include <hWindow/window.h>
#include <math.h>

double current;
double previous;

mesh triangle;
float cubeVert[] =
{
    
//  Vertex posision       Vertex colour(RGB)
    
    -0.5f, -0.5f, -0.5f,   1.0f, 0.0f,0.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,0.0f,
     0.5f,  0.5f, -0.5f,  0.0f, 0.0f,1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 0.0f,0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,1.0f,

    -0.5f, -0.5f,  0.5f,  1.0f, 0.0f,0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, 1.0f,0.0f,
     0.5f,  0.5f,  0.5f,  0.0f, 0.0f,1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,1.0f,

    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,0.0f,
     -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,1.0f,
    -0.5f, -0.5f, -0.5f,  1.0f, 0.0f,0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 1.0f,0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,1.0f,

     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,0.0f,
     0.5f,  0.5f, -0.5f,  0.0f, 1.0f,0.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 0.0f,1.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 0.0f,0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, 1.0f,0.0f,
     0.5f,  0.5f,  0.5f,  0.0f, 0.0f,1.0f,

    -0.5f, -0.5f, -0.5f,  1.0f, 0.0f,0.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, 0.0f,1.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 1.0f,0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,1.0f,

    -0.5f,  0.5f, -0.5f,  1.0f, 0.0f,0.0f,
     0.5f,  0.5f, -0.5f,  0.0f, 1.0f,0.0f,
     0.5f,  0.5f,  0.5f,  0.0f, 0.0f,1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 0.0f,1.0f
    
};

int ind[] =
{
    0, 2, 1,
    1, 2, 3,
    4, 5, 6,
    5, 7, 6,
    0, 1, 4,
    1, 5, 4,
    2, 6, 3,
    3, 6, 7,
    0, 4, 2,
    2, 4, 6,
    1, 3, 5,
    3, 7, 5
};

int n = -1;      // The variable to store the ID of the mesh -1 means invalid ID
object cube;

int main(void)
{
    triangle.vertices = cubeVert;
    triangle.vSize = sizeof(cubeVert);
    triangle.indices = NULL;
    triangle.iSize = 0;

    initEngine(800, 600, "Test", "C:\\Programming\\C\\FleuronVS\\shaders\\vertex.glsl", "C:\\Programming\\C\\FleuronVS\\shaders\\fragment.glsl");
    glfwSetTime(0);
    current = glfwGetTime();


    fl_uploadModel(triangle, &n, true);


    cube.p_modelIndex = &n;
    cube.transform.position[0] = 0.0f;
    cube.transform.position[1] = 0.0f;
    cube.transform.position[2] = -5.0f;

    cube.transform.rotation.angle = 0.0472f;
    cube.transform.rotation.axis[0] = 0.21f;
    cube.transform.rotation.axis[1] = 0.4f;
    cube.transform.rotation.axis[2] = 0.43f;
    //cube.transform.rotation.axis[2] = 1.0f;
    //cube.transform.rotation.axis[1] = 0.0f;
    //cube.transform.rotation.axis[0] = 0.0f;

    fl_pushObject(&cube);


    startEngine();


    return 0;

}

float r = 0.0f;


void Update()
{
    previous = current;
    current = glfwGetTime();
   

    cube.transform.rotation.angle = (r/57);
    

    r++;
    if (r > 360)
    {
        r = 0;
    }
}