#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "Common.h"
#include "Shader.h"
#include "Texture.h"
#include "Renderer.h"
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"

#include <GLFW/glfw3.h>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"


int main(void)
{
	GLFWwindow* window;

	// Initialize the library 
	if (!glfwInit())
		return -1;

	// settings OpenGL 
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Create a windowed mode window and its OpenGL context 
	window = glfwCreateWindow(960, 540, "OpenGL Tutorial", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	// Make the window's context current 
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
	// adding this scope since the program was trying
	// to delete the vb and ib in the end but the window
	// context was already destroyed and glew was raising
	// errors and those are in a infinite loop, one way to
	// solve this since I am allocating the buffer into 
	// the stack is creating a new scope
	{

		// since some vertex are in common between the triangles
		// I do not really need to store them twice so I use an
		// index buffer
		// Now I am adding to more floats, which they are the 
		// texture coordinates (u,v)
		float positions[] = {
			-50.0f, -50.0f, 0.0f, 0.0f, // 0
			 50.0f, -50.0f, 1.0f, 0.0f, // 1
			 50.0f,  50.0f, 1.0f, 1.0f, // 2
			-50.0f,  50.0f, 0.0f, 1.0f  // 3
		};

		// it has to be unsigned
		unsigned int indices[] = {
			0, 1, 2,
			2, 3, 0
		};

		// this is for blending texture but will be
		// put in an other part for now here
		GLCALL(glEnable(GL_BLEND));
		GLCALL(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

		VertexArray va;
		VertexBuffer vb{ positions, 4 * 4 * sizeof(float) };

		VertexBufferLayout layout;
		layout.Push<float>(2);       // x, z 
		layout.Push<float>(2);       // u, v

		va.AddBuffer(vb, layout);

		IndexBuffer ib{ indices, 6 };

		// 4 by 3 aspect ratio projection matrix
		glm::mat4 projection = glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f);
		glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
 
		Shader shader{ "res/shaders/Basic.shader" };
		shader.Bind();

		Texture texture{ "res/textures/b-logo.png" };
		texture.Bind();
		// binding into the shader the texture slot to
		// be used. It must correspond to what pass in
		// to texture.Bind() default is 0
		shader.SetUniform1i("u_Texture", 0);

		Renderer renderer;

		ImGui::CreateContext();
		ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui_ImplOpenGL3_Init();

		ImGui::StyleColorsDark();

		glm::vec3 translationA { 200.0f, 200.0f, 0.0f };
		glm::vec3 translationB { 400.0f, 200.0f, 0.0f };

		// Loop until the user closes the window
		while (!glfwWindowShouldClose(window))
		{
		
			renderer.Clear();

			// to be put above any imgui code for this frame
			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();

			{
				glm::mat4 model = glm::translate(glm::mat4(1.0f), translationA);
				// since in OpenGL matrix are ordered by column
				// I need to multiply the matrix in reverse 
				glm::mat4 mvp = projection * view * model;
				// uniform are per draw, for now I need to do this for
				// settings uniform. One way to solve this would be using
				// materials
				shader.SetUniformMat4f("u_MVP", mvp);
				
				renderer.Draw(va, ib, shader);
			}

			{
				glm::mat4 model = glm::translate(glm::mat4(1.0f), translationB);
				// since in OpenGL matrix are ordered by column
				// I need to multiply the matrix in reverse 
				glm::mat4 mvp = projection * view * model;
				// uniform are per draw, for now I need to do this for
				// settings uniform. One way to solve this would be using
				// materials
				shader.SetUniformMat4f("u_MVP", mvp);

				renderer.Draw(va, ib, shader);
			}
			
			{
				ImGui::SliderFloat3("Translation A", &translationA.x, 0.0f, 940.0f);
				ImGui::SliderFloat3("Translation B", &translationB.x, 0.0f, 940.0f);

				ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f /
					ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
			}

			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
			// Swap front and back buffers 
			glfwSwapBuffers(window);

			// Poll for and process events
			glfwPollEvents();
		}

	}

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwTerminate();

	return 0;
}
