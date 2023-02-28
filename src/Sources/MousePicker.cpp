
#include "../Headers/MousePicker.hpp"

MousePicker::MousePicker(Camera cam, glm::mat4 projection)
{
	this->m_camera = cam;
	this->m_projectionMatrix = projection; //cam.GetProjectionMatrix(int width, int height);
	this->m_viewMatrix = cam.GetViewMatrix();
}

glm::vec3 MousePicker::getCurrentRay() {
	return this->m_currentRay;
}

void MousePicker::update() {
	this->m_viewMatrix = this->m_camera.GetViewMatrix();
	this->m_currentRay = this->computeCurrentRay();
}


glm::vec2 MousePicker::getScreenCoordinate()
{
	this->m_mouseX = gui::mouse_x; 
	this->m_mouseY = gui::mouse_y;
	return glm::vec2(m_mouseX, m_mouseY);
}

glm::vec3  MousePicker::normalisedCoordinates(double xpos, double ypos)
{
	// Find screen coordinates normalized to -1,1
	float x = (2.0f * xpos) / gui::WINDOW_WIDTH - 1.0f;
	float y = 1.0f - (2.0f * ypos) / gui::WINDOW_HEIGHT;
	float z = 1.0f;
	glm::vec3 ray_norm = glm::vec3(x, y, z);

	return ray_norm;
}

glm::vec4 MousePicker::eyeCoordinates(glm::vec4  homogeneousClip)
{
   // glm::mat4 inverseProjectionMatrix = glm::mat4(1);
	glm::mat4 inverseProjectionMatrix = glm::inverse(this->m_projectionMatrix);
	//glm::inverseTranspose(this->m_projectionMatrix,  inverseProjectionMatrix);
    glm::vec4 ray_eye = inverseProjectionMatrix * homogeneousClip;

    return glm::vec4(ray_eye.x, ray_eye.y, -1.0f, 0);
}

glm::vec3 MousePicker::worldCoordinates(glm::vec4  ray_eye)
{
	glm::mat4 inverseViewMatrix =  glm::inverse(this->m_viewMatrix);
    glm::vec4 ray_world = inverseViewMatrix * ray_eye;

    return glm::vec3(ray_world.x, ray_world.y, ray_world.z);
}

glm::vec3 MousePicker::computeCurrentRay() 
{
	glm::vec2 screenCoordinate = this->getScreenCoordinate();
	glm::vec3 normalizedRay = this->normalisedCoordinates(screenCoordinate.x, screenCoordinate.y);
	glm::vec4 homogeneousClip = glm::vec4(normalizedRay.x, normalizedRay.y, -1.0, 1.0);
    glm::vec4 ray_eye = this->eyeCoordinates(homogeneousClip);
    glm::vec3 ray_world = this->worldCoordinates(ray_eye);
	glm::vec3 ray = glm::normalize(ray_world);
	//std::cout << "computeCurrentRay" << "\n";
	//std::cout << glm::to_string(ray) << std::endl;
	return ray;
}

glm::vec3 MousePicker::getPointOnRay(glm::vec3 ray, float distance) {
	glm::vec3 camPos = this->m_camera.getPosition();
	glm::vec3 start =  glm::vec3(camPos.x, camPos.y, camPos.z);
	glm::vec3 scaledRay =  glm::vec3(ray.x * distance, ray.y * distance, ray.z * distance);
	//scaledRay glm::vec3::operator+= start;
	glm::vec3 result = start + scaledRay;
	return result;
}

bool MousePicker::intersectionInRange(float start, float finish, glm::vec3 ray) {
	glm::vec3 startPoint = getPointOnRay(ray, start);
	glm::vec3 endPoint = getPointOnRay(ray, finish);
	

}
