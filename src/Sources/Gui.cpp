#include <iostream>
#include <cstdio>
#include <cstdlib>

#include "../Headers/Gui.hpp"

namespace gui {

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
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, GL_VERSION_MAJOR);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, GL_VERSION_MINOR);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
		glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
		mWindow = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, title, nullptr, nullptr);

		if (mWindow == nullptr)
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

		glfwGetFramebufferSize(mWindow, &WINDOW_WIDTH, &WINDOW_HEIGHT);
		//glViewport(0, 0, this->WINDOW_WIDTH, this->WINDOW_HEIGHT);
		glfwSetFramebufferSizeCallback(mWindow, framebuffer_resize_callback);
	};

	void framebuffer_resize_callback(GLFWwindow* window, int fbW, int fbH)
	{
		glViewport(0, 0, fbW, fbH);
	};

	int initGlad() {
		glfwMakeContextCurrent(mWindow); //IMPORTANT!!
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
		//std::cout << "SETcalback " << mWindow << "\n";

		glfwSetKeyCallback(mWindow, key_callback);
		glfwSetCursorPosCallback(mWindow, mouse_callback);
		glfwSetMouseButtonCallback(mWindow, mouse_button_callback);
		glfwSetScrollCallback(mWindow, scroll_callback);
	}

	//Accessor
	int getWindowShouldClose()
	{
		return glfwWindowShouldClose(mWindow);
	}

	//Modifier
	void setWindowShouldClose()
	{
		glfwSetWindowShouldClose(mWindow, GLFW_TRUE);
	}

	static void key_callback(GLFWwindow* window, int key, int /*scancode*/, int action, int /*mode*/)
	{
		//std::cout << "key_callback "  << "\n";
		//std::cout << key << action << "\n";

		if (action == GLFW_PRESS)
			keys[key] = true;
		else if (action == GLFW_RELEASE)
			keys[key] = false;

		if (keys[GLFW_KEY_ESCAPE]) {

			//std::cout << "GLFW_KEY_ESCAPE" << "\n";
		glfwSetWindowShouldClose(window, GL_TRUE);
		}

		// V-SYNC
		if (keys[GLFW_KEY_U]) {
			static bool vsync = true;
			if (vsync) {
				glfwSwapInterval(1);
			}
			else {
				glfwSwapInterval(0);
			}
			vsync = !vsync;
		}

		if ((keys[GLFW_KEY_0] || keys[GLFW_KEY_KP_0])) {
			std::cout << "You have pressed 0" << std::endl;
		}
	}

	static void mouse_button_callback(GLFWwindow* /*window*/, int button, int action, int /*mods*/) {
		//std::cout << "mouse_button_callback" << "\n";
		//std::cout << button << action << "\n";

		if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
			keys[GLFW_MOUSE_BUTTON_RIGHT] = true;
		else
			keys[GLFW_MOUSE_BUTTON_RIGHT] = false;

		if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
			keys[GLFW_MOUSE_BUTTON_LEFT] = true;
		else
			keys[GLFW_MOUSE_BUTTON_LEFT] = false;

		if (button == GLFW_MOUSE_BUTTON_MIDDLE && action == GLFW_PRESS)
			keys[GLFW_MOUSE_BUTTON_MIDDLE] = true;
		else
			keys[GLFW_MOUSE_BUTTON_MIDDLE] = false;
	}

	void mouse_callback(GLFWwindow* /*window*/, double xpos, double ypos)
	{
		//std::cout << "mouse_callback" << "\n";
		//std::cout << xpos << ypos << "\n";

		mouse_x = (float)xpos;
		mouse_y = (float)ypos;

		static float prev_xpos = 0.0f, prev_ypos = 0.0f;
		static bool first = true;

		if (first) {
			first = false; mouse_dx = 0.0f; mouse_dy = 0.0f;
		}
		else {
			mouse_dx = (float)xpos - prev_xpos; mouse_dy = (float)ypos - prev_ypos;
		}
		prev_xpos = (float)xpos; prev_ypos = (float)ypos;

		//if (keys[GLFW_MOUSE_BUTTON_RIGHT]) //std::cout << "Mouse Position : (" << xpos << ", " << ypos << ")" << std::endl;
		mouse_moved = true;
	}

	void scroll_callback(GLFWwindow* /*window*/, double /*xoffset*/, double yoffset)
	{
		//std::cout << "scroll_callback" << "\n";

		mouse_scroll_dy += (float)yoffset;

		//if (keys[GLFW_MOUSE_BUTTON_LEFT]) //std::cout << "Mouse Offset : " << yoffset << std::endl;
	}

	void updateMouseDisplacements()
	{
		////std::cout << "updateMouseDisplacements" << "\n";

		if (!mouse_moved)
			mouse_dx = mouse_dy = 0.0;
		mouse_moved = false;
	}

	void updateTime() {
		// Measure speed
		timeLast = timeApp;
		timeApp = glfwGetTime();
		timeDelta = timeApp - timeLast;

		time1sDelta = (float)(timeApp - time1sLast);
		time1sNbFrames++;
		if (time1sDelta >= 1.0) { // If last prinf() was more than 1 sec ago
			// printf and reset timer
			avgDeltaTime = time1sDelta / time1sNbFrames;
			lastNbFrames = time1sNbFrames;
			time1sNbFrames = 0;
			time1sLast += 1.0;
			time1sElapsed = true;
		}
		else
			time1sElapsed = false;
	}

	void showFPS() {
		if (time1sElapsed)
			std::cout << 1000.0 * time1sDelta / double(lastNbFrames) << " ms/frame -> " << lastNbFrames << " frames/sec" << std::endl;
	}

	void cameraControl(Camera& camera)
	{
//		//std::cout << camera << "\n";
		////std::cout << "cameraControl" << keys[GLFW_KEY_UP] << "\n";

		if (keys[GLFW_MOUSE_BUTTON_RIGHT])
			camera.ProcessMouseMovement(mouse_dx, -mouse_dy);
		camera.ProcessKeyboard(keys[GLFW_KEY_UP] * FORWARD | keys[GLFW_KEY_DOWN] * BACKWARD
			| keys[GLFW_KEY_LEFT] * LEFT | keys[GLFW_KEY_RIGHT] * RIGHT
			| keys[GLFW_KEY_PAGE_UP] * UP | keys[GLFW_KEY_PAGE_DOWN] * DOWN, avgDeltaTime);

		camera.ProcessMouseScroll(mouse_scroll_dy);
		mouse_scroll_dy = 0;

	}

	// Definitions of variables of the module
	//OpenGL Context
	int WINDOW_WIDTH = 1080;
	int WINDOW_HEIGHT = 720;
	const int GL_VERSION_MAJOR = 3;
	const int GL_VERSION_MINOR = 3;
	GLFWwindow* mWindow;

	bool keys[1024]; // is a key pressed or not ?
	float mouse_dx, mouse_dy, mouse_scroll_dy;
	float mouse_x, mouse_y;
	bool mouse_moved;

	double timeApp = 0.0;
	double timeLast = 0.0;
	double timeDelta = 0.0;

	double avgDeltaTime = 1.0 / 60.0; // Assumes 60fps for the 1st second
	int lastNbFrames = 0;

	int time1sNbFrames = 0;
	bool time1sElapsed = false;
	double time1sLast = 0;
	double time1sDelta = 0;
}