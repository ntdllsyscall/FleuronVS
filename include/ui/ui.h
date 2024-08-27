#pragma once
#ifdef __cplusplus
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
#undef FLEURON_GUI_SOURCE
#endif	// !__cplusplus

/*
extern "C"
{
	#include "../engine.h"
}
*/
#ifndef __cplusplus
	void fl_initUI();
	void fl_UI();
	void fl_uiCleanUp();
#endif // !__cplusplus



/*
extern "C"
{
	void fl_initUI();
	void fl_UI();
	void fl_uiCleanUp();
};
*/