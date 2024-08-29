#include <graphics/shaders.h>
#include <engine.h>
#include <debug.h>

#define r fleuron.renderer.shaders

// Reads a file of filename fName to destination char** dst
// Memory is automatically allocated but needs to be manually freed
static inline void readFile(const char* fName, char** dst)
{
    FILE* fd = fopen(fName, "r");
    fseek(fd, 0, SEEK_END);

    *dst = (char*)malloc(ftell(fd));
    if (*dst == NULL || dst == NULL)
    {
        fl_error("Could not allocate memory for file", FL_FATAL);
    }
#ifdef DEBUG
    printf("Allocated %ld bytes for a file to be read: \n", ftell(fd));
#endif
    fseek(fd, 0, SEEK_SET);
    char c = 0;
    for (int i = 0; (c = getc(fd)) != EOF; i++)
    {
        (*dst)[i] = c;
        (*dst)[i + 1] = '\0';
    }
    fclose(fd);
#ifdef DEBUG
    printf("File read: \n%s\n", *dst);
#endif

    return;
}


void fl_compileVertexShader(const char* path)
{
    char* src = NULL;
    r.vertexShader = glCreateShader(GL_VERTEX_SHADER);
    readFile(path, &src);
    glShaderSource(r.vertexShader, 1, (const GLchar* const*)&src, NULL);
    glCompileShader(r.vertexShader);


    int  success;
    char infoLog[512];
    glGetShaderiv(r.vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(r.vertexShader, 512, NULL, infoLog);
        free(src);
        fl_error(infoLog, FL_FATAL);
        return;
    }
    fl_error("Successfully compiled vertex shader!\n", FL_NOTICE);
    free(src);
    return;
}

void fl_compileFragmentShader(const char* path)
{
    char* src = NULL;
    r.fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    readFile(path, &src);
    glShaderSource(r.fragmentShader, 1, (const GLchar* const*)&src, NULL);
    glCompileShader(r.fragmentShader);

    int  success;
    char infoLog[512];
    glGetShaderiv(r.fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(r.fragmentShader, 512, NULL, infoLog);
        free(src);
        fl_error(infoLog, FL_FATAL);
        return;
    }
    fl_error("Successfully compiled fragment shader!\n", FL_NOTICE);
    free(src);
    return;
}

void fl_createProgram()
{
    r.program = glCreateProgram();
    glAttachShader(r.program, r.vertexShader);
    glAttachShader(r.program, r.fragmentShader);
    glLinkProgram(r.program);

    int success;
    char infoLog[512];
    glGetProgramiv(r.program, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(r.program, 512, NULL, infoLog);
        fl_error(infoLog, FL_FATAL);
        return;
    }
    glDeleteShader(r.vertexShader);
    glDeleteShader(r.fragmentShader);
    printf("Successfully linked the shader program!\n");
    
    glUseProgram(r.program);

    fleuron.renderer.matrices.locations.rotation = glGetUniformLocation(r.program, "rotation");
    fleuron.renderer.matrices.locations.model=  glGetUniformLocation(r.program, "model");
    fleuron.renderer.matrices.locations.projection =  glGetUniformLocation(r.program, "projection");
    fleuron.renderer.matrices.locations.view =  glGetUniformLocation(r.program, "view");

    if ((success = glGetError()) != 0 )
    {
        fl_error("OpenGl error: fl_createProgram(); %d", success);
    }
    return;
}
