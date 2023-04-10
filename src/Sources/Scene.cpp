#include "../Headers/Scene.hpp"


namespace resource
{

	Scene::Scene(const char* title, bool resizable) {
		gui::initGLFW();
		gui::initWindow(title, resizable);
		gui::initGlad();
		gui::initFramebuffer();
		gui::setCallbacks();
		gui::initOpenGLOptions();

		initModels();
		initLights();
	}

	Scene::~Scene() {
		delete m_room;
		delete m_scalpel;

		glfwDestroyWindow(gui::window);
		glfwTerminate();
	}

	void Scene::initModels() {
		std::cout << "initModels" << "\n";

		std::vector<const char*> paths{
			//"Models/Room/table.dae",
			//"Models/Room/Surgical_Room_deci.dae",
			//"Models/Body/Adam_Bones.dae",
			//"Models/Room/Scalpel.dae",
			"Models/Body/Adam_Urimary_System.dae",
			"Models/Body/Adam_Lungs.dae",
			"Models/Body/Adam_Liver.dae",
			"Models/Body/Adam_Heart.dae"
		};

		/// Chargement multithread
		std::vector< std::future<const aiScene*> > futureAiScenes;
		for (auto& path : paths)
			futureAiScenes.push_back(Model::preload((GLchar *) path));

		for (int i = 0; i < futureAiScenes.size(); ++i)
			m_models.push_back(Model(futureAiScenes[i].get(), paths[i]));

		glm::mat4 UnitMatrix = glm::mat4(1.0);
		glm::vec3 translationBody = glm::vec3(-2.0f, -1.75f, -.750f); //glm::vec3(-60.0f, -40.0f, -10.0f);
		glm::mat4 bodyModel = glm::translate(UnitMatrix, translationBody);
		bodyModel = glm::scale(bodyModel, glm::vec3(0.03f));
		for (int m = 0; m < m_models.size(); m++) {
			m_models[m].setPosition(bodyModel);
			m_models[m].setTransform(bodyModel);
			m_models[m].transformAABB();
		}

		m_room = &m_models[0];
		m_scalpel = &m_models[1];

		glm::vec3 translationScalpel = glm::vec3(50.0f, -20.0f, -50.0f);
		glm::mat4 scalpelModel = glm::translate(UnitMatrix, translationScalpel);

		glm::vec3 translationRoom = glm::vec3(0.0f, 0.0f, -.0f);
		glm::mat4 RoomModel = glm::translate(UnitMatrix, translationScalpel);
		RoomModel = glm::scale(bodyModel, glm::vec3(3.50f));
		//RoomModel = glm::rotate(RoomModel, glm::radians(90.0f), glm::vec3(-1.0, 0.0, 0.0));


		std::cout << "translationRoom" << translationRoom.y << "\n";
		m_room->setPosition(translationRoom);
		//m_room->setPosition(UnitMatrix);

		//m_scalpel->setPosition(translationScalpel);
		//m_scalpel->setTransform(scalpelModel);
		//m_scalpel->transformAABB();

		std::cout << "\ndone importing all models\n\n";
	}


	void Scene::initPointLights() {
		std::cout << "initPointLights" << "\n";
		glm::vec3 lightPosition = glm::vec3(10.0f, 20.0f, 54.0f); //in world-space coordinates
		glm::vec3 lightPosition2 = glm::vec3(1.2f, 1.0f, 2.0f);

		glm::vec3 whiteLight = glm::vec3(1.0f, 1.0f, 1.0f);//lumiere blanche

#define NR_POINT_LIGHTS 8  
		glm::vec3 pointLightAmbien = glm::vec3(0.1f, 0.1f, 0.1f);
		glm::vec3 pointLightDiffuse = glm::vec3(0.5f, 0.5f, 0.5f);
		glm::vec3 pointLightSpecular = glm::vec3(0.5f, 0.5f, 0.5f);
		glm::vec3 pointLightColor = whiteLight; //glm::vec3(1.0f, 0.9f, 0.78f); //lumiere JAUNE
		double pointLightConstant = 1.0f;
		double pointLightLinear = 0.09;
		double pointLightQuadratic = 0.032;

		glm::vec3 dirLightDirection = glm::vec3(1.0f, 1.0f, 1.0f);
		glm::vec3 dirLightAmbient = glm::vec3(0.1f, 0.1f, 0.1f);
		glm::vec3 dirLightDiffuse = glm::vec3(0.4f, 0.4f, 0.4f);
		glm::vec3 dirLightSpecular = glm::vec3(0.5f, 0.5f, 0.5f);
		glm::vec3 dirLightColor = whiteLight;

		m_lightPosition = lightPosition;
		m_lightColor = whiteLight;
	}

	void Scene::initLights() {
		initPointLights();
	};


	// returns pointer to the first occurrence of model with a name that matches name
	// returns nullpointer if none matches
	Model* Scene::findModel(std::string name) {
		for (auto& model : m_models)
			if (model.getName().find(name) != std::string::npos)
				return &model;
		return nullptr;
	}

	// returns vector of pointers to models with name that matches name
	std::vector<Model*> Scene::findModels(std::string name) {
		std::vector<Model*> matches;
		for (auto& model : m_models)
			if (model.getName().find(name) != std::string::npos)
				matches.push_back(&model);
		return matches;
	}

	/* add Mesh to the Scene. */
	void Scene::addObject(Model* model, glm::vec3 position)
	{
		model->setPosition(position);
		m_models.push_back(*model);
	}
}
