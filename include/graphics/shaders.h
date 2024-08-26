#pragma once
#include "../common.h"


void fl_compileVertexShader(const char* path);
void fl_compileFragmentShader(const char* path);
void fl_createProgram();

struct ShadersBase
{
    unsigned int vertexShader;
    unsigned int fragmentShader;
    unsigned int program;
};
