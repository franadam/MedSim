#include "../Headers/Simulation.hpp"

void Simulation::initMatrices() {
	std::cout << "initMatrices" << "\n";

	m_camera = new Camera(glm::vec3(0.0f, -.10f, .40f));
	m_ViewMatrix = m_camera->GetViewMatrix();
	m_ProjectionMatrix = glm::perspective(m_camera->Zoom, (float)gui::WINDOW_WIDTH / (float)gui::WINDOW_HEIGHT, 0.1f, 100.0f);
}

void Simulation::initShaders() {
	std::cout << "initShaders" << "\n";

	//shaders["skyboxShader"] = new Shader("Shaders/skyboxShaderV.vert", "Shaders/skyboxShaderF.frag");
	m_shaders["lightingShader"] = new Shader("Shaders/advancedLightingMapShaderV.vert", "Shaders/advancedLightingMapShaderF.frag");
	m_shaders["flatShader"] = new Shader("Shaders/flat.vert", "Shaders/flat.frag");
	//shaders["bumpShader"] = new Shader("Shaders/objectMappingShaderV.vert", "Shaders/objectMappingShaderF.frag");
}

void Simulation::initModels() {
	std::cout << "initModels" << "\n";

	std::vector<std::string> paths{
		//"Models/Room/table.dae",
		"Models/Room/Surgical_Room_deci.dae",
		"Models/Body/Adam_Bones.dae",
		"Models/Room/Scalpel.dae",
		"Models/Body/Adam_Urimary_System.dae",
		"Models/Body/Adam_Lungs.dae",
		"Models/Body/Adam_Liver.dae",
		"Models/Body/Adam_Heart.dae"
	};

	/// Chargement multithread
	std::vector< std::future<std::pair<const aiScene*, std::string>> > preloaded;
	for (auto& path : paths)
		preloaded.emplace_back(Model::preload(path.c_str()));

	std::set< std::future<std::pair<const aiScene*, std::string>>* > loaded;
	while (loaded.size() < preloaded.size()) {
		for (auto& pre : preloaded)
			if (!loaded.count(&pre) && is_ready(pre))
				loaded.insert(&pre);

		glClearColor(0.0f, 0.0f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		/// Waiting animation here ...

		glfwSwapBuffers(gui::window);
		glfwPollEvents();
	}
	glm::mat4 UnitMatrix = glm::mat4(1.0);

	for (auto& pre : preloaded)
		m_models.push_back(pre.get());

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

	worldAxisVAO = createWorldAxisVAO();
	posTargetVAO = createPosTargetVAO();
	posTargetPosition = glm::translate(glm::mat4(1), glm::vec3(gui::mouse_x, gui::mouse_y, 0));


	std::cout << "\ndone importing all models\n\n";
}

void Simulation::initUniforms(const GLchar* shaderName, glm::mat4 modelMatrix) {
	//std::cout << "initUniforms" << "\n";
	//setMVPmatrix(*shaders[shaderName], modelMatrix, ProjectionMatrix, ViewMatrix);
	m_shaders[shaderName]->use();

	m_shaders[shaderName]->setMatrix4("Model", modelMatrix);
	m_shaders[shaderName]->setMatrix4("Projection", m_ProjectionMatrix);
	m_shaders[shaderName]->setMatrix4("View", m_ViewMatrix);

	m_shaders[shaderName]->setVector3f("lightPos", m_lightPosition);
	m_shaders[shaderName]->setVector3f("lightColor", m_lightColor);
	m_shaders[shaderName]->setVector3f("viewPos", m_camera->Position);
}

void Simulation::drawModels(const GLchar* shaderName, glm::mat4 modelMatrix) {
	//std::cout << "drawModel" << "\n";
	initUniforms(shaderName, modelMatrix);

	for (int m = 0; m < m_models.size(); m++)
		m_models[m].Draw(*m_shaders[shaderName]);
}

void Simulation::drawModel(const size_t pos, const GLchar* shaderName, glm::mat4 modelMatrix) {
	//std::cout << "drawModel" << "\n";
	initUniforms(shaderName, modelMatrix);

	m_models.at(pos).Draw(*m_shaders[shaderName]);
}

void Simulation::initPointLights() {
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

void Simulation::initLights() {
	initPointLights();
};


//
//void Simulation::initPhysics() {
//	float Friction = 0.9;
//	float Restitution = 0.5;
//
//	btBroadphaseInterface* broadphase;
//	btDefaultCollisionConfiguration* collisionConfig;
//	btCollisionDispatcher* dispatcher;
//	btSequentialImpulseConstraintSolver* solver;
//	btDiscreteDynamicsWorld* world;
//	std::vector<btRigidBody*> bodies;
//
//	broadphase = new btDbvtBroadphase();
//	collisionConfig = new btDefaultCollisionConfiguration();
//	dispatcher = new btCollisionDispatcher(collisionConfig);
//	solver = new btSequentialImpulseConstraintSolver;
//	world = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, collisionConfig);
//	world->setGravity(btVector3(0, -10, 0));
//
//	btTransform t;
//	t.setIdentity();
//	t.setOrigin(btVector3(0, 0, 0));
//	btMotionState* motion = new btDefaultMotionState(t);
//}

//Constructors 
Simulation::Simulation(
	const char* title,
	const int WINDOW_WIDTH,
	const int WINDOW_HEIGHT,
	const int GL_VERSION_MAJOR, const int GL_VERSION_MINOR,
	bool resizable
)
{
	//Init variables

	gui::initGLFW();
	gui::initWindow(title, resizable);
	gui::initGlad();
	gui::initFramebuffer();
	gui::setCallbacks();
	gui::initOpenGLOptions();
	std::cout << gui::window << "\n";

	initMatrices();
	initLights();
	initShaders();
	initModels();

	std::cout << "glGetString(GL_VERSION) : " << glGetString(GL_VERSION) << "\n";

}

// Destructors
Simulation::~Simulation() {

	glfwDestroyWindow(gui::window);
	glfwTerminate();

	//for (size_t i = 0; i < shaders.size(); i++)
		//delete shaders[i];

	//for (auto i = 0; i < models.size(); i++)
		//delete models[i] ;

	//for (size_t i = 0; i < pointLights.size(); i++)
		//delete pointLights[i];
}

void Simulation::testIntersect(Ray ray) {
	for (int m = 0; m < m_models.size(); m++) {
		if (m_models[m].computeIntersectAABB(ray)) {
			m_id_model = m;
			m_models[m_id_model].m_isSelected = true;
		}
	}
}

void Simulation::modelsInteractions() {
	static Ray lastRay;

	Ray ray = m_camera->computeCurrentRay();
	float mouse_x;
	float mouse_y;

	if (gui::keyDown[GLFW_MOUSE_BUTTON_LEFT] && m_id_model == -1)
	{
		testIntersect(ray);
		mouse_x = gui::mouse_x;
		mouse_x = gui::WINDOW_HEIGHT - gui::mouse_y;
	}
	else if (!gui::keyDown[GLFW_MOUSE_BUTTON_LEFT]) {
		m_id_model = -1;
	}
	if (m_id_model != -1) {
		moveModel();
	}
}

void Simulation::moveModel() {
	float mouse_x;
	float mouse_y;

	if (m_id_model != -1) {
		std::cout << "grabbing " << m_id_model << "\n";

		glm::vec3 originalVec = m_models[m_id_model].getPositionVector();

		glm::vec3 world_delta = glm::vec3(gui::mouse_x, gui::WINDOW_HEIGHT - gui::mouse_y, 1) - originalVec;

		m_models[m_id_model].setPosition(world_delta);

		posTargetPosition = glm::translate(glm::mat4(1), world_delta);

		m_models[m_id_model].setTransform(posTargetPosition);
		m_models[m_id_model].transformAABB(); //computeAABB

	}
}

void Simulation::update() {
	//UPDATE --- 
	//std::cout << "update" << "\n";

	/// time management
	gui::updateTime();
	//gui::showFPS();

	/// interactions !
	gui::updateMouseDisplacements();
	gui::updateKeyPress();
	m_camera->cameraControl();

	modelsInteractions();

	//Update the uniforms
	//updateUniforms();


}


/// Test code : show extra info
void Simulation::renderDebug(glm::mat4 placement) {

	/// Drawing world axis
	m_shaders["flatShader"]->use();

	m_shaders["flatShader"]->setMatrix4("Model", glm::mat4(1));
	m_shaders["flatShader"]->setMatrix4("View", m_ViewMatrix);
	m_shaders["flatShader"]->setMatrix4("Projection", m_ProjectionMatrix);
	glBindVertexArray(worldAxisVAO);
	glDrawArrays(GL_LINES, 0, 6);
	glBindVertexArray(0);

	/// Test code : show extra position and bounding boxes

	m_shaders["flatShader"]->setMatrix4("Model", posTargetPosition);
	glBindVertexArray(posTargetVAO);
	glDrawArrays(GL_LINES, 0, 6);
	glBindVertexArray(0);

	for (int m = 0; m < m_models.size(); m++)
		m_models[m].DrawBoundingBox(*m_shaders["flatShader"], glm::mat4(1));

}


void Simulation::render() {
	//DRAW ---
	//std::cout << "render" << "\n";

	//Clear
	glClearColor(0.2f, 0.0f, 0.4f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);


	m_ViewMatrix = m_camera->GetViewMatrix();
	m_ProjectionMatrix = glm::perspective(m_camera->Zoom, (float)gui::WINDOW_WIDTH / (float)gui::WINDOW_HEIGHT, 0.1f, 100.0f);

	//Render models
	glm::mat4 UnitMat = glm::mat4(1);
	//glm::mat4 RoomModel = glm::scale(UnitMat, glm::vec3(0.1));

	glm::vec3 translation = glm::vec3(10.0f, -10.0f, -10.0f);
	glm::mat4 RoomModel = glm::translate(UnitMat, translation);

	drawModels("lightingShader", UnitMat);
	renderDebug(RoomModel);
	//End Draw
	glfwSwapBuffers(gui::window);
	// Flip Buffers and Draw
	glfwPollEvents();
}

// set Shader
void Simulation::setMVPmatrix(Shader myShader, glm::mat4 matModel, glm::mat4 matProjection, glm::mat4 matView)
{
	myShader.use();
	myShader.setMatrix4("Model", matModel);
	myShader.setMatrix4("Projection", matProjection);
	myShader.setMatrix4("View", matView);
}

void Simulation::setShadeDirectLight(Shader pShader, glm::vec3 dirLightDirection, glm::vec3 dirLightAmbient, glm::vec3 dirLightDiffuse, glm::vec3 dirLightSpecular, glm::vec3 dirLightColor)
{
	pShader.use();
	pShader.setVector3f("dirLight.direction", dirLightDirection);
	pShader.setVector3f("dirLight.ambient", dirLightAmbient);
	pShader.setVector3f("dirLight.diffuse", dirLightDiffuse);
	pShader.setVector3f("dirLight.specular", dirLightSpecular);
	pShader.setVector3f("dirLight.color", dirLightColor);

}