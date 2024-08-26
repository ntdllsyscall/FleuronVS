#include <hWindow/input.h>
#include <engine.h>

void fl_key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    fleuron.input.keyboard.key = key;
    fleuron.input.keyboard.scancode = scancode;
    fleuron.input.keyboard.action = action;
    fleuron.input.keyboard.mods = mods;
    return;
}

void fl_cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
    fleuron.input.mouse.xPos = xpos;
    fleuron.input.mouse.yPos = ypos;
    return;
}