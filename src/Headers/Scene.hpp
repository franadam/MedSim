#pragma once
#ifndef _SCENE_
#define _SCENE_

// System Headers
#include "glitter.hpp"

// Local Headers
#include "Gui.hpp"
#include "Shader.hpp"
#include "Model.hpp"

namespace resource
{

	class Scene
	{
	public:
		//Models
		std::vector<Model> m_models;
		Model* m_room;
		Model* m_scalpel;
		glm::vec3 m_lightPosition;
		glm::vec3 m_lightColor;

		Scene();
		~Scene();

		Model* findModel(std::string name);
		std::vector<Model*> findModels(std::string name);
		void addObject(Model* model, glm::vec3 position);

	private:

		void initModels();
		void initPointLights();
		void initLights();
	};
}

#endif