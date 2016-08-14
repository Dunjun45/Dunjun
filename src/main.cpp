#include <Dungun/Common.hpp>
#include <gl/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>

// function pointer that takes in int and unsign int
typedef void(*GENBUFFERS) (GLsizei, GLuint*);

GLOBAL const int g_windowWidth = 854;
GLOBAL const int g_windowHeight = 480;

void glfwHints()
{
	glfwWindowHint(GLFW_VERSION_MAJOR, 2);
	glfwWindowHint(GLFW_VERSION_MINOR, 1);
}

int main(int argc, char** argv)
{
//	if (!glewInit())
//		return EXIT_FAILURE;

	GLFWwindow* window;

	if (!glfwInit())
		return EXIT_FAILURE;

	glfwHints();
	window = glfwCreateWindow(g_windowWidth, g_windowHeight, "Dunjun", nullptr, nullptr);
	if (!window)
	{
		glfwTerminate();
		return EXIT_FAILURE;
	}

	glfwMakeContextCurrent(window);

	// This creates all the openGL pointers at runtime
	glewInit();

	float vertices[] = {
//        x      y
		+0.0f, +0.5f, // Vertex 1
		-0.5f, -0.5f, // Vextex 2 
		+0.5f, -0.5f  // Vextex 3
	};


	GLuint vbo; // Vertex Buffer Object - holders all the information of these indices
	glGenBuffers(1, &vbo); // Generate a buffers
	glBindBuffer(GL_ARRAY_BUFFER, vbo); // We have binded vbo to the opengl buffer

	// GL_STATIC_DRAW tells opengl what this buffer data is use for:
	
	/* GL_STATIC_DRAW  - Vertex data only uploaded once
	   GL_DYNAMIC_DRAW - Vertex will change every so offen e.g animation
	   GL_STREAM_DRAW  - Vertex will be change every everytime
	 */
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); // Assign the vertices

	// Source code for vertexshader
	const char* vertexShaderText = {
		"#version 120\n"
		"\n"
		"attribute vec2 position;"
		"void main()"
		"{"
		"	gl_Position = vec4(position, 0.0, 1.0);"
		"}"
	};

	// Source for fragmentshader
	const char* fragmentShaderText = {
		"#version 120\n"
		"\n"
		"void main()"
		"{"
		"	gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0);"
		"}"
	};

	// Create the vertexShader, This is the position on memory
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	// Load the Shader from source code; 1 here means only one piece of source
	glShaderSource(vertexShader, 1, &vertexShaderText, nullptr);
	glCompileShader(vertexShader);

	// Create fragmentShader
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderText, nullptr);
	glCompileShader(fragmentShader);

	// Create a shderProgram so that we can attach all the little shader to each other
	GLuint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);

	// After we can attach the shader, we and bind the variable position to which location of the BufferData
	glBindAttribLocation(shaderProgram, 0, "position");
	glLinkProgram(shaderProgram);
	glUseProgram(shaderProgram);



	bool running = true;
	bool fullscreen = false;
	while (running)
	{
		glClearColor(0.5f, 0.69f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);


		// Draw Things
		{
			// Enble openGL to use the attriArray of position '0'
			glEnableVertexAttribArray(0);
			// It starts at pos 0, it's an array of 2d and the values is float; its not normalize and there is no strife
			glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
			glDrawArrays(GL_TRIANGLES, 0, 3);

			glDisableVertexAttribArray(0);
		}
		glfwSwapBuffers(window);
		glfwPollEvents();

		glfwHints();
		if (glfwWindowShouldClose(window) ||
			glfwGetKey(window, GLFW_KEY_ESCAPE))
			running = false;

		if (glfwGetKey(window, GLFW_KEY_F11))
		{ 
			fullscreen = !fullscreen;
			GLFWwindow* newWindow;

			if (fullscreen)
			{
				int count;
				const GLFWvidmode* modes = glfwGetVideoModes(glfwGetPrimaryMonitor(), &count);
				
				newWindow = glfwCreateWindow(modes[count - 1].width, modes[count - 1].height, "Dunjun", glfwGetPrimaryMonitor(), window);
			}
			else
			{
				newWindow = glfwCreateWindow(g_windowWidth, g_windowHeight, "Dunjun", nullptr, window);
			}

			glfwDestroyWindow(window);
			window = newWindow;
			glfwMakeContextCurrent(window);
		}

	}

	glfwDestroyWindow(window);
	glfwTerminate();
	return EXIT_SUCCESS;
}