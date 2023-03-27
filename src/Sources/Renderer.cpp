#include "../Headers/Renderer.hpp"


Renderer::Renderer(Camera* camera, Scene* scene)
{
	m_scene = scene;
	m_camera = camera;

	initShaders();

	worldAxisVAO = createWorldAxisVAO();
	posTargetVAO = createPosTargetVAO();
	posTargetPosition = glm::translate(glm::mat4(1), glm::vec3(gui::mouse_x, gui::mouse_y, 0));


}

Renderer::~Renderer() {
	delete m_scene;
	delete m_camera;
}

void Renderer::initMatrices() {
	std::cout << "initMatrices" << "\n";

	m_ViewMatrix = m_camera->GetViewMatrix();
	m_ProjectionMatrix = glm::perspective(m_camera->Zoom, (float)gui::WINDOW_WIDTH / (float)gui::WINDOW_HEIGHT, 0.1f, 100.0f);
}

void Renderer::initShaders() {
	std::cout << "initShaders" << "\n";

	//shaders["skyboxShader"] = new Shader("Shaders/skyboxShaderV.vert", "Shaders/skyboxShaderF.frag");
	m_shaders["lightingShader"] = new Shader("Shaders/advancedLightingMapShaderV.vert", "Shaders/advancedLightingMapShaderF.frag");
	m_shaders["flatShader"] = new Shader("Shaders/flat.vert", "Shaders/flat.frag");
	//shaders["bumpShader"] = new Shader("Shaders/objectMappingShaderV.vert", "Shaders/objectMappingShaderF.frag");
}

void Renderer::setUniforms(const GLchar* shaderName, glm::mat4 modelMatrix) {
	//std::cout << "setUniforms" << "\n";
	//setMVPmatrix(*shaders[shaderName], modelMatrix, ProjectionMatrix, ViewMatrix);
	m_shaders[shaderName]->use();

	m_shaders[shaderName]->setMatrix4("Model", modelMatrix);
	m_shaders[shaderName]->setMatrix4("Projection", m_ProjectionMatrix);
	m_shaders[shaderName]->setMatrix4("View", m_ViewMatrix);

	m_shaders[shaderName]->setVector3f("lightPos", m_scene->m_lightPosition);
	m_shaders[shaderName]->setVector3f("lightColor", m_scene->m_lightColor);
	m_shaders[shaderName]->setVector3f("viewPos", m_camera->Position);
}

void Renderer::drawModels(const GLchar* shaderName, glm::mat4 modelMatrix) {
	//std::cout << "drawModel" << "\n";
	setUniforms(shaderName, modelMatrix);

	for (int m = 0; m < m_scene->m_models.size(); m++)
		m_scene->m_models[m].Draw(*m_shaders[shaderName]);
}

void Renderer::drawModel(const size_t pos, const GLchar* shaderName, glm::mat4 modelMatrix) {
	//std::cout << "drawModel" << "\n";
	setUniforms(shaderName, modelMatrix);

	m_scene->m_models.at(pos).Draw(*m_shaders[shaderName]);
}

// set Shader
void Renderer::setMVPmatrix(Shader myShader, glm::mat4 matModel, glm::mat4 matProjection, glm::mat4 matView)
{
	myShader.use();
	myShader.setMatrix4("Model", matModel);
	myShader.setMatrix4("Projection", matProjection);
	myShader.setMatrix4("View", matView);
}


void Renderer::update()
{
	gui::showFPS();

	m_camera->cameraControl();

	gui::updateMouseDisplacements();
	gui::updateKeyPress();
}

// ici m_scene est NULL
void Renderer::run() {
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

 //Test code : show extra info
void Renderer::renderDebug(glm::mat4 placement) {

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

	for (int m = 0; m < m_scene->m_models.size(); m++)
		m_scene->m_models[m].DrawBoundingBox(*m_shaders["flatShader"], glm::mat4(1));

}
