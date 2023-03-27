#pragma once

// Preprocessor Directives
#ifndef GUI
#define GUI

// System Headers
#include "glitter.hpp"

// Local Headers
#include "Camera.hpp"

namespace gui {

	//OpenGL Context
	extern int WINDOW_WIDTH;
	extern int WINDOW_HEIGHT;
	extern const int GL_VERSION_MAJOR;
	extern const int GL_VERSION_MINOR;
	
	extern GLFWwindow* window;

	extern bool keyDown[1024]; // is key down or not ?
	extern bool prevKeyDown[1024]; // was key down or not ?
	extern bool keyPressed[1024]; // was key pressed during last frame (one shot) ?
	
	extern float mouse_x, mouse_y;
	extern float mouse_dx, mouse_dy, mouse_scroll_dy;
	extern bool mouse_moved;

	extern double timeApp;
	extern double timeLast;
	extern double timeDelta;

	extern double avgDeltaTime;
	extern int lastNbFrames;

	extern int time1sNbFrames;
	extern bool time1sElapsed;
	extern double time1sLast;
	extern double time1sDelta;

	/// GLFW hooks
	void initGLFW();
	void setCallbacks();
	void initWindow(const char* title, bool resizable);
	void framebuffer_resize_callback(GLFWwindow* window, int fbW, int fbH);
	void initFramebuffer();
	int initGlad();
	void setCallbacks();
	void initOpenGLOptions();

	//Accessors
	int getWindowShouldClose();

	//Modifiers
	void setWindowShouldClose();

	/// Key / Mouse Inputs
	void key_callback(GLFWwindow* window, int key, int /*scancode*/, int action, int /*mode*/);
	void mouse_button_callback(GLFWwindow* /*window*/, int button, int action, int /*mods*/);
	void mouse_callback(GLFWwindow* window, double xpos, double ypos);
	void scroll_callback(GLFWwindow* window, double /*xoffset*/, double yoffset);

	void updateMouseDisplacements();
	void updateKeyPress();

	/// Time management
	void updateTime();
	void showFPS();

	void setMouseUserCallback(void (*cb)(void));
	extern void (*mouseUserCallback)(void);

}
#endif
