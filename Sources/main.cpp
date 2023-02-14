// Local Headers
#include "glitter.hpp"

// System Headers
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// Standard Headers
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <vector>

//Model
#include "Shader.hpp"
#include "Model.hpp"

//Matrix
#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4, glm::ivec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/type_ptr.hpp> // glm::value_ptr

//Camera
#include "Camera.hpp"
#include <glm/gtc/matrix_transform.hpp>
Camera camera(glm::vec3(-10.0f, 100.0f, 150.0f));
GLfloat lastX = 400, lastY = 300;
bool firstMouse = true;
float deltaTime = 0.0f;


//--------------------------------------------------------------------------------------------------------------------------------------------------
// Callbacks
static bool keys[1024]; // is a key pressed or not ?
// External static callback
// Is called whenever a key is pressed/released via GLFW
static void key_callback(GLFWwindow* window, int key, int /*scancode*/, int action, int /*mode*/);
static void mouse_button_callback(GLFWwindow* /*window*/, int button, int action, int /*mods*/);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double /*xoffset*/, double yoffset);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void Do_Movement();

void showFPS(void);

// create VAO
GLuint createLightVAO();
GLuint createAxisVAO();

// Load Texture
GLuint loadTexture(const char* path);
GLuint loadCubemap(std::vector<std::string> faces);

// set Shader
void setMVPmatrix(Shader pShader, glm::mat4 matModel, glm::mat4 matProjection, glm::mat4 matView);
void setShadeDirectLight(Shader pShader, glm::vec3 dirLightDirection, glm::vec3 dirLightAmbient, glm::vec3 dirLightDiffuse, glm::vec3 dirLightSpecular, glm::vec3 dirLightColor);

float Friction = 0.9;
float Restitution = 0.5;

//--------------------------------------------------------------------------------------------------------------------------------------------------
// Object Positions
glm::vec3 pointLightPositions[] = {
	glm::vec3(80.0 , 40.0f, 41.0),
	glm::vec3(-80.0, 40.0f, 41.0),
	glm::vec3(25.0, 40.0f, 41.0),
	glm::vec3(-25.0, 40.0f, 41.0),
	glm::vec3(80.0 , 40.0f, 123.0),
	glm::vec3(-80.0, 40.0f, 123.0),
	glm::vec3(25.0, 40.0f, 123.0),
	glm::vec3(-80.0, 40.0f, 123.0),
	glm::vec3(25.0, 50.0f, 150.0),
	glm::vec3(50.0, 50.0f, 50.0),
	glm::vec3(-25.0, 40.0f, 123.0)
};

int main(int argc, char* argv[]) {

	// Load GLFW and Create a Window
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	auto mWindow = glfwCreateWindow(mWidth, mHeight, "Medical Training", nullptr, nullptr);

	// Check for Valid Context
	if (mWindow == nullptr) {
		fprintf(stderr, "Failed to Create OpenGL Context");
		return EXIT_FAILURE;
	}

	// Create Context and Load OpenGL Functions
	glfwMakeContextCurrent(mWindow);
	gladLoadGL();
	fprintf(stderr, "OpenGL %s\n", glGetString(GL_VERSION));

	// Set the required callback functions
	glfwSetKeyCallback(mWindow, key_callback);
	glfwSetCursorPosCallback(mWindow, mouse_callback);
	glfwSetMouseButtonCallback(mWindow, mouse_button_callback);
	glfwSetScrollCallback(mWindow, scroll_callback);

	// Change Viewport
	int width, height;
	glfwGetFramebufferSize(mWindow, &width, &height);
	glViewport(0, 0, width, height);

	//--------------------------------------------------------------------------------------------------------------------------------------------------
	//creat skybox

	GLuint skyboxVAO = createLightVAO();
	GLuint axisVAO = createAxisVAO();

	//--------------------------------------------------------------------------------------------------------------------------------------------------
	//shader

	Shader skyboxShader("Shaders/skyboxShaderV.vert", "Shaders/skyboxShaderF.frag");
	skyboxShader.compile();

	Shader lightingShader("Shaders/advancedLightingMapShaderV.vert", "Shaders/advancedLightingMapShaderF.frag");
	lightingShader.compile();

	Shader bumpShader("Shaders/BumpMappingShaderV.vert", "Shaders/BumpMappingShaderF.frag");
	bumpShader.compile();

	Shader axisShader("Shaders/objectMappingShaderV.vert", "Shaders/objectMappingShaderF.frag");
	axisShader.compile();


	//--------------------------------------------------------------------------------------------------------------------------------------------------
	//Model
	Model Room("Models/Room/Surgical_Room_Copie.dae");
	Model Kidney("Models/Body/Adam_Urimary_System.dae");
	Model Bones("Models/Body/Adam_Bones.dae");
	Model Lung("Models/Body/Adam_Lungs.dae");
	Model Liver("Models/Body/Adam_Liver.dae");
	Model Heart("Models/Body/Adam_Heart.dae");

	//--------------------------------------------------------------------------------------------------------------------------------------------------
	//Texture
	std::vector<std::string> multipleTextures = { "Textures/Yokohama3/posx.jpg", "Textures/Yokohama3/negx.jpg","Textures/Yokohama3/posy.jpg","Textures/Yokohama3/negy.jpg" ,"Textures/Yokohama3/posz.jpg" ,"Textures/Yokohama3/negz.jpg" };
	GLuint skyboxTexture = loadCubemap(multipleTextures);

	//--------------------------------------------------------------------------------------------------------------------------------------------------
	//Matrix
	glm::mat4 M = glm::mat4(1.0);
	//--------------------------------------------------------------------------------------------------------------------------------------------------
	//light
	glm::vec3 lightPosition = glm::vec3(10.0f, 20.0f, 54.0f); //in world-space coordinates
	glm::vec3 lightPosition2 = glm::vec3(1.2f, 1.0f, 2.0f);

	glm::vec3 whiteLight = glm::vec3(1.0f, 1.0f, 1.0f);//lumiere blanche

#define NR_POINT_LIGHTS 8  
	glm::vec3 pointLightAmbien = glm::vec3(0.1f, 0.1f, 0.1f);
	glm::vec3 pointLightDiffuse = glm::vec3(0.5f, 0.5f, 0.5f);
	glm::vec3 pointLightSpecular = glm::vec3(0.5f, 0.5f, 0.5f);
	glm::vec3 pointLightColor = whiteLight; //glm::vec3(1.0f, 0.9f, 0.78f); //lumiere JAUNE
	float pointLightConstant = 1.0f;
	float pointLightLinear = 0.09;
	float pointLightQuadratic = 0.032;

	glm::vec3 dirLightDirection = glm::vec3(1.0f, 1.0f, 1.0f);
	glm::vec3 dirLightAmbient = glm::vec3(0.1f, 0.1f, 0.1f);
	glm::vec3 dirLightDiffuse = glm::vec3(0.4f, 0.4f, 0.4f);
	glm::vec3 dirLightSpecular = glm::vec3(0.5f, 0.5f, 0.5f);
	glm::vec3 dirLightColor = whiteLight;


	//--------------------------------------------------------------------------------------------------------------------------------------------------
	// Depth
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	showFPS();
	// Rendering Loop
	while (glfwWindowShouldClose(mWindow) == false) {


		double time = glfwGetTime();

		glm::mat4 View = camera.GetViewMatrix();
		glm::mat4 Projection = glm::perspective(camera.Zoom, (float)mWidth / (float)mHeight, 0.1f, 1000.0f);

		Do_Movement();


		// Background Fill Color
		glClearColor(0.0f, 0.3f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // avec Depth

		//--------------------------------------------------------------------------------------------------------------------------------------------------
		//RenderScene


		//-------------------------------------------------------------------
		//Room

		glm::mat4 RoomModel = glm::scale(M, glm::vec3(400)); //glm::translate(M, glm::vec3(10.0f, 10.0f, -10.0f));

		//RoomModel = glm::scale(M, glm::vec3(50));
		RoomModel = glm::rotate(RoomModel, glm::radians(90.0f), glm::vec3(-1.0, 0.0, 0.0));
		//lightingShader.setMatrix4("Model", RoomModel);

		setMVPmatrix(lightingShader, RoomModel, Projection, View);
		//lightingShader.setVector3f("lightPos", camera.Position);
		//lightingShader.setInteger("skybox", 0);

		lightingShader.setVector3f("lightPos", lightPosition);
		lightingShader.setVector3f("lightColor", whiteLight);
		lightingShader.setVector3f("viewPos", camera.Position);

		Room.Draw(lightingShader);

		//--------------------------------------------------------------------------------------------------------------------------------------------------
		// Body

		glm::mat4 BodyModel = glm::scale(M, glm::vec3(50));
		//lightingShader.setMatrix4("Model", RoomModel);

		setMVPmatrix(lightingShader, BodyModel, Projection, View);
		bumpShader.setVector3f("lightPos", lightPosition);
		bumpShader.setVector3f("lightColor", whiteLight);
		bumpShader.setVector3f("viewPos", camera.Position);
		Bones.Draw(bumpShader);
		Lung.Draw(bumpShader);
		Kidney.Draw(bumpShader);
		Heart.Draw(bumpShader);
		Liver.Draw(bumpShader);

		glDepthMask(GL_TRUE);

		// Flip Buffers and Draw
		glfwSwapBuffers(mWindow);
		glfwPollEvents();
	}

	glfwTerminate();
	return EXIT_SUCCESS;
}

//--------------------------------------------------------------------------------------------------------------------------------------------------
// Callbacks
void Do_Movement()
{
	if (keys[GLFW_KEY_W])    camera.ProcessKeyboard(FORWARD, deltaTime);
	if (keys[GLFW_KEY_S])    camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (keys[GLFW_KEY_A])    camera.ProcessKeyboard(LEFT, deltaTime);
	if (keys[GLFW_KEY_D])    camera.ProcessKeyboard(RIGHT, deltaTime);
}

void showFPS(void) {
	static double lastTime = glfwGetTime();
	static int nbFrames = 0;

	// Measure speed
	double currentTime = glfwGetTime();
	deltaTime = currentTime - lastTime;
	nbFrames++;
	if (currentTime - lastTime >= 1.0) { // If last prinf() was more than 1 sec ago
		// printf and reset timer
		std::cout << 1000.0 / double(nbFrames) << " ms/frame -> " << nbFrames << " frames/sec" << std::endl;
		nbFrames = 0;
		lastTime += 1.0;
	}
}


static void key_callback(GLFWwindow* window, int key, int /*scancode*/, int action, int /*mode*/)
{
	if (action == GLFW_PRESS)
		keys[key] = true;
	else if (action == GLFW_RELEASE)
		keys[key] = false;

	if (keys[GLFW_KEY_ESCAPE])
		glfwSetWindowShouldClose(window, GL_TRUE);

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

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (keys[GLFW_MOUSE_BUTTON_RIGHT]) {
		std::cout << "Mouse Position : (" << xpos << ", " << ypos << ")" << std::endl;
	}
	if (keys[GLFW_MOUSE_BUTTON_LEFT])
	{
		if (firstMouse)
		{
			lastX = xpos; lastY = ypos;
			firstMouse = false;
		}

		GLfloat xoffset = xpos - lastX; GLfloat yoffset = lastY - ypos; // Reversed since y-coordinates go from bottom to left        
		lastX = xpos; lastY = ypos;
		camera.ProcessMouseMovement(xoffset, yoffset);
	}
}

void scroll_callback(GLFWwindow* window, double /*xoffset*/, double yoffset)
{
	if (keys[GLFW_MOUSE_BUTTON_LEFT]) {
		std::cout << "Mouse Offset : " << yoffset << std::endl;
	}
}


void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}


//----------------------------------------------------------------------------------------------------------------------------------
// set Shader
void setMVPmatrix(Shader pShader, glm::mat4 matModel, glm::mat4 matProjection, glm::mat4 matView)
{
	pShader.use();
	pShader.setMatrix4("Model", matModel);
	pShader.setMatrix4("Projection", matProjection);
	pShader.setMatrix4("View", matView);
}


void setShadeDirectLight(Shader pShader, glm::vec3 dirLightDirection, glm::vec3 dirLightAmbient, glm::vec3 dirLightDiffuse, glm::vec3 dirLightSpecular, glm::vec3 dirLightColor)
{
	pShader.use();

	pShader.setVector3f("dirLight.direction", dirLightDirection);
	pShader.setVector3f("dirLight.ambient", dirLightAmbient);
	pShader.setVector3f("dirLight.diffuse", dirLightDiffuse);
	pShader.setVector3f("dirLight.specular", dirLightSpecular);
	pShader.setVector3f("dirLight.color", dirLightColor);

}



//----------------------------------------------------------------------------------------------------------------------------------
// create VAO
GLuint createLightVAO()
{
	float vertices[] = {
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
		0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f

	};


	unsigned int lightVAO, VBO;

	glGenVertexArrays(1, &lightVAO);
	glBindVertexArray(lightVAO);
	glGenBuffers(1, &VBO);

	// we only need to bind to the VBO, the container's VBO's data already contains the correct data.
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//position
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);

	//normal
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glEnableVertexAttribArray(0);

	return lightVAO;
}

GLuint createAxisVAO()
{
	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------

	// Vertex input
	float vertices[] = {
		// positions x,y,z      // colors RBG
		0.0f, 0.0f, 0.0f,		1.0f, 0.0f, 0.0f,
		0.5f, 0.0f, 0.0f,		1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f,		0.0f, 0.0f, 1.0f,
		0.0f, 0.5f, 0.0f,		0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 0.0f,		0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.5f,		0.0f, 1.0f, 0.0f
	};

	// ..:: Initialization code (done once (unless your object frequently changes)) :: ..
	// 1. bind Vertex Array Object
	// VAO Vertex Array Object
	unsigned int VAO;
	glGenVertexArrays(1, &VAO);

	// VBO Vertex Buffer Object
	unsigned int VBO; // set memory and specify how to send the data to the graphics card
	glGenBuffers(1, &VBO); 	// 2. Copy our vertices array in a buffer for OpenGL to use

	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// 3. then set our vertex attributes pointers
	//Linking Vertex Attributes
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
	// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	glBindVertexArray(0);

	return VAO;
}



//--------------------------------------------------------------------------------------------------------------------------------------------------
// Load Texture
GLuint loadTexture(char const* path)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrComponents;
	unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);

	if (data)
	{
		GLenum format;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
	else
	{
		std::cout << "Texture failed to load at path: " << path << std::endl;
		stbi_image_free(data);
	}

	return textureID;
}

GLuint loadCubemap(std::vector<std::string> multipleTextures)
{
	GLuint skyboxTexture;
	glGenTextures(1, &skyboxTexture);
	glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTexture);

	int width3, height3, n3;
	unsigned char* imageC;

	for (GLuint i = 0; i < multipleTextures.size(); i++)
	{
		imageC = stbi_load(multipleTextures[i].c_str(), &width3, &height3, &n3, 0);
		if (imageC)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width3, height3, 0, GL_RGB, GL_UNSIGNED_BYTE, imageC);

		}
		else
		{
			std::cout << "Cubemap texture failed to load at path: " << multipleTextures[i] << std::endl;
			stbi_image_free(imageC);
		}
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR); 	// Set texture filtering
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // Set texture filtering

	return skyboxTexture;

}