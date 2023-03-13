
#include "../Headers/MousePicker.hpp"

MousePicker::MousePicker(Camera* cam, glm::mat4 projection)
{
	m_camera = cam;
	m_projectionMatrix = projection; //cam.GetProjectionMatrix(int width, int height);
	m_viewMatrix = cam->GetViewMatrix();
	//m_currentRay.direction = glm::vec3(0.0);
	//m_currentRay.origin = glm::vec3(0.0);
	//m_mouseX = 0.0;
	//m_mouseY = 0.0;
}

Ray MousePicker::getCurrentRay() {

	return m_currentRay;
}

void MousePicker::update() {
	std::cout << "MousePicker::update()  >>> Zoom = " << m_camera->Zoom << std::endl;
	m_viewMatrix = m_camera->GetViewMatrix();
	m_projectionMatrix = glm::perspective(m_camera->Zoom, (float)gui::m_WINDOW_WIDTH / (float)gui::m_WINDOW_HEIGHT, 0.1f, 100.0f);
	Ray ray = computeCurrentRay();
	std::cout << "m_currentRay >>> direction = x : " << m_currentRay.direction.x << " y : " << m_currentRay.direction.y << " z : " << m_currentRay.direction.z << std::endl;
	m_currentRay = computeCurrentRay2();
	std::cout << "Ray >>> direction = x : " << ray.direction.x << " y : " << ray.direction.y << " z : " << ray.direction.z << std::endl;
}


glm::vec2 MousePicker::getScreenCoordinates()
{
	m_mouseX = gui::m_mouse_x;
	m_mouseY = gui::m_mouse_y;
	return glm::vec2(m_mouseX, m_mouseY);
}

glm::vec3  MousePicker::normalisedCoordinates(double xpos, double ypos)
{
	// Find screen coordinates normalized to -1,1
	float x = (2.0f * xpos) / gui::m_WINDOW_WIDTH - 1.0f;
	float y = (2.0f * ypos) / gui::m_WINDOW_HEIGHT - 1.0f;
	float z = 1.0f;
	glm::vec3 ray_norm = glm::vec3(x, y, z);

	return ray_norm;
}

glm::vec4 MousePicker::eyeCoordinates(glm::vec4  homogeneousClip)
{
	// glm::mat4 inverseProjectionMatrix = glm::mat4(1);
	glm::mat4 inverseProjectionMatrix = glm::inverse(m_projectionMatrix);
	//glm::inverseTranspose(m_projectionMatrix,  inverseProjectionMatrix);
	glm::vec4 ray_eye = inverseProjectionMatrix * homogeneousClip;

	return glm::vec4(ray_eye.x, ray_eye.y, -1.0f, 0);
}

glm::vec3 MousePicker::worldCoordinates(glm::vec4  ray_eye)
{
	glm::mat4 inverseViewMatrix = glm::inverse(m_viewMatrix);
	glm::vec4 ray_world = inverseViewMatrix * ray_eye;

	return glm::vec3(ray_world.x, ray_world.y, ray_world.z);
}

//debug
Ray MousePicker::computeCurrentRay()
{
	Ray ray;
	glm::vec2 screenCoordinate = getScreenCoordinates();
	std::cout << "screenCoordinate = x : " << screenCoordinate.x << " y : " << screenCoordinate.y << std::endl;
	glm::vec3 normalizedRay = normalisedCoordinates(screenCoordinate.x, screenCoordinate.y);
	std::cout << "normalizedRay = x : " << normalizedRay.x << " y : " << normalizedRay.y << " z : " << normalizedRay.z << std::endl;
	glm::vec4 homogeneousClip = glm::vec4(normalizedRay.x, normalizedRay.y, -1.0, 1.0);
	glm::vec4 ray_eye = eyeCoordinates(homogeneousClip);
	std::cout << "ray_eye = x : " << ray_eye.x << " y : " << ray_eye.y << " z : " << ray_eye.z << std::endl;
	glm::vec3 ray_world = worldCoordinates(ray_eye);
	std::cout << "ray_world = x : " << ray_world.x << " y : " << ray_world.y << " z : " << ray_world.z << std::endl;
	glm::vec3 direction = glm::normalize(ray_world);
	std::cout << "direction = x : " << direction.x << " y : " << direction.y << " z : " << direction.z << std::endl;
	ray.direction = direction;
	ray.origin = m_camera->getPosition();
	//std::cout << "computeCurrentRay" << "\n";
	//std::cout << glm::to_string(ray) << std::endl;
	return ray;
}

Ray MousePicker::computeCurrentRay2()
{
	Ray ray;
	ray.origin = m_camera->getPosition();
	glm::dvec3 pos = glm::unProject(glm::vec3(gui::m_mouse_x, gui::m_mouse_y,1), m_viewMatrix, m_projectionMatrix, glm::vec4(0, 0, gui::m_WINDOW_WIDTH, gui::m_WINDOW_HEIGHT));
	std::cout << "pos = x : " << pos.x << " y : " << pos.y << " z : " << pos.z << std::endl;
	glm::vec3 direction = glm::normalize(pos - ray.origin);
	std::cout << "direction = x : " << direction.x << " y : " << direction.y << " z : " << direction.z << std::endl;
	ray.direction = direction;
	//std::cout << "computeCurrentRay" << "\n";
	//std::cout << glm::to_string(ray) << std::endl;
	return ray;
}
