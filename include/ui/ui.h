#pragma once
#include <common.h>
#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

extern "C"
{
	#include "../engine.h"
}

extern "C"
{
	void fl_initUI();
	void fl_UI();
	void fl_uiCleanUp();
};