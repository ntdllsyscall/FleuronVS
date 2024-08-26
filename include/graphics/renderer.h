#pragma once
#include "../common.h"
#include <stdbool.h>
#include "shaders.h"

// A handle to a mesh in memory. Used to modify attributes
// Do not allocate dynamically on your own, use the built in function instead or unexpected behaviour may occur
// This handle is a long long int wrapper made to be more intuitive; It is basically and index in an array of meshes


struct Mesh
{
    float* vertices;
    int* indices;
    size_t vSize; // bytes
    size_t iSize; // bytes
    GLenum usage;
};
typedef struct Mesh mesh;

// Essentially a vec3
struct Position
{
    float xPos;
    float yPos;
    float zPos;
};

struct Rotation
{
    float angle;
    // Add axis( vec3 );
};

struct Transform
{
    struct Position position;
    struct Rotation rotation;
};


struct Object
{
    size_t modelIndex; // The index of the model used by the object

};
typedef struct Object object;

struct VBO
{
    size_t size;        // For concatenation purposes; 8 bytes
    float* vertices;    // 8 bytes
    unsigned int ID;    // 4 bytes
    // Thus no padding; size - 20 bytes;
};

struct VAO
{
    unsigned int ID;    // 4 bytes
};
struct EBO
{
    size_t size;        // For concatenation purposes; 8 bytes
    int* indices;       // 8 bytes
    unsigned int ID;    // 4 bytes
    bool isUsed;        // 0 - not used
    // 1 - used
    // 2 - uninitialized
};

typedef struct VBO VBO;
typedef struct VAO VAO;
typedef struct EBO EBO;

struct RendererMeshBase
{
    VBO vbo;
    VAO vao;
    EBO ebo;
    int* pIndex;    // Used during relocation of the mesh in memory so that we keep track of the index where it is located in the array;
    //This value should preferably be staticly allocated
};

// A collection of those 3 buffers is a mesh
struct RendererBuffers
{
    size_t sizeInElements; // size of the meshes array
    struct RendererMeshBase* meshes;

};

struct RendererBase
{
    struct RendererBuffers buffers;
    struct ShadersBase shaders;
};


void fl_initRenderer();
void fl_uploadModel(mesh m, int* rtnIndex, bool send);








