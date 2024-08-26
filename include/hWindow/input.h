#pragma once
#include "../common.h"

struct Kbrd
{
    int key;
    int scancode;
    int action;
    int mods;
};

struct Mouse
{
    double xPos;
    double yPos;
};

struct InputBase
{
    struct Kbrd keyboard;
    struct Mouse mouse;
};

void fl_key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void fl_cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
