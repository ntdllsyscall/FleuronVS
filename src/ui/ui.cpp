#include <ui/ui.h>
#include <stdio.h>

static inline object* getObjAt(size_t index)
{
	if (index >= fleuron.renderer.objectTable.sizeInElements || index < 0)
	{
		return NULL;
	}
	object* current = fleuron.renderer.objectTable.objects;
	for (int i = 0; i < index; i++)
	{
		current = current->next;
	}
	return current;
}


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



// Helps with examining buffers of different kind, used for debugging
// Opotimize checking meshIDInput >= sizeInElements
static bool displayBufferHandler = false;
static bool displayObjectHandler = false;

static inline void Debug()
{
	ImGui::Begin("Debug");

	if (ImGui::Button("Buffer handler"))
	{
		displayBufferHandler = true;
	}
	if (ImGui::Button("Object handler"))
	{
		displayObjectHandler = true;
	}

	ImGui::End();
}

static int objIndex;

static void objectHandler()
{
	if (displayObjectHandler == true)
	{
		ImGui::Begin("Object handler", &displayObjectHandler);
		ImGui::InputInt("Object index", &objIndex);
		
		if (objIndex >= fleuron.renderer.objectTable.sizeInElements || objIndex < 0)
		{
			ImGui::Text("Invalid index (Too large or < 0)");
		}
		else
		{
			object* current = getObjAt(objIndex);

			ImGui::BeginChild("##scrolling2", ImVec2(0, 200));

			ImGui::Text("transform:");
			ImGui::Text("	position:");
		
			ImGui::Text("	");
			ImGui::SameLine();
			ImGui::InputFloat3("xyz;", current->transform.position);
			

			ImGui::Text("	rotation:");
			ImGui::Text("	");
			ImGui::SameLine();
			
			ImGui::InputFloat("angle", &(current->transform.rotation.angle));
			ImGui::Text("	");
			ImGui::SameLine();
			ImGui::InputFloat3("axis", current->transform.rotation.axis);
			
			ImGui::Text("	");
			ImGui::SameLine();
			if (ImGui::Button("Normalize axis"))
			{
				glm_vec3_normalize(current->transform.rotation.axis);
			}

			ImGui::EndChild();
		}


		ImGui::End();
	}
}






static int meshIDInput;
static bool displayInfo;	// Examiner
static bool displayInfo1;	// Vertices
static bool displayInfo2;	// Indices

static inline void bufferHandler()
{
	if(displayBufferHandler == true)
	{
		ImGui::Begin("Buffer examiner", &displayBufferHandler);

		ImGui::Text("Number of meshes: %d", fleuron.renderer.buffers.sizeInElements);
		ImGui::InputInt("Enter a mesh ID", &meshIDInput);

		if (meshIDInput >= fleuron.renderer.buffers.sizeInElements)
		{
			ImGui::Text("Invalid ID (Too large)");
		}
		else
		{

			if (ImGui::Button("Examine") || displayInfo == true)
			{
				// !!!!!!!!
				ImGui::BeginChild("##scrolling", ImVec2(0, 100), true);

				{
					//fleuron.renderer.buffers.meshes[1].vbo.size

					displayInfo = true;
					ImGui::Text("Vbo size: %d", fleuron.renderer.buffers.meshes[meshIDInput].vbo.size);
					ImGui::Text("Ebo size: %d", fleuron.renderer.buffers.meshes[meshIDInput].ebo.size);
					ImGui::Text("Vbo id: %d", fleuron.renderer.buffers.meshes[meshIDInput].vbo.ID);
					ImGui::Text("Ebo id: %d", fleuron.renderer.buffers.meshes[meshIDInput].ebo.ID);
					ImGui::Text("Vao id: %d", fleuron.renderer.buffers.meshes[meshIDInput].vao.ID);

					if (ImGui::Button("Clear"))
					{
						displayInfo = false;
					}
				}
				ImGui::EndChild();
			}

			if (ImGui::Button("View vertices") || displayInfo1 == true)
			{
				ImGui::BeginChild("##scrolling1", ImVec2(0, 100), true);

				{
					for (int i = 0; i < (fleuron.renderer.buffers.meshes[meshIDInput].vbo.size) / (sizeof(float)); i++)
					{
						if (i % 3 == 0)
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

			if (ImGui::Button("View indices") || displayInfo2 == true)
			{

				ImGui::BeginChild("##scrolling2", ImVec2(0, 200), true);


				{
					displayInfo2 = true;

					for (int i = 0; i < (fleuron.renderer.buffers.meshes[meshIDInput].ebo.size) / sizeof(int); i++)
					{
						if (i % 3 == 0)
						{
							ImGui::Text("%d", fleuron.renderer.buffers.meshes[meshIDInput].ebo.indices[i]);
						}
						else
						{
							ImGui::SameLine();
							ImGui::Text("%d", fleuron.renderer.buffers.meshes[meshIDInput].ebo.indices[i]);
						}
					}


					if (ImGui::Button("Clear"))
					{
						displayInfo2 = false;
					}
				}
				ImGui::EndChild();
			}
		}


		ImGui::End();
	}

	return;
}



// NOTE: Look at the position of start frame and end frame
void fl_UI()
{
	startFrame();
	
	Debug();
	bufferHandler();
	objectHandler();

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

