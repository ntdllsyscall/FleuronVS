#include <ui/ui.h>
#include <stdio.h>

static inline void createUiContext()
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	// Load fonts here

	ImGui_ImplGlfw_InitForOpenGL(fleuron.window.ptrWindow, true);
	ImGui_ImplOpenGL3_Init();

	ImGui::StyleColorsDark();
	return;
}

// Add to start of main loop:
static inline void startFrame()
{

	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	return;
}

// Add to end of main loop:
static inline void endFrame()
{
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

// UI code goes here
void fl_initUI()
{
	createUiContext();
	return;
}


// NOTE: Look at the position of start frame and end frame
void fl_UI()
{
	startFrame();
	ImGui::Begin("Hello I am GUI!!!!!!!!");

	ImGui::Text("This is some text, please work!");
	if(ImGui::Button("LOKO LOKO POBEDA!!!!!"))
	{
		printf("Pressed\n");
	}

	ImGui::End();
	endFrame();
	return;
}

// Cleans up the UIs resources, called on the end of the program
void fl_uiCleanUp()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

}

