


#include "..\lib\glad\glad.h"
#include "..\lib\GLFW\glfw3.h"
#include <xaudio2.h>
#include <stdio.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "synesthetic.h"
#include "audio_engine.cpp"

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 800


GLFWwindow* window;

void frameBufferResizeCallback(GLFWwindow* window, int width, int height)
{
	glViewport(0,0, width, height);
}


int main()
{
	/* Initialize OpenGL */
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);

	window = glfwCreateWindow( SCREEN_WIDTH, SCREEN_HEIGHT, "Synesthetic", 0,0);

	if(!window)
	{
		puts("Failed to create OpenGl Window");
		return -1;
	}

	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, frameBufferResizeCallback );

	if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		puts("Failed to initialize GLAD\n");
		return -1;
	}

	/* Initialize XAudio */
	

	SynestheticMainLoop();


	puts("Hello World!");
	return 1;
}

void Render()
{
	glClearColor(0.0f,0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	glfwSwapBuffers(window);
	glfwPollEvents();
}

void ProcessInput(application_state* state)
{
	if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		state->running = false;
	}
	
}