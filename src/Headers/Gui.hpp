// Preprocessor Directives
#ifndef GUI
#define GUI
#pragma once

// System Headers
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// Local Headers
//#include "glitter.hpp"
#include "Camera.hpp"

namespace gui {

	//OpenGL Context
	extern int m_WINDOW_WIDTH;
	extern int m_WINDOW_HEIGHT;
	extern const int m_GL_VERSION_MAJOR;
	extern const int m_GL_VERSION_MINOR;
	extern GLFWwindow* m_window;

	extern bool m_keys[1024]; // is a key pressed or not ?
	extern float m_mouse_dx, m_mouse_dy, m_mouse_scroll_dy;
	extern float m_mouse_x, m_mouse_y;
	extern bool m_mouse_moved;

	extern double m_timeApp;
	extern double m_timeLast;
	extern double m_timeDelta;

	extern double m_avgDeltaTime;
	extern int m_lastNbFrames;

	extern int m_time1sNbFrames;
	extern bool m_time1sElapsed;
	extern double m_time1sLast;
	extern double m_time1sDelta;

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

	/// Time management
	void updateTime();
	void showFPS();

	/// Camera management
	void cameraControl(Camera * camera);

	void setMouseUserCallback(void (*cb)(void));
	extern void (*mouseUserCallback)(void);

}
#endif
