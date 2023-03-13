#include "../Headers/Simulation.hpp"

void mouseCallback();
Simulation* simu;

void Simulation::initMatrices() {
	std::cout << "initMatrices" << "\n";

	m_camera = new Camera(glm::vec3(-1.0f, 1.0f, 1.0f));
	m_ViewMatrix = m_camera->GetViewMatrix();
	m_ProjectionMatrix = glm::perspective(m_camera->Zoom, (float)gui::m_WINDOW_WIDTH / (float)gui::m_WINDOW_HEIGHT, 0.1f, 100.0f);
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
		//"Models/Body/Adam_Bones.dae",
		"Models/Body/Adam_Urimary_System.dae",
		"Models/Body/Adam_Lungs.dae",
		"Models/Body/Adam_Liver.dae",
		"Models/Body/Adam_Heart.dae"
	};

	/// Chargement multithread
	std::vector< std::future<std::pair<const aiScene*, std::string>> > preloaded;
	for (auto& path : paths)
		preloaded.emplace_back(Model::preload(path.c_str()));

	/// Make the app reactive while loading the models
	std::set< std::future<std::pair<const aiScene*, std::string>>* > loaded;
	while (loaded.size() < preloaded.size()) {
		for (auto& pre : preloaded)
			if (!loaded.count(&pre) && is_ready(pre))
				loaded.insert(&pre);

		glClearColor(0.0f, 0.0f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		/// Waiting animation here ...

		glfwSwapBuffers(gui::m_window);
		glfwPollEvents();
	}

	for (auto& pre : preloaded)
		m_models.push_back(pre.get());

	worldAxisVAO = createWorldAxisVAO();
	posTargetVAO = createPosTargetVAO();
	posTargetPosition = glm::translate(glm::mat4(1), glm::vec3(gui::m_mouse_x, gui::m_mouse_y, 0));


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

void Simulation::initPicker()
{
	std::cout << "initPicker" << "\n";
	MousePicker picker = MousePicker(m_camera, m_ProjectionMatrix);
	m_picker = picker;
}

void Simulation::updatePicker(glm::mat4 viewMat, glm::mat4 projectionMat)
{
	std::cout << "initMatrices" << "\n";

	m_ViewMatrix = m_camera->GetViewMatrix();
	m_ProjectionMatrix = glm::perspective(m_camera->Zoom, (float)gui::m_WINDOW_WIDTH / (float)gui::m_WINDOW_HEIGHT, 0.1f, 100.0f);
}
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
	std::cout << gui::m_window << "\n";

	initMatrices();
	initLights();
	initShaders();
	initModels();
	initPicker();

	std::cout << "glGetString(GL_VERSION) : " << glGetString(GL_VERSION) << "\n";

	gui::setMouseUserCallback(mouseCallback);
	simu = this;

	//GLuint myCubeVAO = createCubeVAO();
	//GLuint mySquareVAO = createSquareVAO();
	//
	//GLuint worldAxisVAO = createWorldAxisVAO();
}

// Destructors
Simulation::~Simulation() {

	glfwDestroyWindow(gui::m_window);
	glfwTerminate();

	//for (size_t i = 0; i < shaders.size(); i++)
		//delete shaders[i];

	//for (auto i = 0; i < models.size(); i++)
		//delete models[i] ;

	//for (size_t i = 0; i < pointLights.size(); i++)
		//delete pointLights[i];
}


void Simulation::update() {
	//UPDATE --- 
	//std::cout << "update" << "\n";

	/// time management
	gui::updateTime();
	//gui::showFPS();

	/// interactions !
	gui::updateMouseDisplacements();
	gui::cameraControl(m_camera);
	
	//posTargetPosition = glm::translate(glm::mat4(1), glm::vec3(gui::m_mouse_x, gui::m_mouse_y, 0));
	if(m_id_model!=-1) posTargetPosition = glm::translate(glm::mat4(1), m_models[m_id_model].m_intersectPosition);

	//if (gui::m_keys[GLFW_MOUSE_BUTTON_LEFT] == true && m_id_model == -1) {
	//	std::cout << "update : GLFW_MOUSE_BUTTON_LEFT" << "\n";

	//	m_picker.update();
	//	Ray ray = m_picker.getCurrentRay();

	//	//selection model
	//	for (int m = 0; m < m_models.size(); m++) {
	//		if (m_models[m].computeIntersectAABB(ray)) {
	//			m_id_model = m;

	//		}
	//	}

	//	std::cout << "update : m_id_model" << m_id_model << "\n";
	//}



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
		m_models[m].DrawBoundingBox(*m_shaders["flatShader"], placement);

	//glClear(GL_COLOR_BUFFER_BIT);
	//glBegin(GL_POINTS);
	//	glVertex3f(0.0f, 0.0f, 0.0f);
	//	glVertex3f(50.0f, 40.0f, 0.0f);
	//	glVertex3f(50.0f, 50.0f, 50.0f);
	//glEnd();
}


void Simulation::render() {
	//DRAW ---
	//std::cout << "render" << "\n";

	//Clear
	glClearColor(0.2f, 0.0f, 0.4f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);


	m_ViewMatrix = m_camera->GetViewMatrix();
	m_ProjectionMatrix = glm::perspective(m_camera->Zoom, (float)gui::m_WINDOW_WIDTH / (float)gui::m_WINDOW_HEIGHT, 0.1f, 100.0f);

	//Render models
	glm::mat4 UnitMat = glm::mat4(1);
	glm::mat4 RoomModel = glm::scale(UnitMat, glm::vec3(0.1));

	RoomModel = glm::rotate(UnitMat, glm::radians(90.0f), glm::vec3(0.0, 1.0, 0.0));
	RoomModel = glm::translate(RoomModel, glm::vec3(10.0f, -10.0f, -10.0f));

	//for (int m = 0; m < m_models.size(); m++){
	//	m_models[m].setTransform(RoomModel);
	//	m_models[m].computeAABB();
	//}

	drawModels("lightingShader", RoomModel);
	renderDebug(RoomModel);
	//End Draw
	glfwSwapBuffers(gui::m_window);
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

void mouseCallback()
{
	if (gui::m_keys[GLFW_MOUSE_BUTTON_LEFT] == true) {

		std::cout << "***********************************************************************************" << std::endl;

		simu->m_picker.update();
		Ray ray = simu->m_picker.getCurrentRay();
		std::cout << "Ray = x:" << ray.origin.x << "  y: " << ray.origin.y << "  z: " << ray.origin.z << std::endl;
		std::cout << "Ray > x:" << ray.direction.x << "  y: " << ray.direction.y << "  z: " << ray.direction.z << std::endl;

		//selection model
		for (int m = 0; m < simu->m_models.size(); m++) {
			std::cout << "---------------------------------------------------------" << std::endl;
			if (simu->m_models[m].computeIntersectAABB(ray)) {
				simu->m_id_model = m;
				std::cout << "model = " << m << std::endl;
			}
		}

		std::cout << "***********************************************************************************" << std::endl;

	}
}