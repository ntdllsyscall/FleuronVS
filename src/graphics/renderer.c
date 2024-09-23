#include <graphics/renderer.h>
#include <debug.h>
#include <engine.h>

// The renderer handles buffer objects, rendering, etc
#define rad(x) (x/57.2958f)
#define r fleuron.renderer

void fl_initRenderer(const char* vertexShaderSrc, const char* fragmentShaderSrc)
{
    // Make sure to init the renderer struct here too.
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        // implement a cleanup function
        fl_error("Could not initialize OpenGL \n", FL_FATAL);
    }
    glViewport(0, 0, fleuron.window.width, fleuron.window.height);
    
    
    // Shader handling
    fl_compileVertexShader(vertexShaderSrc);
    fl_compileFragmentShader(fragmentShaderSrc);
    fl_createProgram();

    r.buffers.meshes = NULL;
    r.buffers.sizeInElements = 0;
    r.objectTable.objects = NULL;
    r.objectTable.sizeInElements = 0;

    glmc_mat4_identity(fleuron.renderer.matrices.rotation);
    glmc_mat4_identity(fleuron.renderer.matrices.model);
    glmc_mat4_identity(fleuron.renderer.matrices.projection);
    glmc_mat4_identity(fleuron.renderer.matrices.view);

    glmc_perspective(rad(45.0f), ((float)fleuron.window.width / (float)fleuron.window.height), 0.1f, 100.0f, r.matrices.projection);

    glUniformMatrix4fv(fleuron.renderer.matrices.locations.rotation, 1, GL_FALSE, (float*)fleuron.renderer.matrices.rotation);
    glUniformMatrix4fv(fleuron.renderer.matrices.locations.model, 1, GL_FALSE, (float*)fleuron.renderer.matrices.model);
    glUniformMatrix4fv(fleuron.renderer.matrices.locations.view, 1, GL_FALSE, (float*)fleuron.renderer.matrices.view);
    glUniformMatrix4fv(fleuron.renderer.matrices.locations.projection, 1, GL_FALSE, (float*)fleuron.renderer.matrices.projection);
    
    glEnable(GL_DEPTH_TEST);

    return;
}


// ----------------------------------------- DEBUGING ------------------------------------------------ //


static inline printMat4(mat4 m)
{
    for (int i = 1; i < 5; i++)
    {
        for (int j = 1; j < 5; j++)
        {
            printf("%f ", m[j][i]);
        }
        printf("\n");
    }
    printf("\n");
    return 0;
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
        struct RendererMeshBase* check = (struct RendererMeshBase*)realloc(r.buffers.meshes, (r.buffers.sizeInElements + 1) * sizeof(struct RendererMeshBase));
        r.buffers.sizeInElements++;
        if (check == NULL)
        {
            fl_error("Could not reallocate memory for r.buffers.meshes struct", FL_FATAL);
        }
        r.buffers.meshes = (struct RendererMeshBase*)check;

        r.buffers.meshes[r.buffers.sizeInElements].vbo.ID = 0;
        r.buffers.meshes[r.buffers.sizeInElements].vbo.size = 0;
        r.buffers.meshes[r.buffers.sizeInElements].vbo.vertices = NULL;

        r.buffers.meshes[r.buffers.sizeInElements].vao.ID = 0;

        r.buffers.meshes[r.buffers.sizeInElements].ebo.ID = 0;
        r.buffers.meshes[r.buffers.sizeInElements].ebo.size = 0;
        r.buffers.meshes[r.buffers.sizeInElements].ebo.indices = NULL;
        r.buffers.meshes[r.buffers.sizeInElements].ebo.isUsed = 2;

        *i = r.buffers.sizeInElements - (size_t)1;
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

    free((void*)(r.buffers.meshes[i].vbo.vertices));
    if (r.buffers.meshes[i].ebo.isUsed == true)
    {
        free((void*)(r.buffers.meshes[i].ebo.indices));
    }

    *(r.buffers.meshes[i].pIndex) = -1;    // Making the handle invalid
    r.buffers.meshes[i] = r.buffers.meshes[r.buffers.sizeInElements - 1];
    *(r.buffers.meshes[i].pIndex) = i;
    struct RendererMeshBase* check = (struct RendererMeshBase*)realloc(r.buffers.meshes, (r.buffers.sizeInElements - 1) * sizeof(struct RendererMeshBase));
    if (check == NULL)
    {
        fl_error("Could not remove mesh memory, realloc returned null", FL_FATAL);
    }
    r.buffers.meshes = (struct RendererMeshBase*)check;
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

// Copies from src to dst size bytes of type int
static inline void cpyInt(int* dst, int* src, size_t size)
{
    for (int i = 0; i < size / sizeof(int); i++)
    {
        dst[i] = src[i];
    }
    return;
}

// Binds the VBO of a mesh with index of index
static inline void bindVbo(int index)
{
    glBindBuffer(GL_ARRAY_BUFFER, r.buffers.meshes[index].vbo.ID);

}
// Binds the VAO of a mesh with index of index
static inline void bindVao(int index)
{
    glBindVertexArray(r.buffers.meshes[index].vao.ID);

}
// Binds the EBO of a mesh with index of index
static inline void bindEbo(int index)
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, r.buffers.meshes[index].ebo.ID);

}

static inline void unbindVbo()
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

static inline void unbindVao()
{
    glBindVertexArray(0);

}

static inline void unbindEbo()
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

}

// Loads or concatenates a Mesh/Model to the Mesh object of index of i
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

        float* check = (float*)realloc(r.buffers.meshes[i].vbo.vertices, r.buffers.meshes[i].vbo.size + m.vSize);
        if (check == NULL)
        {
            printf("Failed to reallocate memory for the vertices with mesh ID of %d\n", i);
            fl_error("\n", FL_FATAL);
        }
        r.buffers.meshes[i].vbo.vertices = check;
        #ifdef DEBUG
        printf("Concatenating a mesh to another one with index of %d ...\n", i);
        #endif

        cpyFloat(r.buffers.meshes[i].vbo.vertices + r.buffers.meshes[i].vbo.size, m.vertices, m.vSize);

        r.buffers.meshes[i].vbo.size += m.vSize;
        #ifdef DEBUG
        printf("Done!\n");
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
            int* check = (int*)realloc(r.buffers.meshes[i].ebo.indices, r.buffers.meshes[i].ebo.size + m.iSize);
            if (check == NULL)
            {
                printf("Failed to reallocate memory for the indices with mesh ID of %d\n", i);
                fl_error("\n", FL_FATAL);
            }
            r.buffers.meshes[i].ebo.indices = check;

            cpyInt(r.buffers.meshes[i].ebo.indices + r.buffers.meshes[i].ebo.size, m.indices, m.iSize);

            r.buffers.meshes[i].ebo.size += m.iSize;
            return;

        }
    }

    return;
}

// Calls loadModel() and sends the model to the GPU with the appropriate settings if send is true
// This function is user accessible. loadModel() should not be used manually, thus it is static
void fl_uploadModel(mesh m, int* rtnIndex, bool send)
{
    if (send == true)
    {
        allocMeshMem(rtnIndex);
    }
    loadModel(m, *rtnIndex);

    // TODO: VBO configuration (Temporary)
    if (send == true)
    {
        bindVao(*rtnIndex);
        bindVbo(*rtnIndex);
        bindEbo(*rtnIndex);

        glBufferData(GL_ARRAY_BUFFER, r.buffers.meshes[*rtnIndex].vbo.size, r.buffers.meshes[*rtnIndex].vbo.vertices, GL_STATIC_DRAW);

        if (r.buffers.meshes[*rtnIndex].ebo.isUsed == true)
        {
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, r.buffers.meshes[*rtnIndex].ebo.size, r.buffers.meshes[*rtnIndex].ebo.indices, GL_STATIC_DRAW);
        }

        // position attribute
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        // color attribute
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        unbindVao();
        unbindEbo();
        unbindVbo();

        return;
    }

    return;
}

// Sends the mesh of index i to the rendering pipeline
// This all boils down to if the Engine architecture allows for rendering without having filled all of the vertex attributes
// This would require an array of shader programs each processing a certain ammount of vertex attributes.
static inline void sendToPipeline(int i)
{
    bindVao(i);
    glUseProgram(r.shaders.program);
    if (r.buffers.meshes[i].ebo.isUsed == true)
    {
        glDrawElements(GL_TRIANGLES, ((r.buffers.meshes[i].ebo.size) / (sizeof(int))), GL_UNSIGNED_INT, 0);
    }
    else
    {
        // TODO: This should be changed as more vertex attributes are added!
        glDrawArrays(GL_TRIANGLES, 0, (r.buffers.meshes[i].vbo.size) / (sizeof(float) * 6));
    }

    unbindVao();
    return;
}
                            // -----------------------------    Object Handling    ------------------------------ //
// 0 indexed
static inline object* getObjAt(size_t index)
{
    if (index >= r.objectTable.sizeInElements || index < 0)
    {
        return NULL;
    }
    object* current = r.objectTable.objects;
    for (int i = 0; i < index; i++)
    {
        current = current->next;
    }
    return current;
}


// Appends an object to the linked list in the object table
void fl_pushObject(object* obj)
{
    object* current = r.objectTable.objects;
    if (current == NULL)
    {
        r.objectTable.objects = obj;
        obj->next = NULL;
        r.objectTable.sizeInElements++;
        return;
    }
    else
    {
        for (int i = 0; i < r.objectTable.sizeInElements - 1; i++)
        {
            current = current->next;
        }
        current->next = obj;
        obj->next = NULL;
        r.objectTable.sizeInElements++;
        return;
    }
    
}

// Should not be called by the user
void fl_renderObjectTable()
{
    object* current;
    for (int i = 0; (current = getObjAt(i)) != NULL; i++)
    {
        
        // Matrices
        glmc_rotate(r.matrices.rotation, current->transform.rotation.angle, current->transform.rotation.axis);
        glmc_translate(r.matrices.model, current->transform.position);        

        glUniformMatrix4fv(r.matrices.locations.rotation, 1, GL_FALSE, (float*)r.matrices.rotation);
        glUniformMatrix4fv(r.matrices.locations.model, 1, GL_FALSE, (float*)r.matrices.model);
        
        glmc_mat4_identity(r.matrices.rotation);
        glmc_mat4_identity(r.matrices.model);

        // TODO: Future support of child objects should be added;
        sendToPipeline(*(current->p_modelIndex));

    }

    return;
}




