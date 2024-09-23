#include <stdio.h>
#include <engine.h>
#include <hWindow/window.h>
#include <math.h>
#include <resources/cube.h>



mesh Cube;

int n = -1;      // The variable to store the ID of the mesh -1 means invalid ID
object cube1;

void Start()
{
    
    Cube.vertices = fl_cube_coloured;
    Cube.vSize = sizeof(fl_cube_coloured);
    Cube.indices = NULL;
    Cube.iSize = 0;

    fl_uploadModel(Cube, &n, true);

    cube1.p_modelIndex = &n;
    cube1.transform.position[0] = 0.0f;
    cube1.transform.position[1] = 0.0f;
    cube1.transform.position[2] = -5.0f;

    cube1.transform.rotation.angle = 0.0472f;
    cube1.transform.rotation.axis[0] = 0.21f;
    cube1.transform.rotation.axis[1] = 0.4f;
    cube1.transform.rotation.axis[2] = 0.43f;

    fl_pushObject(&cube1);
    

}


float r = 0.0f;


void Update()
{

    cube1.transform.rotation.angle = (r / 57);

    r++;
    if (r > 360)
    {
        r = 0;
    }
}

