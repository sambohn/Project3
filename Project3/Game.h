#pragma once
#include "libs.h"




class Game
{
private:
// Variables
	// Window
	GLFWwindow* window;
	const int WINDOW_WIDTH;
	const int WINDOW_HEIGHT;
	int framebufferWidth;
	int framebufferHeight;

	// OpenGL Context
	const int GL_VERSION_MAJOR;
	const int GL_VERSION_MINOR;

// Private functions
	void initGLFW();
	void initWindow(const char* title, bool resizable);
	void initGLEW(); // After context creation
	void initOpenGLOptions();

// Static variables

public:
	Game(const char* title,
		const int WINDOW_WIDTH, const int WINDOW_HEIGHT,
		int GL_VERSION_MAJOR, int GL_VERSION_MINOR,
		bool resizable);

	virtual ~Game();

// Accessors
	int getWindowShouldClose();

// Modifiers
	void setWindowShouldClose();

// Functions
	void update();
	void render();

// Static functions
	static void framebuffer_resize_callback(GLFWwindow* window, int fbW, int fbH);
};

