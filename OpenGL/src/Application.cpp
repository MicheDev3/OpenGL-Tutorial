#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "Common.h"
#include "Renderer.h"

#include <GLFW/glfw3.h>

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include "tests/TestClearColor.h"


int main(void)
{
	GLFWwindow* window;

	if (!glfwInit()) 
	{
		return -1;
	}

	// settings OpenGL properties
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window = glfwCreateWindow(960, 540, "OpenGL Tutorial", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}
 
	glfwMakeContextCurrent(window);

	// the default for, I guess, the NVidia implementation
	// is 1, I do not have the staggering animation 
	// just for clarity I will put it anyway 
	// this will synchronize with the vsync in order to
	// get the transaction smoothly 
	glfwSwapInterval(1); 

	// glewInit must be called right after 
	// a context has be made
	if (glewInit() != GLEW_OK)
	{
		std::cout << "Error\n";
	}

	std::cout << glGetString(GL_VERSION) << '\n';
	{
		
		// this is for blending texture but will be
		// put in an other part for now here
		GLCALL(glEnable(GL_BLEND));
		GLCALL(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

		Renderer renderer;

		ImGui::CreateContext();
		ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui_ImplOpenGL3_Init();

		ImGui::StyleColorsDark();

		test::Test* currentTest = nullptr;
		auto* testMenu = new test::TestMenu(currentTest);
		currentTest = testMenu;

		testMenu->RegisterTest<test::TestClearColor>("Clear Color");

		while (!glfwWindowShouldClose(window))
		{
			// to be moved out
			GLCALL(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));

			renderer.Clear();

			ImGui_ImplOpenGL3_NewFrame();  // to be put above any imgui code for this frame
			ImGui_ImplGlfw_NewFrame();     // to be put above any imgui code for this frame
			ImGui::NewFrame();             // to be put above any imgui code for this frame

			if (currentTest) 
			{
				currentTest->OnUpdate(0.0f);
				currentTest->OnRender();
				ImGui::Begin("Test");
				if (currentTest != testMenu && ImGui::Button("<-")) 
				{
					delete currentTest;
					currentTest = testMenu;
				}
				currentTest->OnImGuiRender();
				ImGui::End();
			}

			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

			glfwSwapBuffers(window);

			glfwPollEvents();
		}

		delete currentTest;
		if (testMenu != currentTest)
		{
			delete testMenu;
		}

	}

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwTerminate();

	return 0;
}
