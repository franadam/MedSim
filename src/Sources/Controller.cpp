#include "../Headers/Controller.hpp"

Controller::Controller(renderer::Camera* camera, resource::Scene* scene) {
	m_scene = scene;
	m_camera = camera;
	m_ray.direction = glm::vec3(0);
	m_ray.origin = glm::vec3(0);
}

Controller::~Controller()
{
}

void Controller::update() {

	glfwPollEvents();
	gui::updateTime();

	//gui::showFPS();
	gui::updateMouseDisplacements();
	gui::updateKeyPress();

	//if (gui::keyDown[GLFW_KEY_ENTER])
	//	m_scene->m_models[3].setPosition(glm::vec3(0));

	m_camera->cameraControl();
	modelsInteractions();

	// todo : m_physics est géré par la controller, pas par le renderer
	// m_physics->update(1.0 / 60.0);
}

void Controller::testIntersect(Ray ray) {
	for (int m = 0; m < m_scene->m_models.size(); m++) {
		if (m_scene->m_models[m].computeIntersectAABB(ray)) {
			m_id_model = m;
		}
	}
}

void Controller::modelsInteractions() {
	static Ray lastRay;
	static double distanceIntersect;

	Ray ray = m_camera->computeCurrentRay();

	if (gui::keyDown[GLFW_MOUSE_BUTTON_LEFT] && m_id_model == -1)
	{   
		testIntersect(ray);
		if (m_id_model != -1) {
			std::cout << "grabbing " << m_scene->m_models[m_id_model].getName() << "\n";
			lastRay = ray;
			distanceIntersect = glm::distance(m_camera->getPosition(), m_scene->m_models[m_id_model].m_intersectPosition);
		}
	}
	else if (!gui::keyDown[GLFW_MOUSE_BUTTON_LEFT]) {
		if (m_id_model != -1)
			std::cout << "releasing " << m_scene->m_models[m_id_model].getName() << "\n";
		m_id_model = -1;
	}
	if (m_id_model != -1) {
		moveModel( distanceIntersect*(ray.direction-lastRay.direction) );
		lastRay = ray;
	}
}

void Controller::moveModel(glm::vec3 shift)
{
	m_scene->m_models[m_id_model].shiftPosition(shift);
}
