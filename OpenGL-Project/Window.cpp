#include "Window.h"

bool Window::shouldClose() {
	return glfwWindowShouldClose(mainWindow);
}

void Window::swapBuffers() {
	glfwSwapBuffers(mainWindow);
}

GLint Window::getBufferWidth() {
	return bufferWidth;
}

GLint Window::getBufferHeight() {
	return bufferHeight;
}

Window::Window() {
	width = 800;
	height = 600;

	for (size_t i = 0; i < 1024; i++) {
		keys[i] = false;
	}

	mouseFirstMoved = true;

	xChange = 0.0f;
	yChange = 0.0f;
}

Window::Window(GLint windowWidth, GLint windowHeight) {
	width = windowWidth;
	height = windowHeight;

	for (size_t i = 0; i < 1024; i++) {
		keys[i] = false;
	}

	mouseFirstMoved = true;

	xChange = 0.0f;
	yChange = 0.0f;
}

int Window::initialize() {
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
	mainWindow = glfwCreateWindow(width, height, "Test Window", NULL, NULL);

	//check if the window was created
	if (!mainWindow) {
		printf("Failed to create a window");
		glfwTerminate();
		return 1;
	}

	//get buffer size information
	glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);

	//Set glew context window
	glfwMakeContextCurrent(mainWindow);

	//set callbacks
	createCallbacks();
	glfwSetInputMode(mainWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	//allow modern extension features
	glewExperimental = GL_TRUE;

	//initialize glew
	if (glewInit() != GLEW_OK) {
		printf("Failed to initialize GLEW");
		glfwDestroyWindow(mainWindow);
		glfwTerminate();
		return 1;
	}

	glEnable(GL_DEPTH_TEST);

	//set viewport size
	glViewport(0, 0, bufferWidth, bufferHeight);

	glfwSetWindowUserPointer(mainWindow, this);
}

Window::~Window() {
	glfwDestroyWindow(mainWindow);
	glfwTerminate();
}

void Window::createCallbacks() {
	glfwSetKeyCallback(mainWindow, handleKeys);
	glfwSetCursorPosCallback(mainWindow, handleMouse);
}

void Window::handleKeys(GLFWwindow* window, int key, int code, int action, int mode) {
	Window* currentWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));

	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, GL_TRUE);
	}

	if (key >= 0 && key < 1024) {
		if (action == GLFW_PRESS) {
			currentWindow->keys[key] = true;
		}
		else if (action == GLFW_RELEASE) {
			currentWindow->keys[key] = false;
		}
	}
}

void Window::handleMouse(GLFWwindow* window, double xPos, double yPos) {
	Window* currentWindow = static_cast<Window*>(glfwGetWindowUserPointer(window));

	if (currentWindow->mouseFirstMoved) {
		currentWindow->lastX = xPos;
		currentWindow->lastY = yPos;
		currentWindow->mouseFirstMoved = false;
	}

	currentWindow->xChange = xPos - currentWindow->lastX;
	currentWindow->yChange = currentWindow->lastY - yPos;

	currentWindow->lastX = xPos;
	currentWindow->lastY = yPos;
}

bool* Window::getKeys() {
	return keys;
}

GLfloat Window::getXchange() {
	GLfloat delta = xChange;
	xChange = 0.0f;
	return delta;
}

GLfloat Window::getYchange() {
	GLfloat delta = yChange;
	yChange = 0.0f;
	return delta;
}