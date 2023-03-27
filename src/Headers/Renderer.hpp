#pragma once
#ifndef _RENDERER_
#define _RENDERER_

// Local Headers
#include "Scene.hpp"
#include "Camera.hpp"
#include "util.hpp"

class Renderer
{
public:
	Scene* m_scene;

	GLuint worldAxisVAO = 0;
	GLuint posTargetVAO = 0;
	glm::mat4 posTargetPosition;

	Renderer(Camera* camera, Scene* scene);
	Renderer() = default;
	~Renderer();

	void run();
	void update();

private:
	//Camera
	Camera* m_camera;
	glm::mat4 m_ViewMatrix;
	glm::mat4 m_ProjectionMatrix;

	//Shaders
	std::map<std::string, Shader*> m_shaders;
		
	void initMatrices();
	void initShaders();
	void setMVPmatrix(Shader myShader, glm::mat4 matModel, glm::mat4 matProjection, glm::mat4 matView);
	void setUniforms(const GLchar* shaderName, glm::mat4 modelMatrix);
	void setShadeDirectLight(Shader pShader, glm::vec3 dirLightDirection, glm::vec3 dirLightAmbient, glm::vec3 dirLightDiffuse, glm::vec3 dirLightSpecular, glm::vec3 dirLightColor);

	void drawModels(const GLchar* shaderName, glm::mat4 modelMatrix);
	void drawModel(const size_t pos, const GLchar* shaderName, glm::mat4 modelMatrix);
	void renderDebug(glm::mat4 placement);
};

#endif