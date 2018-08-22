#include <iostream>
#include <fstream>
#include <string>
#include <sstream>


#include "Common.h"
#include "Shader.h"
#include "Renderer.h"
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "VertexBuffer.h"


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
	window = glfwCreateWindow(640, 480, "OpenGL Tutorial", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	// Make the window's context current 
	glfwMakeContextCurrent(window);

	// the default for I guess the NVidia implementation
	// is 1, I do not have the staggering animation 
	// just for clarity I will put it anyway 
	glfwSwapInterval(1);                                                                          // this will synchronize with the vsync in order to get the transaction smoothly  (it seems I do not need it)

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
		float positions[] = {
			-0.5f, -0.5f, // 0
			 0.5f, -0.5f, // 1
			 0.5f,  0.5f, // 2
			-0.5f,  0.5f  // 3
		};

		// it has to be unsigned
		unsigned int indices[] = {
			0, 1, 2,
			2, 3, 0
		};

		VertexArray va;
		VertexBuffer vb{ positions, 4 * 2 * sizeof(float) };

		VertexBufferLayout layout;
		layout.Push<float>(2);

		va.AddBuffer(vb, layout);

		IndexBuffer ib{ indices, 6 };

		Shader shader{ "res/shaders/Basic.shader" };
		shader.Bind();

		Renderer renderer;

		float r = 0.0f;
		float increment = 0.05f;

		// Loop until the user closes the window
		while (!glfwWindowShouldClose(window))
		{
		
			renderer.Clear();

			// uniform are per draw, for now I need to do this for
			// settings uniform. One way to solve this would be using
			// materials
			shader.SetUniform4f("u_Color", r, 0.3f, 0.8f, 1.0f);

			renderer.Draw(va, ib, shader);
		
			if (r >= 1.0f)
			{
				increment = -0.05f;
			}
			else if (r < 0.0f)
			{
				increment = 0.05f;
			}

			r += increment;

			// Swap front and back buffers 
			glfwSwapBuffers(window);

			// Poll for and process events
			glfwPollEvents();
		}

	}

	glfwTerminate();

	return 0;
}
