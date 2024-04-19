#include <stdio.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

//Window dimensions
const GLint WIDTH = 800, HEIGHT = 600;

int main() {
	//Initialize GLFW
	if (!glfwInit()) {
		printf("Failed to initialize GLFW");
		glfwTerminate();
		return 1;
	}

	//Setup GLFW Window Properties
	//Set open gl verison 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	//disable legacy features
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//enable forward compatibility
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	//create the window
	GLFWwindow* mainWindow = glfwCreateWindow(WIDTH, HEIGHT, "Test Window", NULL, NULL);

	//check if the window was created
	if (!mainWindow) {
		printf("Failed to create a window");
		glfwTerminate();
		return 1;
	}

	//get buffer size information
	int bufferWidth, bufferHeight;
	glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);

	//Set glew context window
	glfwMakeContextCurrent(mainWindow);

	//allow modern extension features
	glewExperimental = GL_TRUE;

	//initialize glew
	if (glewInit() != GLEW_OK) {
		printf("Failed to initialize GLEW");
		glfwDestroyWindow(mainWindow);
		glfwTerminate();
		return 1;
	}

	//set viewport size
	glViewport(0,0,bufferWidth, bufferHeight);

	//Main Loop
	while (!glfwWindowShouldClose(mainWindow)) {
		glfwPollEvents();

		//clear window
		glClearColor(0.0f, 0.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glfwSwapBuffers(mainWindow);
	}

	return 0;
}