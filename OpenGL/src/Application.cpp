#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <fstream>
#include <sstream>

#include "VertexBufferObject.h"
#include "IndexBufferObject.h"
#include "Renderer.h"
#include "VertexArrayObject.h"

#include "Shader.h"
#include "Texture.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "ImGUI/imgui.h"
#include "ImGUI/imgui_impl_glfw.h"
#include "ImGUI/imgui_impl_opengl3.h"

int main(void)
{
	/* Initialize the library */
	if (!glfwInit())
		return -1;

	// OPEN GL 3.3
	// SET CORE
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	/* Create a windowed mode window and its OpenGL context */
	GLFWwindow* window = glfwCreateWindow(1280, 720, "Hello World", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	// set fps to monitor refresh rate
	glfwSwapInterval(1); // value=1 is vsync

	// This must be called after glfwMakeContextCurrent
	if (glewInit() != GLEW_OK) {
		std::cout << "Error init GLEW" << std::endl;
	}

	// Setup ImGUI
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();

	// Setup Platform/Renderer backends
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init(nullptr);
	// End Setup ImGUI

	std::cout << "Running GL_VERSION: " << glGetString(GL_VERSION) << std::endl;


	// Scoped
	{

		// to be put in the vertex buffer
		//float positions[] = {
		//	// positions	 // color		    // texCoords (0,0) is bottom left, (1,1) is top right
		//	-50.0f, -50.0f,  1.0f, 0.0f, 0.0f,  0.0f, 0.0f, // vertex 0
		//	 50.0f, -50.0f,  0.0f, 1.0f, 0.0f,  1.0f, 0.0f, // vertex 1
		//	 50.0f,  50.0f,  0.0f, 1.0f, 0.0f,  1.0f, 1.0f, // vertex 2
		//	-50.0f,  50.0f,  0.0f, 1.0f, 0.0f,  0.0f, 1.0f  // vertex 3
		//};

		float vertices[] = {
			// positions		   // texCoords (0,0) is bottom left, (1,1) is top right
			-0.5f, -0.5f, 0.0f,  0.0f, 0.0f, // vertex 0
			 0.5f, -0.5f, 0.0f,  1.0f, 0.0f, // vertex 1
			 0.5f,  0.5f, 0.0f,  1.0f, 1.0f, // vertex 2
			-0.5f,  0.5f, 0.0f,  0.0f, 1.0f  // vertex 3
		};

		//for (int i = 0; i < sizeof(positions) / sizeof(positions[0]); i++) {
		//	positions[i] = positions[i] * 100 + 100;
		//}

		std::cout << vertices[1] << std::endl;

		// index buffer, we do this so we don't repeat storing the same positional data in memory
		// unsigned ints is required for index buffer 
		unsigned int indices[] = { // for drawing a square :D (2 triangles)
			0, 1, 2,
			2, 3, 0
		};

		// Setup gl blending
		GLCall(glEnable(GL_BLEND));
		GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));


		// Vertex Array Buffer
		VertexArray vao;

		// VBO
		VertexBuffer vbo(vertices, sizeof(float) * 4 * 7); // 4 vertex * 7 components per vertex

		// VBO Layout
		VertexBufferLayout vbl;
		vbl.Push<float>(3); // 1-3 components pertain to the position attribute
		vbl.Push<float>(2); // 4-5 components pertain to the texture coordinates attribute

		//vbl.Push<float>(2); // 1-2 components pertain to the position attribute
		//vbl.Push<float>(3); // 3-5 components pertain to the color attribute
		//vbl.Push<float>(2); // 6-7 components pertain to the texture coordinates attribute

		vao.AddBuffer(vbo, vbl);

		IndexBuffer ibo(indices, 6);

		// The projection matrix
		// 4:3
		//glm::mat4 proj = glm::ortho(-2.0f, 2.0f, -1.5f, 1.5f);
		//glm::mat4 projection = glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f);
		// glm::perspective(glm::radians(45.0f), (float)width/(float)height, 0.1f, 100.0f);
		
		// fov, aspect ratio, near plane, far plane
		glm::mat4 projection = glm::perspective(glm::radians(45.0f), 1280.0f / 720.0f, 0.1f, 100.0f);
		//glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -3.0f)); // move the camera 100 units to the LEFT 
																			   // (or move all objects 100 to the right)

		// create multiple models for multiple objects :D
		//glm::mat4 model = glm::translate(glm::mat4(1.0), glm::vec3(200, 200, 0));
		//glm::mat4 mvp = projection * view * model;

		// relative to the project directory, defined in debugging
		Shader shader("res/shaders/Vertex.shader", "res/shaders/Frag.shader");
		Texture texture("res/textures/soldier01.png");

		// Binding textures
		texture.Bind(); // no param means bound to slot 0 
		shader.SetUniform1i("u_Texture", 0);

		// unbind everything D:
		vao.Unbind(); //GLCall(glBindVertexArray(0));
		shader.Unbind(); //glUseProgram(0);
		vbo.Unbind(); //glBindBuffer(GL_ARRAY_BUFFER, 0);
		ibo.Unbind(); //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);



		Renderer renderer;

		// Our state
		glm::vec3 cameraPosition = glm::vec3(0.0f, 0.0f, -3.0f);
		ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
		float moveSpeed = 0.01f;

		/* Loop until the user closes the window */
		while (!glfwWindowShouldClose(window))
		{
			// Poll and handle events (inputs, window resize, etc.)
			// You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
			// - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application, or clear/overwrite your copy of the mouse data.
			// - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application, or clear/overwrite your copy of the keyboard data.
			// Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
			glfwPollEvents();
			if (glfwGetWindowAttrib(window, GLFW_ICONIFIED) != 0)
			{
				ImGui_ImplGlfw_Sleep(10);
				continue;
			}

			// Start the Dear ImGui frame
			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();

			/* Render here */
			renderer.Clear();

			// Inputs

			if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
				cameraPosition.z = cameraPosition.z + moveSpeed;
			if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
				cameraPosition.z = cameraPosition.z - moveSpeed;
			if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
				cameraPosition.x = cameraPosition.x + moveSpeed;
			if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
				cameraPosition.x = cameraPosition.x - moveSpeed;

			glm::mat4 view = glm::translate(glm::mat4(1.0f), cameraPosition);

			// object 1
			shader.Bind();
			shader.SetUniform4f("u_Color", clear_color.x, clear_color.y, clear_color.z, clear_color.w); // setting the color here!
			glm::mat4 model = glm::translate(glm::mat4(1.0), glm::vec3(0.5, 0, 0));
			glm::mat4 mvp = projection * view * model;
			shader.SetUniformMat4f("u_MVP", mvp);
			renderer.Draw(vao, ibo, shader);
			

			// object 2
			shader.Bind();
			shader.SetUniform4f("u_Color", clear_color.x, clear_color.y, clear_color.z, clear_color.w); // setting the color here!
			glm::mat4 model2 = glm::mat4(1.0);
			model2 = glm::rotate(model2, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));
			glm::mat4 mvp2 = projection * view * model2;
			shader.SetUniformMat4f("u_MVP", mvp2);
			renderer.Draw(vao, ibo, shader);




			// 2. Show a simple window that we create ourselves. We use a Begin/End pair to create a named window.
			{
				static float f = 0.0f;
				static int counter = 0;

				ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

				ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)

				ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
				ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

				if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
					counter++;
				ImGui::SameLine();
				ImGui::Text("counter = %d", counter);

				ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
				ImGui::End();
			}

			// Render ImGUI
			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

			/* Swap front and back buffers */
			glfwSwapBuffers(window);

			/* Poll for and process events */
			glfwPollEvents();
		}
	}

	// Cleanup
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
	glfwTerminate();
	return 0;
}