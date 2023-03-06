#include <iostream>
#include <cstdio>
#include <cstdlib>

#include "../Headers/Gui.hpp"

namespace gui {

	// Definitions of variables of the module
	//OpenGL Context
	int m_WINDOW_WIDTH = 1080;
	int m_WINDOW_HEIGHT = 720;
	const int m_GL_VERSION_MAJOR = 3;
	const int m_GL_VERSION_MINOR = 3;
	GLFWwindow* m_window;

	bool m_keys[1024]; // is a key pressed or not ?
	float m_mouse_dx, m_mouse_dy, m_mouse_scroll_dy;
	float m_mouse_x, m_mouse_y;
	bool m_mouse_moved;

	double m_avgDeltaTime = 1.0 / 60.0; // Assumes 60fps for the 1st second
	int m_lastNbFrames = 0;

	double m_timeApp = 0.0;
	double m_timeLast = 0.0;
	double m_timeDelta = 0.0;

	int m_time1sNbFrames = 0;
	bool m_time1sElapsed = false;
	double m_time1sLast = 0;
	double m_time1sDelta = 0;

	void initGLFW() {
		// Load GLFW glfwInit()
		if (glfwInit() == GLFW_FALSE)
		{
			std::cout << "ERROR::GLFW_INIT_FAILED" << "\n";
			glfwTerminate();
		}if (glfwInit() == GLFW_FALSE)
		{
			std::cout << "ERROR::GLFW_INIT_FAILED" << "\n";
			glfwTerminate();
		}
	}

	void initWindow(const char* title, 	bool resizable) {
		// Create a Window
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, m_GL_VERSION_MAJOR);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, m_GL_VERSION_MINOR);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
		glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
		m_window = glfwCreateWindow(m_WINDOW_WIDTH, m_WINDOW_HEIGHT, title, nullptr, nullptr);

		if (m_window == nullptr)
		{
			std::cout << "initWindow" << "\n";
			std::cout << "ERROR::GLFW_WINDOW_INIT_FAILED" << "\n";
			glfwTerminate();
		}
		//std::cout << "GLFW_OPENGL_PROFILE : " << GLFW_OPENGL_CORE_PROFILE << "\n";
	}

	void initFramebuffer()
	{
		//std::cout << "this->WINDOW_HEIGHT : " << WINDOW_HEIGHT << "\n";

		glfwGetFramebufferSize(m_window, &m_WINDOW_WIDTH, &m_WINDOW_HEIGHT);
		//glViewport(0, 0, this->WINDOW_WIDTH, this->WINDOW_HEIGHT);
		glfwSetFramebufferSizeCallback(m_window, framebuffer_resize_callback);
	};

	void framebuffer_resize_callback(GLFWwindow* window, int fbW, int fbH)
	{
		glViewport(0, 0, fbW, fbH);
	};

	int initGlad() {
		glfwMakeContextCurrent(m_window); //IMPORTANT!!
		gladLoadGL();

		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			std::cout << "Failed to initialize GLAD" << std::endl;
			return -1;
		}

		if (__cplusplus == 201703L) std::cout << "C++17\n";
		else if (__cplusplus == 201402L) std::cout << "C++14\n";
		else if (__cplusplus == 201103L) std::cout << "C++11\n";
		else if (__cplusplus == 199711L) std::cout << "C++98\n";
		else std::cout << "pre-standard C++\n";
	}

	void initOpenGLOptions() {
		glDepthMask(GL_TRUE);
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);
		glEnable(GL_CULL_FACE);
	}

	void setCallbacks()
	{
		//std::cout << "SETcalback " << m_window << "\n";

		glfwSetKeyCallback(m_window, key_callback);
		glfwSetCursorPosCallback(m_window, mouse_callback);
		glfwSetMouseButtonCallback(m_window, mouse_button_callback);
		glfwSetScrollCallback(m_window, scroll_callback);
	}

	//Accessor
	int getWindowShouldClose()
	{
		return glfwWindowShouldClose(m_window);
	}

	//Modifier
	void setWindowShouldClose()
	{
		glfwSetWindowShouldClose(m_window, GLFW_TRUE);
	}

	static void key_callback(GLFWwindow* window, int key, int /*scancode*/, int action, int /*mode*/)
	{
		//std::cout << "key_callback "  << "\n";
		//std::cout << key << action << "\n";

		if (action == GLFW_PRESS)
			m_keys[key] = true;
		else if (action == GLFW_RELEASE)
			m_keys[key] = false;

		if (m_keys[GLFW_KEY_ESCAPE]) {

			//std::cout << "GLFW_KEY_ESCAPE" << "\n";
		glfwSetWindowShouldClose(window, GL_TRUE);
		}

		// V-SYNC
		if (m_keys[GLFW_KEY_U]) {
			static bool vsync = true;
			if (vsync) {
				glfwSwapInterval(1);
			}
			else {
				glfwSwapInterval(0);
			}
			vsync = !vsync;
		}

		if ((m_keys[GLFW_KEY_0] || m_keys[GLFW_KEY_KP_0])) {
			std::cout << "You have pressed 0" << std::endl;
		}
	}

	static void mouse_button_callback(GLFWwindow* /*window*/, int button, int action, int /*mods*/) {
		//std::cout << "mouse_button_callback" << "\n";
		//std::cout << button << action << "\n";

		if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
			m_keys[GLFW_MOUSE_BUTTON_RIGHT] = true;
		else
			m_keys[GLFW_MOUSE_BUTTON_RIGHT] = false;

		if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
			m_keys[GLFW_MOUSE_BUTTON_LEFT] = true;
		else
			m_keys[GLFW_MOUSE_BUTTON_LEFT] = false;

		if (button == GLFW_MOUSE_BUTTON_MIDDLE && action == GLFW_PRESS)
			m_keys[GLFW_MOUSE_BUTTON_MIDDLE] = true;
		else
			m_keys[GLFW_MOUSE_BUTTON_MIDDLE] = false;
	}

	void mouse_callback(GLFWwindow* /*window*/, double xpos, double ypos)
	{
		//std::cout << "mouse_callback" << "\n";
		//std::cout << xpos << ypos << "\n";

		m_mouse_x = (float)xpos;
		m_mouse_y = (float)ypos;

		static float prev_xpos = 0.0f, prev_ypos = 0.0f;
		static bool first = true;

		if (first) {
			first = false; m_mouse_dx = 0.0f; m_mouse_dy = 0.0f;
		}
		else {
			m_mouse_dx = (float)xpos - prev_xpos; m_mouse_dy = (float)ypos - prev_ypos;
		}
		prev_xpos = (float)xpos; prev_ypos = (float)ypos;

		//if (keys[GLFW_MOUSE_BUTTON_RIGHT]) //std::cout << "Mouse Position : (" << xpos << ", " << ypos << ")" << std::endl;
		m_mouse_moved = true;
	}

	void scroll_callback(GLFWwindow* /*window*/, double /*xoffset*/, double yoffset)
	{
		//std::cout << "scroll_callback" << "\n";

		m_mouse_scroll_dy += (float)yoffset;

		//if (keys[GLFW_MOUSE_BUTTON_LEFT]) //std::cout << "Mouse Offset : " << yoffset << std::endl;
	}

	void updateMouseDisplacements()
	{
		////std::cout << "updateMouseDisplacements" << "\n";

		if (!m_mouse_moved)
			m_mouse_dx = m_mouse_dy = 0.0;
		m_mouse_moved = false;
	}

	void updateTime() {
		// Measure speed
		m_timeLast = m_timeApp;
		m_timeApp = glfwGetTime();
		m_timeDelta = m_timeApp - m_timeLast;

		m_time1sDelta = (float)(m_timeApp - m_time1sLast);
		m_time1sNbFrames++;
		if (m_time1sDelta >= 1.0) { // If last prinf() was more than 1 sec ago
			// printf and reset timer
			m_avgDeltaTime = m_time1sDelta / m_time1sNbFrames;
			m_lastNbFrames = m_time1sNbFrames;
			m_time1sNbFrames = 0;
			m_time1sLast += 1.0;
			m_time1sElapsed = true;
		}
		else
			m_time1sElapsed = false;
	}

	void showFPS() {
		if (m_time1sElapsed)
			std::cout << 1000.0 * m_time1sDelta / double(m_lastNbFrames) << " ms/frame -> " << m_lastNbFrames << " frames/sec" << std::endl;
	}

	void cameraControl(Camera& camera)
	{
//		//std::cout << camera << "\n";
		////std::cout << "cameraControl" << keys[GLFW_KEY_UP] << "\n";

		if (m_keys[GLFW_MOUSE_BUTTON_RIGHT])
			camera.ProcessMouseMovement(m_mouse_dx, -m_mouse_dy);
		camera.ProcessKeyboard(m_keys[GLFW_KEY_UP] * FORWARD | m_keys[GLFW_KEY_DOWN] * BACKWARD
			| m_keys[GLFW_KEY_LEFT] * LEFT | m_keys[GLFW_KEY_RIGHT] * RIGHT
			| m_keys[GLFW_KEY_PAGE_UP] * UP | m_keys[GLFW_KEY_PAGE_DOWN] * DOWN, m_avgDeltaTime);

		camera.ProcessMouseScroll(m_mouse_scroll_dy);
		m_mouse_scroll_dy = 0;

	}
}