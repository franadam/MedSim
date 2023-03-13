#pragma once
#ifndef _SIMULATION_
#define _SIMULATION_
// Local Headers

// System Headers
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// Standard Headers
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <set>

//Matrix
#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4, glm::ivec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/type_ptr.hpp> // glm::value_ptr

// Local Headers
#include "Gui.hpp"
#include "Shader.hpp"
#include "Model.hpp"
#include "Camera.hpp"
#include "MousePicker.hpp"
#include "util.hpp"

class Simulation
{
public:
	///Variables

	//Camera
	Camera * m_camera;

	//Model
	int m_id_model = -1;

	//Matrices
	glm::mat4 m_ViewMatrix;
	glm::mat4 m_ProjectionMatrix;
			  
	glm::vec3 m_lightPosition;
	glm::vec3 m_lightColor;

	//Shaders
	std::map<std::string, Shader *> m_shaders;
	
	//Models
	std::vector<Model> m_models;

	//Mouse picker
	MousePicker m_picker;

	GLuint worldAxisVAO = 0;
	GLuint posTargetVAO = 0;
	glm::mat4 posTargetPosition;

	//Private functions
	void initMatrices();
	void initShaders();
	void initModels();
	void initUniforms(const GLchar* shaderName, glm::mat4 modelMatrix);
	void drawModels(const GLchar* shaderName, glm::mat4 modelMatrix);
	void drawModel(const size_t pos, const GLchar *shaderName, glm::mat4 modelMatrix);
	void initPointLights();
	void setMVPmatrix(Shader myShader, glm::mat4 matModel, glm::mat4 matProjection, glm::mat4 matView);
	void initLights();
	void initPicker();
	void updatePicker(glm::mat4 viewMat, glm::mat4 projectionMat);
	void setShadeDirectLight(Shader pShader, glm::vec3 dirLightDirection, glm::vec3 dirLightAmbient, glm::vec3 dirLightDiffuse, glm::vec3 dirLightSpecular, glm::vec3 dirLightColor);
	void initTextures();
	void updateUniforms();
	void renderDebug(glm::mat4 placement);

	//Constructors / Destructors
	Simulation(
		const char* title,
		const int WINDOW_WIDTH, const int WINDOW_HEIGHT,
		const int GL_VERSION_MAJOR, const int GL_VERSION_MINOR,
		bool resizable
	);
	virtual ~Simulation();


	//Functions
	void update();
	void render();

};


#endif