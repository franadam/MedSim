#include "../Headers/Simulation.hpp"

void Simulation::initMatrices(Camera camera) {
	std::cout << "initMatrices" << "\n";

	this->camera = camera;
	this->ViewMatrix = camera.GetViewMatrix();
	this->ProjectionMatrix = glm::perspective(camera.Zoom, (float)gui::WINDOW_WIDTH / (float)gui::WINDOW_HEIGHT, 0.1f, 100.0f);
}

void Simulation::initShaders() {
	std::cout << "initShaders" << "\n";

	//this->shaders["skyboxShader"] = new Shader("Shaders/skyboxShaderV.vert", "Shaders/skyboxShaderF.frag");
	this->shaders["lightingShader"] = new Shader("Shaders/advancedLightingMapShaderV.vert", "Shaders/advancedLightingMapShaderF.frag");
	//this->shaders["bumpShader"] = new Shader("Shaders/objectMappingShaderV.vert", "Shaders/objectMappingShaderF.frag");
}

void Simulation::initModels() {
	std::cout << "initModels" << "\n";

	/// Chargement multithread
	/// la structure de ce code n'est clairement pas terrible...
	//auto futRoom = Model::preload("Models/Room/Surgical_Room_Copie.dae");
	auto futBones = Model::preload("Models/Body/Adam_Bones.dae");
	auto futKidney = Model::preload("Models/Body/Adam_Urimary_System.dae");
	auto futLung = Model::preload("Models/Body/Adam_Lungs.dae");
	auto futLiver = Model::preload("Models/Body/Adam_Liver.dae");
	auto futHeart = Model::preload("Models/Body/Adam_Heart.dae");
	
	/// Make the app reactive while loading the models
	std::set<std::string> loaded;
	while (loaded.size() < 5) {
		if (is_ready(futKidney)) loaded.insert("Kidney");
		if (is_ready(futLung)) loaded.insert("Lung");
		if (is_ready(futLiver)) loaded.insert("Liver");
		if (is_ready(futHeart)) loaded.insert("Heart");
		if (is_ready(futBones)) loaded.insert("Bones");
		//if (is_ready(futRoom)) loaded.insert("Room");
	}
	
	Model Kidney(futKidney.get());
	Model Lung(futLung.get());
	Model Liver(futLiver.get());
	Model Heart(futHeart.get());
	Model Bones(futBones.get());
	//Model Room(futRoom.get());
	
	std::cout << "\ndone importing all models\n\n";
	
	this->models.push_back(Kidney);
	this->models.push_back(Lung);
	this->models.push_back(Liver);
	this->models.push_back(Heart);
	this->models.push_back(Bones);
	//this->models.push_back(Room);
}

void Simulation::initUniforms(const GLchar* shaderName, glm::mat4 modelMatrix) {
	//std::cout << "initUniforms" << "\n";
	//this->setMVPmatrix(*this->shaders[shaderName], modelMatrix, this->ProjectionMatrix, this->ViewMatrix);
	this->shaders[shaderName]->use();

	this->shaders[shaderName]->setMatrix4("Model", modelMatrix);
	this->shaders[shaderName]->setMatrix4("Projection", this->ProjectionMatrix);
	this->shaders[shaderName]->setMatrix4("View", this->ViewMatrix);

	this->shaders[shaderName]->setVector3f("lightPos", this->lightPosition);
	this->shaders[shaderName]->setVector3f("lightColor", this->lightColor);
	this->shaders[shaderName]->setVector3f("viewPos", this->camera.Position);
}

void Simulation::drawModels(const GLchar* shaderName, glm::mat4 modelMatrix) {
	//std::cout << "drawModel" << "\n";
	this->initUniforms(shaderName, modelMatrix);

	for (int m = 0; m < this->models.size(); m++)
		this->models[m].Draw(*this->shaders[shaderName]);
}

void Simulation::drawModel(const size_t pos, const GLchar* shaderName, glm::mat4 modelMatrix) {
	//std::cout << "drawModel" << "\n";
	this->initUniforms(shaderName, modelMatrix);

	this->models.at(pos).Draw(*this->shaders[shaderName]);
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

	this->lightPosition = lightPosition;
	this->lightColor = whiteLight;
}

void Simulation::initLights() {
	this->initPointLights();
};

void Simulation::initPicker()
{
	std::cout << "initPicker" << "\n";
	MousePicker picker = MousePicker(this->camera, this->ProjectionMatrix);
	this->picker = picker;
}

void Simulation::updatePicker(glm::mat4 viewMat, glm::mat4 projectionMat)
{
	std::cout << "initMatrices" << "\n";

	this->camera = camera;
	this->ViewMatrix = camera.GetViewMatrix();
	this->ProjectionMatrix = glm::perspective(camera.Zoom, (float)gui::WINDOW_WIDTH / (float)gui::WINDOW_HEIGHT, 0.1f, 100.0f);
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
	Camera camera(glm::vec3(-1.0f, 1.0f, 1.0f));

	gui::initGLFW();
	gui::initWindow(title, resizable);
	gui::initGlad();
	gui::initFramebuffer();
	gui::setCallbacks();
	gui::initOpenGLOptions();
	std::cout << gui::mWindow << "\n";

	this->initMatrices(camera);
	this->initLights();
	this->initShaders();
	this->initModels();
	this->initPicker();

	std::cout << "glGetString(GL_VERSION) : " << glGetString(GL_VERSION) << "\n";

}

// Destructors
Simulation::~Simulation() {

	glfwDestroyWindow(gui::mWindow);
	glfwTerminate();

	//for (size_t i = 0; i < this->shaders.size(); i++)
		//delete this->shaders[i];

	//for (auto i = 0; i < this->models.size(); i++)
		//delete this->models[i] ;

	//for (size_t i = 0; i < this->pointLights.size(); i++)
		//delete this->pointLights[i];
}

void Simulation::update() {
	//UPDATE --- 
	//std::cout << "update" << "\n";

	/// time management
	gui::updateTime();
	gui::showFPS();

	/// interactions !
	gui::updateMouseDisplacements();
	gui::cameraControl(this->camera);
	//this->picker.update();
	
	//Update the uniforms
	//this->updateUniforms();


}

void Simulation::render() {
	//DRAW ---
	//std::cout << "render" << "\n";

	//Clear
	glClearColor(0.0f, 0.3f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	this->ViewMatrix = camera.GetViewMatrix();
	this->ProjectionMatrix = glm::perspective(camera.Zoom, (float)gui::WINDOW_WIDTH / (float)gui::WINDOW_HEIGHT, 0.1f, 100.0f);

	//Render models
	glm::mat4 UnitMat = glm::mat4(1);
	glm::mat4 RoomModel = glm::scale(UnitMat, glm::vec3(0.1));

	RoomModel = glm::rotate(UnitMat, glm::radians(90.0f), glm::vec3(0.0, 1.0, 0.0));
	RoomModel = glm::translate(RoomModel, glm::vec3(10.0f, -10.0f, -10.0f));
	this->drawModels("lightingShader", RoomModel);

	//End Draw
	glfwSwapBuffers(gui::mWindow);
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
