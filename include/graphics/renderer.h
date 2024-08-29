#pragma once
#include "../common.h"
#include <stdbool.h>
#include "shaders.h"
#include <cglm/call.h>

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


struct Rotation
{
    float angle;
    vec3 axis;
};

struct Transform
{
    vec3 position;
    struct Rotation rotation;
};

// Add child support
// Add animation support
struct Object
{
    int* p_modelIndex; // Pointer to the index of the model used by the object
    struct Transform transform;
    struct Object* next;
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

struct RendererBuffers
{
    size_t sizeInElements; // size of the meshes array
    struct RendererMeshBase* meshes;

};

struct ObjectTableBase
{
    object* objects;    // First element in a linked list
    size_t sizeInElements;
};

struct MatrixLocationBase
{
    int rotation;
    int model;
    int projection;
    int view;
};

struct MatrixBase
{
    mat4 rotation;
    mat4 model;
    mat4 projection;
    mat4 view;

    struct MatrixLocationBase locations;
};


struct RendererBase
{
    struct RendererBuffers buffers;
    struct ShadersBase shaders;
    struct ObjectTableBase objectTable;
    struct MatrixBase matrices;
};


void fl_initRenderer(const char* vertexShaderSrc, const char* fragmentShaderSrc);
void fl_uploadModel(mesh m, int* rtnIndex, bool send);
void fl_pushObject(object* obj);
void fl_renderObjectTable();






