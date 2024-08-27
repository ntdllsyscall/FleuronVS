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

static int meshIDInput;
static bool displayInfo;	// VBO examiner
static bool displayInfo1;


// Helps with examining buffers of different kind, used for debugging
static inline void bufferHandler()
{
	ImGui::Begin("Buffer examiner");

	ImGui::Text("Number of meshes: %d", fleuron.renderer.buffers.sizeInElements);
	ImGui::InputInt("Enter a mesh ID", &meshIDInput);
	
	
	if(ImGui::Button("Examine") || displayInfo == true)
	{
		// !!!!!!!!
		ImGui::BeginChild("##scrolling", ImVec2(0, 100), true);
		if (meshIDInput >= fleuron.renderer.buffers.sizeInElements)
		{
			ImGui::Text("Invalid ID (Too large)");
			displayInfo = true;
			if (ImGui::Button("Clear"))
			{
				displayInfo = false;
			}
		}else
		{
			//fleuron.renderer.buffers.meshes[1].vbo.size

			displayInfo = true;
			ImGui::Text("Vbo size: %d", fleuron.renderer.buffers.meshes[meshIDInput].vbo.size);
			ImGui::Text("Ebo size: %d", fleuron.renderer.buffers.meshes[meshIDInput].ebo.size);
			ImGui::Text("Vbo id: %d", fleuron.renderer.buffers.meshes[meshIDInput].vbo.ID);
			ImGui::Text("Ebo id: %d", fleuron.renderer.buffers.meshes[meshIDInput].ebo.ID);
			ImGui::Text("Vao id: %d", fleuron.renderer.buffers.meshes[meshIDInput].vao.ID);

			if(ImGui::Button("Clear"))
			{
				displayInfo = false;
			}
		}
		ImGui::EndChild();
	}

	if(ImGui::Button("View vertices") || displayInfo1 == true)
	{
		ImGui::BeginChild("##scrolling1", ImVec2(0, 100), true);
		if (meshIDInput >= fleuron.renderer.buffers.sizeInElements)
		{
			ImGui::Text("Invalid ID (Too large)");
			displayInfo1 = true;
			if (ImGui::Button("Clear"))
			{
				displayInfo1 = false;
			}
		}
		else
		{
			for (int i = 0; i < (fleuron.renderer.buffers.meshes[meshIDInput].vbo.size) / (sizeof(float)); i++)
			{
				if(i % 3 == 0)
				{
					ImGui::Text("%.3f ", fleuron.renderer.buffers.meshes[meshIDInput].vbo.vertices[i]);
				}
				else
				{
					ImGui::SameLine();
					ImGui::Text("%.3f ", fleuron.renderer.buffers.meshes[meshIDInput].vbo.vertices[i]);
				}
			}

			displayInfo1 = true;
			if (ImGui::Button("Clear"))
			{
				displayInfo1 = false;
			}
		}
		ImGui::EndChild();
	}



	ImGui::End();

	return;
}

// NOTE: Look at the position of start frame and end frame
void fl_UI()
{
	startFrame();
	
	bufferHandler();

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

