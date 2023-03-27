#pragma once
#include "../Headers/Camera.hpp"

// Constructor with vectors
Camera::Camera(glm::vec3 position , glm::vec3 up , GLfloat yaw, GLfloat pitch) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVTY), Zoom(ZOOM)
{
    this->Position = position;
    this->WorldUp = up;
    this->Yaw = yaw;
    this->Pitch = pitch;
    this->updateCameraVectors();
}
// Constructor with scalar values
Camera::Camera(GLfloat posX, GLfloat posY, GLfloat posZ, GLfloat upX, GLfloat upY, GLfloat upZ, GLfloat yaw, GLfloat pitch) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVTY), Zoom(ZOOM)
{
    this->Position = glm::vec3(posX, posY, posZ);
    this->WorldUp = glm::vec3(upX, upY, upZ);
    this->Yaw = yaw;
    this->Pitch = pitch;
    this->updateCameraVectors();
}

// Returns the view matrix calculated using Eular Angles and the LookAt Matrix
glm::mat4 Camera::GetViewMatrix()
{
    return glm::lookAt(this->Position, this->Position + this->Front, this->Up);
}


// Returns the projection matrix
glm::mat4 Camera::GetProjectionMatrix(int width, int height)
{
    return glm::perspective(
        glm::radians(this->Zoom),
        (float)width / height,
        0.1f,
        100.0f
    );
}

glm::vec3 Camera::getPosition() {
    return this->Position;
}

void Camera::setPosition(glm::vec3 position) {
    Position = position;
}


// Processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
void Camera::Camera::ProcessKeyboard(unsigned int  direction, GLfloat deltaTime)
{
    GLfloat velocity = MovementSpeed * deltaTime;
    if (direction & FORWARD)
        Position += Front * velocity;
    if (direction & BACKWARD)
        Position -= Front * velocity;
    if (direction & UP)
        Position += Up * velocity;
    if (direction & DOWN)
        Position -= Up * velocity;
    if (direction & LEFT)
        Position -= Right * velocity;
    if (direction & RIGHT)
        Position += Right * velocity;
}

// Processes input received from a mouse input system. Expects the offset value in both the x and y direction.
void Camera::ProcessMouseMovement(GLfloat xoffset, GLfloat yoffset, GLboolean constrainPitch )
{
    xoffset *= this->MouseSensitivity;
    yoffset *= this->MouseSensitivity;

    this->Yaw   += xoffset;
    this->Pitch += yoffset;

    // Make sure that when pitch is out of bounds, screen doesn't get flipped
    if (constrainPitch)
    {
        if (this->Pitch > 89.0f)
            this->Pitch = 89.0f;
        if (this->Pitch < -89.0f)
            this->Pitch = -89.0f;
    }

    // Update Front, Right and Up Vectors using the updated Eular angles
    this->updateCameraVectors();
}

// Processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
void Camera::ProcessMouseScroll(GLfloat yoffset)
{
    this->Zoom -= yoffset;
    if (this->Zoom <= 5.0f)
        this->Zoom = 5.0f;
    if (this->Zoom >= 60.0f)
        this->Zoom = 60.0f;
}

// Calculates the front vector from the Camera's (updated) Eular Angles
void Camera::updateCameraVectors()
{
    // Calculate the new Front vector
    glm::vec3 front;
    front.x = cos(glm::radians(this->Yaw)) * cos(glm::radians(this->Pitch));
    front.y = sin(glm::radians(this->Pitch));
    front.z = sin(glm::radians(this->Yaw)) * cos(glm::radians(this->Pitch));
    this->Front = glm::normalize(front);
    // Also re-calculate the Right and Up vector
    this->Right = glm::normalize(glm::cross(this->Front, this->WorldUp));  // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
    this->Up    = glm::normalize(glm::cross(this->Right, this->Front));
}

void Camera::cameraControl()
{
    //		std::cout << camera << "\n";
            //std::cout << "cameraControl" << keyDown[GLFW_KEY_UP] << "\n";

    if (gui::keyDown[GLFW_MOUSE_BUTTON_RIGHT])
        ProcessMouseMovement(gui::mouse_dx, -gui::mouse_dy);

    ProcessKeyboard(gui::keyDown[GLFW_KEY_UP] * FORWARD | gui::keyDown[GLFW_KEY_DOWN] * BACKWARD
        | gui::keyDown[GLFW_KEY_LEFT] * LEFT | gui::keyDown[GLFW_KEY_RIGHT] * RIGHT
        | gui::keyDown[GLFW_KEY_PAGE_UP] * UP | gui::keyDown[GLFW_KEY_PAGE_DOWN] * DOWN, gui::timeDelta);//avgDeltaTime);

    ProcessMouseScroll(gui::mouse_scroll_dy);

    // todo this is not to be done here ! => in gui module
    gui::mouse_scroll_dy = 0;
}

glm::vec3  Camera::normalisedCoordinates(double xpos, double ypos)
{
    // Find screen coordinates normalized to -1,1
    float x = (2.0f * xpos) / gui::WINDOW_WIDTH - 1.0f;
    float y = (2.0f * ypos) / gui::WINDOW_HEIGHT - 1.0f;
    float z = 1.0f;
    glm::vec3 ray_norm = glm::vec3(x, y, z);

    return ray_norm;
}

glm::vec4 Camera::eyeCoordinates(glm::vec4  homogeneousClip)
{
    glm::mat4 projectionMatrix = GetProjectionMatrix(gui::WINDOW_WIDTH, gui::WINDOW_HEIGHT);
    glm::mat4 inverseProjectionMatrix = glm::inverse(projectionMatrix);
    glm::vec4 ray_eye = inverseProjectionMatrix * homogeneousClip;

    return glm::vec4(ray_eye.x, ray_eye.y, -1.0f, 0);
}

glm::vec3 Camera::worldCoordinates(glm::vec4  ray_eye)
{
    glm::mat4 viewMatrix = GetViewMatrix();
    glm::mat4 inverseViewMatrix = glm::inverse(viewMatrix);
    glm::vec4 ray_world = inverseViewMatrix * ray_eye;

    return glm::vec3(ray_world.x, ray_world.y, ray_world.z);
}

Ray Camera::computeCurrentRay()
{
    Ray ray;
    glm::vec3 normalizedRay = normalisedCoordinates(gui::mouse_x, gui::WINDOW_HEIGHT - gui::mouse_y);
    glm::vec4 homogeneousClip = glm::vec4(normalizedRay.x, normalizedRay.y, -1.0, 1.0);
    glm::vec4 ray_eye = eyeCoordinates(homogeneousClip);
    glm::vec3 ray_world = worldCoordinates(ray_eye);
    glm::vec3 direction = glm::normalize(ray_world);
    ray.direction = direction;
    ray.origin = getPosition();
    return ray;
}

Ray Camera::computeCurrentRay2()
{
    Ray ray;
    glm::mat4 viewMatrix = GetViewMatrix();
    glm::mat4 projectionMatrix = GetProjectionMatrix(gui::WINDOW_WIDTH, gui::WINDOW_HEIGHT);
    glm::vec4 viewport = glm::vec4(0, 0, gui::WINDOW_WIDTH, gui::WINDOW_HEIGHT);

    glm::dvec3 pos = glm::unProject(glm::vec3(gui::mouse_x, gui::WINDOW_HEIGHT - gui::mouse_y, 1), viewMatrix, projectionMatrix, viewport);
    glm::vec3 direction = glm::normalize(pos - ray.origin);

    ray.direction = direction;
    ray.origin = getPosition();

    return ray;
}
