#pragma once
#ifndef _RENDERER_
#define _RENDERER_

// Local Headers
#include "Scene.hpp"
#include "Camera.hpp"
#include "Physics.hpp"
#include "util.hpp"

namespace renderer
{

	class Renderer
	{
	public:
		resource::Scene* m_scene;

		GLuint worldAxisVAO = 0;
		GLuint posTargetVAO = 0;
		glm::mat4 posTargetPosition;

		Renderer(Camera* camera, resource::Scene* scene, physics::Physics* m_physics);
		//Renderer(Camera* camera, resource::Scene* scene);
		Renderer() = default;
		~Renderer();

		void run();
		//void update();

	private:
		//Camera
		Camera* m_camera;
		glm::mat4 m_ViewMatrix;
		glm::mat4 m_ProjectionMatrix;

		//Shaders
		std::map<std::string, resource::Shader*> m_shaders;

		//Physics
		//physics::Physics* m_physics;

		GLuint m_BoxVAO;

		void initMatrices();
		void initShaders();
		void setMVPmatrix(resource::Shader myShader, glm::mat4 matModel, glm::mat4 matProjection, glm::mat4 matView);
		void setUniforms(const GLchar* shaderName, glm::mat4 modelMatrix);
		void setShadeDirectLight(resource::Shader pShader, glm::vec3 dirLightDirection, glm::vec3 dirLightAmbient, glm::vec3 dirLightDiffuse, glm::vec3 dirLightSpecular, glm::vec3 dirLightColor);

		void drawModels(const GLchar* shaderName, glm::mat4 modelMatrix);
		int drawModel(std::string name, const GLchar* shaderName, glm::mat4 modelMatrix);
		void renderDebug(glm::mat4 placement);
		void DrawBoundingBox(resource::Shader& shader, glm::mat4 placement, GLuint BoxVAO);
	};
}

#endif