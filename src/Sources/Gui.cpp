#include "../Headers/Gui.hpp"

namespace gui {

	// Definitions of variables of the module
	//OpenGL Context
	int WINDOW_WIDTH = 1080;
	int WINDOW_HEIGHT = 720;
	const int GL_VERSION_MAJOR = 3;
	const int GL_VERSION_MINOR = 3;
	
	GLFWwindow* window;

	bool keyDown[1024]; // is key down or not ?
	bool prevKeyDown[1024]; // was key down or not ?
	bool keyPressed[1024]; // was key pressed during last frame (one shot) ?

	float mouse_x = WINDOW_WIDTH / 2;
	float mouse_y = WINDOW_HEIGHT / 2;
	float mouse_dx, mouse_dy, mouse_scroll_dy;
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

	void (*mouseUserCallback)(void);

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
		window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, title, nullptr, nullptr);

		if (window == nullptr)
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

		glfwGetFramebufferSize(window, &WINDOW_WIDTH, &WINDOW_HEIGHT);
		//glViewport(0, 0, this->WINDOW_WIDTH, this->WINDOW_HEIGHT);
		glfwSetFramebufferSizeCallback(window, framebuffer_resize_callback);
	};

	void framebuffer_resize_callback(GLFWwindow* window, int fbW, int fbH)
	{
		glViewport(0, 0, fbW, fbH);
	};

	int initGlad() {
		glfwMakeContextCurrent(window); //IMPORTANT!!
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
		//std::cout << "SETcalback " << window << "\n";

		glfwSetKeyCallback(window, key_callback);
		glfwSetCursorPosCallback(window, mouse_callback);
		glfwSetMouseButtonCallback(window, mouse_button_callback);
		glfwSetScrollCallback(window, scroll_callback);
	}

	//Accessor
	int getWindowShouldClose()
	{
		return glfwWindowShouldClose(window);
	}

	//Modifier
	void setWindowShouldClose()
	{
		glfwSetWindowShouldClose(window, GLFW_TRUE);
	}

	static void key_callback(GLFWwindow* window, int key, int /*scancode*/, int action, int /*mode*/)
	{
		//std::cout << "key_callback "  << "\n";
		//std::cout << key << action << "\n";

		if (action == GLFW_PRESS)
			keyDown[key] = true;
		else if (action == GLFW_RELEASE)
			keyDown[key] = false;

		if (keyDown[GLFW_KEY_ESCAPE]) {

			//std::cout << "GLFW_KEY_ESCAPE" << "\n";
		glfwSetWindowShouldClose(window, GL_TRUE);
		}

		// V-SYNC
		if (keyDown[GLFW_KEY_U]) {
			static bool vsync = true;
			if (vsync) {
				glfwSwapInterval(1);
			}
			else {
				glfwSwapInterval(0);
			}
			vsync = !vsync;
		}

		if ((keyDown[GLFW_KEY_0] || keyDown[GLFW_KEY_KP_0])) {
			std::cout << "You have pressed 0" << std::endl;
		}
	}
	
	void updateKeyPress()
	{
		for (int i = 0; i < 1024; ++i) {
			keyPressed[i] = !prevKeyDown[i] && keyDown[i];
			prevKeyDown[i] = keyDown[i];
		}
	}

	static void mouse_button_callback(GLFWwindow* /*window*/, int button, int action, int /*mods*/) {
		//std::cout << "mouse_button_callback" << "\n";
		//std::cout << button << action << "\n";

		if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
			keyDown[GLFW_MOUSE_BUTTON_RIGHT] = true;
		else
			keyDown[GLFW_MOUSE_BUTTON_RIGHT] = false;

		if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
			keyDown[GLFW_MOUSE_BUTTON_LEFT] = true;
		else
			keyDown[GLFW_MOUSE_BUTTON_LEFT] = false;

		if (button == GLFW_MOUSE_BUTTON_MIDDLE && action == GLFW_PRESS)
			keyDown[GLFW_MOUSE_BUTTON_MIDDLE] = true;
		else
			keyDown[GLFW_MOUSE_BUTTON_MIDDLE] = false;
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
			//std::cout << "mouse_callback"<<  "mouse_dx " << mouse_dx << "mouse_dy " << mouse_dy << "\n";
		}
		prev_xpos = (float)xpos; prev_ypos = (float)ypos;

		//if (keys[GLFW_MOUSE_BUTTON_RIGHT]) //std::cout << "Mouse Position : (" << xpos << ", " << ypos << ")" << std::endl;
		mouse_moved = true;
	}

	void scroll_callback(GLFWwindow* /*window*/, double /*xoffset*/, double yoffset)
	{
		//std::cout << "scroll_callback" << "\n";

		mouse_scroll_dy += (float)yoffset;
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
}