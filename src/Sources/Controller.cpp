#include "../Headers/Controller.hpp"

Controller::Controller( Camera* camera, Scene* scene) {
	m_scene = scene;
	m_camera = camera;
	m_ray.direction = glm::vec3(0);
	m_ray.origin = glm::vec3(0);
};


void Controller::update() {
	/// interactions !
	gui::updateMouseDisplacements();
	gui::updateKeyPress();

	modelsInteractions();
}

void Controller::testIntersect(Ray ray) {
	for (int m = 0; m < m_scene->m_models.size(); m++) {
		if (m_scene->m_models[m].computeIntersectAABB(ray)) {
			m_id_model = m;
			m_scene->m_models[m_id_model].m_isSelected = true;
		}
	}
}

void Controller::modelsInteractions() {
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

void Controller::moveModel() {
	float mouse_x;
	float mouse_y;

	if (m_id_model != -1) {
		std::cout << "grabbing " << m_id_model << "\n";

		glm::vec3 originalVec = m_scene->m_models[m_id_model].getPositionVector();

		glm::vec3 world_delta = glm::vec3(gui::mouse_x, gui::WINDOW_HEIGHT - gui::mouse_y, 1) - originalVec;

		 m_scene->m_models[m_id_model].setPosition(world_delta);

		posTargetPosition = glm::translate(glm::mat4(1), world_delta);

		m_scene->m_models[m_id_model].setTransform(posTargetPosition);
		m_scene->m_models[m_id_model].transformAABB(); //computeAABB

	}
}
