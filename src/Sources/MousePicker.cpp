
#include "../Headers/MousePicker.hpp"

MousePicker::MousePicker(Camera cam, glm::mat4 projection)
{
	m_camera = cam;
	m_projectionMatrix = projection; //cam.GetProjectionMatrix(int width, int height);
	m_viewMatrix = cam.GetViewMatrix();
	//m_currentRay.direction = glm::vec3(0.0);
	//m_currentRay.origine = glm::vec3(0.0);
	//m_mouseX = 0.0;
	//m_mouseY = 0.0;
}

Ray MousePicker::getCurrentRay() {
	return m_currentRay;
}

void MousePicker::update() {
	m_viewMatrix = m_camera.GetViewMatrix();
	m_currentRay = computeCurrentRay();
}


glm::vec2 MousePicker::getScreenCoordinate()
{
	m_mouseX = gui::m_mouse_x; 
	m_mouseY = gui::m_mouse_y;
	return glm::vec2(m_mouseX, m_mouseY);
}

glm::vec3  MousePicker::normalisedCoordinates(double xpos, double ypos)
{
	// Find screen coordinates normalized to -1,1
	float x = (2.0f * xpos) / gui::m_WINDOW_WIDTH - 1.0f;
	float y = 1.0f - (2.0f * ypos) / gui::m_WINDOW_HEIGHT;
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
	glm::mat4 inverseViewMatrix =  glm::inverse(m_viewMatrix);
    glm::vec4 ray_world = inverseViewMatrix * ray_eye;

    return glm::vec3(ray_world.x, ray_world.y, ray_world.z);
}

//debug
Ray MousePicker::computeCurrentRay()
{
	Ray ray;
	glm::vec2 screenCoordinate = getScreenCoordinate();
	glm::vec3 normalizedRay = normalisedCoordinates(screenCoordinate.x, screenCoordinate.y);
	glm::vec4 homogeneousClip = glm::vec4(normalizedRay.x, normalizedRay.y, -1.0, 1.0);
    glm::vec4 ray_eye = eyeCoordinates(homogeneousClip);
    glm::vec3 ray_world = worldCoordinates(ray_eye);
	glm::vec3 direction = glm::normalize(ray_world);
	ray.direction = direction;
	ray.origine = m_camera.getPosition();
	//std::cout << "computeCurrentRay" << "\n";
	//std::cout << glm::to_string(ray) << std::endl;
	return ray;
}
