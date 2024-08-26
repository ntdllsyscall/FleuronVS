#include <graphics/renderer.h>
#include <debug.h>
#include <engine.h>

// The renderer handles buffer objects, rendering, etc

#define r fleuron.renderer

void fl_initRenderer()
{
    // Make sure to init the renderer struct here too.
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        // implement a cleanup function
        fl_error("Could not initialize OpenGL \n", FL_FATAL);
    }
    glViewport(0, 0, fleuron.window.width, fleuron.window.height);

    return;
}




// ---BUFFER HANDLING---

// Allocates space in memory to store all of the attributes of a mesh. Sets the Index to the mesh
// Creates the OpenGL objects
static inline void allocMeshMem(int* i)
{
    if (r.buffers.meshes == NULL)
    {
        r.buffers.meshes = (struct RendererMeshBase*)malloc(sizeof(struct RendererMeshBase));
        r.buffers.sizeInElements = 1;
        if (r.buffers.meshes == NULL)
        {
            fl_error("Could not allocate memory for r.buffers.meshes struct", FL_FATAL);
        }
        r.buffers.meshes[0].vbo.ID = 0;
        r.buffers.meshes[0].vbo.size = 0;
        r.buffers.meshes[0].vbo.vertices = NULL;

        r.buffers.meshes[0].vao.ID = 0;

        r.buffers.meshes[0].ebo.ID = 0;
        r.buffers.meshes[0].ebo.size = 0;
        r.buffers.meshes[0].ebo.indices = NULL;
        r.buffers.meshes[0].ebo.isUsed = 2;

        *i = 0;
        r.buffers.meshes[0].pIndex = i;

        // Error checking; DEBUG
        glGenBuffers(1, &(r.buffers.meshes[0].vbo.ID));
        glGenBuffers(1, &(r.buffers.meshes[0].ebo.ID));
        glGenVertexArrays(1, &(r.buffers.meshes[0].vao.ID));

    }
    else
    {
        r.buffers.meshes = (struct RendererMeshBase*)realloc(r.buffers.meshes, (r.buffers.sizeInElements + 1) * sizeof(struct RendererMeshBase));
        r.buffers.sizeInElements++;
        if (r.buffers.meshes == NULL)
        {
            fl_error("Could not reallocate memory for r.buffers.meshes struct", FL_FATAL);
        }
        r.buffers.meshes[r.buffers.sizeInElements].vbo.ID = 0;
        r.buffers.meshes[r.buffers.sizeInElements].vbo.size = 0;
        r.buffers.meshes[r.buffers.sizeInElements].vbo.vertices = NULL;

        r.buffers.meshes[r.buffers.sizeInElements].vao.ID = 0;

        r.buffers.meshes[r.buffers.sizeInElements].ebo.ID = 0;
        r.buffers.meshes[r.buffers.sizeInElements].ebo.size = 0;
        r.buffers.meshes[r.buffers.sizeInElements].ebo.indices = NULL;
        r.buffers.meshes[r.buffers.sizeInElements].ebo.isUsed = 2;

        *i = r.buffers.sizeInElements - 1;
        r.buffers.meshes[r.buffers.sizeInElements].pIndex = i;

        glGenBuffers(1, &(r.buffers.meshes[*i].vbo.ID));
        glGenBuffers(1, &(r.buffers.meshes[*i].ebo.ID));
        glGenVertexArrays(1, &(r.buffers.meshes[*i].vao.ID));

    }

    return;
}

// Untested but probably works
static inline void removeMeshMem(int i)
{
    glDeleteBuffers(1, &(r.buffers.meshes[i].vbo.ID));
    glDeleteBuffers(1, &(r.buffers.meshes[i].ebo.ID));
    glDeleteVertexArrays(1, &(r.buffers.meshes[i].vao.ID));

    free(r.buffers.meshes[i].vbo.vertices);
    if (r.buffers.meshes[i].ebo.isUsed == true)
    {
        free(r.buffers.meshes[i].ebo.indices);
    }

    *(r.buffers.meshes[i].pIndex) = -1;    // Making the handle invalid
    r.buffers.meshes[i] = r.buffers.meshes[r.buffers.sizeInElements - 1];
    *(r.buffers.meshes[i].pIndex) = i;
    r.buffers.meshes = realloc(r.buffers.meshes, (r.buffers.sizeInElements - 1) * sizeof(struct RendererMeshBase));
    r.buffers.sizeInElements--;


    return;
}

// Copies from src to dst size bytes of type float
static inline void cpyFloat(float* dst, float* src, size_t size)
{
    for (int i = 0; i < size / (sizeof(float)); i++)
    {
        dst[i] = src[i];
    }
    return;
}

static inline void cpyInt(int* dst, int* src, size_t size)
{
    for (int i = 0; i < size / sizeof(int); i++)
    {
        dst[i] = src[i];
    }
    return;
}


static inline void bindVbo(int index)
{
    glBindBuffer(GL_ARRAY_BUFFER, r.buffers.meshes[index].vbo.ID);
#ifdef DEBUG
    printf("Vbo bound: %d\n", index);
#endif
}
// Binds the VAO of a mesh with index of index
static inline void bindVao(int index)
{
    glBindVertexArray(r.buffers.meshes[index].vao.ID);
#ifdef DEBUG
    printf("Vao bound: %d\n", index);
#endif
}
// Binds the EBO of a mesh with index of index
static inline void bindEbo(int index)
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, r.buffers.meshes[index].ebo.ID);
#ifdef DEBUG
    printf("Ebo bound: %d\n", index);
#endif
}

static inline void unbindVbo()
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
#ifdef DEBUG
    printf("Vbo unbound\n");
#endif
}

static inline void unbindVao()
{
    glBindVertexArray(0);
#ifdef DEBUG
    printf("Vao unbound\n");
#endif
}

static inline void unbindEbo()
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
#ifdef DEBUG
    printf("Ebo unbound\n");
#endif
}

// Loads a Mesh/Model to the Mesh object of index of i
// This function only loads it into RAM and does not send it to the GPU
// This function is cruical
static void loadModel(mesh m, int i)
{
    if (m.vSize < 1)
    {
        fl_error("No mesh to be concatenated!", FL_WARNING);
        return;
    }

    if (i >= r.buffers.sizeInElements || i < 0)
    {
        fl_error("Invalid mesh ID to concatenate to!\n", FL_WARNING);
    }

    // Concatenation of vertices to a VBO
    if (r.buffers.meshes[i].vbo.size < 1)
    {
        #ifdef DEBUG
        printf("Adding vertices from a mesh to a renderer VBO object with index of %d ...\n", i);
        #endif

        r.buffers.meshes[i].vbo.vertices = (float*)malloc(m.vSize);
        if (r.buffers.meshes[i].vbo.vertices == NULL)
        {
            printf("Failed to allocate memory for the vertices with mesh ID of %d\n", i);
            fl_error("\n", FL_FATAL);
        }
        cpyFloat(r.buffers.meshes[i].vbo.vertices, m.vertices, m.vSize);

        r.buffers.meshes[i].vbo.size = m.vSize;
        #ifdef DEBUG
        printf("Done\n");
        #endif

    }
    else if (r.buffers.meshes[i].vbo.size > 0)
    {

        r.buffers.meshes[i].vbo.vertices = (float*)realloc(r.buffers.meshes[i].vbo.vertices, r.buffers.meshes[i].vbo.size + m.vSize);
        if (r.buffers.meshes[i].vbo.vertices == NULL)
        {
            printf("Failed to reallocate memory for the vertices with mesh ID of %d\n", i);
            fl_error("\n", FL_FATAL);
        }
        #ifdef DEBUG
        printf("Concatenating a mesh to another one with index of %d ...\n", i);
        #endif

        cpyFloat(r.buffers.meshes[i].vbo.vertices + r.buffers.meshes[i].vbo.size, m.vertices, m.vSize);

        r.buffers.meshes[i].vbo.size += m.vSize;
        #ifdef DEBUG
        printf("Done\n");
        #endif
    }

    // Concatenation of indices(if any) to an EBO
    if (m.iSize < 1)
    {
        if (r.buffers.meshes[i].ebo.size < 1)
        {
            #ifdef DEBUG
            printf("EBO with mesh index of %d will not be used\n", i);
            #endif
            r.buffers.meshes[i].ebo.isUsed = false;
            return;
        }

        #ifdef DEBUG
        printf("No EBO with mesh index of %d concatenation will occur\n", i);
        #endif
        return;

    }
    else if (m.iSize > 0)
    {

        if (r.buffers.meshes[i].ebo.size < 1)
        {
            #ifdef DEBUG
            printf("Loading indices to a mesh with index of %d\n", i);
            #endif

            r.buffers.meshes[i].ebo.isUsed = true;
            r.buffers.meshes[i].ebo.indices = (int*)malloc(m.iSize);
            if (r.buffers.meshes[i].ebo.indices == NULL)
            {
                printf("Failed to allocate memory for the indices with mesh ID of %d\n", i);
                fl_error("\n", FL_FATAL);
            }

            cpyInt(r.buffers.meshes[i].ebo.indices, m.indices, m.iSize);

            r.buffers.meshes[i].ebo.size = m.iSize;
            return;
        }
        else
        {
            #ifdef DEBUG
            printf("Concatenating indices to a mesh with index of %d \n", i);
            #endif

            r.buffers.meshes[i].ebo.isUsed = true;
            r.buffers.meshes[i].ebo.indices = (int*)realloc(r.buffers.meshes[i].ebo.indices, r.buffers.meshes[i].ebo.size + m.iSize);
            if (r.buffers.meshes[i].ebo.indices == NULL)
            {
                printf("Failed to reallocate memory for the indices with mesh ID of %d\n", i);
                fl_error("\n", FL_FATAL);
            }

            cpyInt(r.buffers.meshes[i].ebo.indices + r.buffers.meshes[i].ebo.size, m.indices, m.iSize);

            r.buffers.meshes[i].ebo.size += m.iSize;
            return;

        }
    }

    return;
}

// Calls loadModel() and sends the model to the GPU with the appropriate settings if send is true
// This function is user accessible. loadModel() should not be used manually, thus it is static
// The mesh should have dynamically allocated arrays (verrtices and indices)
void fl_uploadModel(mesh m, int* rtnIndex, bool send)
{
    allocMeshMem(rtnIndex);
    loadModel(m, *rtnIndex);

    // VBO configuration (Temporary)
    if (send == true)
    {
        bindVao(*rtnIndex);
        bindVbo(*rtnIndex);
        bindEbo(*rtnIndex);

        glBufferData(GL_ARRAY_BUFFER, r.buffers.meshes[*rtnIndex].vbo.size, r.buffers.meshes[*rtnIndex].vbo.vertices, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        unbindVao();
        unbindEbo();
        unbindVbo();

        return;
    }

    return;
}

// Sends the mesh of index i to the rendering pipeline
static inline void sendToPipeline(int i)
{
    bindVao(i);
    glUseProgram(r.shaders.program);
    if (r.buffers.meshes[i].ebo.isUsed == true)
    {
        glDrawElements(GL_TRIANGLES, (r.buffers.meshes[i].ebo.size) / (sizeof(int)), GL_UNSIGNED_INT, 0);
    }
    else
    {
        glDrawArrays(GL_TRIANGLES, 0, (r.buffers.meshes[i].vbo.size) / (sizeof(float) * 3));
    }

    unbindVao();
    return;
}

