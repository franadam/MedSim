#pragma once
#include "../Headers/Camera.hpp"

namespace renderer
{

    // Constructor with vectors
    Camera::Camera(glm::vec3 position, glm::vec3 up, GLfloat yaw, GLfloat pitch) : m_front(glm::vec3(0.0f, 0.0f, -1.0f)), m_movementSpeed(SPEED), m_mouseSensitivity(SENSITIVTY), m_zoom(ZOOM)
    {
        m_position = position;
        m_worldUp = up;
        m_yaw = yaw;
        m_pitch = pitch;
        updateCameraVectors();
    }
    // Constructor with scalar values
    Camera::Camera(GLfloat posX, GLfloat posY, GLfloat posZ, GLfloat upX, GLfloat upY, GLfloat upZ, GLfloat yaw, GLfloat pitch) : m_front(glm::vec3(0.0f, 0.0f, -1.0f)), m_movementSpeed(SPEED), m_mouseSensitivity(SENSITIVTY), m_zoom(ZOOM)
    {
        m_position = glm::vec3(posX, posY, posZ);
        m_worldUp = glm::vec3(upX, upY, upZ);
        m_yaw = yaw;
        m_pitch = pitch;
        updateCameraVectors();
    }

    // Returns the view matrix calculated using Eular Angles and the LookAt Matrix
    glm::mat4 Camera::GetViewMatrix()
    {
        return glm::lookAt(m_position, m_position + m_front, m_up);
    }


    // Returns the projection matrix
    glm::mat4 Camera::GetProjectionMatrix(int width, int height)
    {
        return glm::perspective(
            glm::radians(m_zoom),
            (float)width / height,
            0.1f,
            100.0f
        );
    }

    glm::vec3 Camera::getPosition() {
        return m_position;
    }

    void Camera::setPosition(glm::vec3 position) {
        m_position = position;
    }


    // Processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
    void Camera::Camera::ProcessKeyboard(unsigned int  direction, GLfloat deltaTime)
    {
        GLfloat velocity = m_movementSpeed * deltaTime;
        if (direction & FORWARD)
            m_position += m_front * velocity;
        if (direction & BACKWARD)
            m_position -= m_front * velocity;
        if (direction & UP)
            m_position += m_up * velocity;
        if (direction & DOWN)
            m_position -= m_up * velocity;
        if (direction & LEFT)
            m_position -= m_right * velocity;
        if (direction & RIGHT)
            m_position += m_right * velocity;
    }

    // Processes input received from a mouse input system. Expects the offset value in both the x and y direction.
    void Camera::ProcessMouseMovement(GLfloat xoffset, GLfloat yoffset, GLboolean constrainPitch)
    {
        xoffset *= m_mouseSensitivity;
        yoffset *= m_mouseSensitivity;

        m_yaw += xoffset;
        m_pitch += yoffset;

        // Make sure that when pitch is out of bounds, screen doesn't get flipped
        if (constrainPitch)
        {
            if (m_pitch > 89.0f)
                m_pitch = 89.0f;
            if (m_pitch < -89.0f)
                m_pitch = -89.0f;
        }

        // Update m_front, m_right and m_up Vectors using the updated Eular angles
        updateCameraVectors();
    }

    // Processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
    void Camera::ProcessMouseScroll(GLfloat yoffset)
    {
        m_zoom -= yoffset;
        if (m_zoom <= 5.0f)
            m_zoom = 5.0f;
        if (m_zoom >= 60.0f)
            m_zoom = 60.0f;
    }

    // Calculates the front vector from the Camera's (updated) Eular Angles
    void Camera::updateCameraVectors()
    {
        // Calculate the new m_front vector
        glm::vec3 front;
        front.x = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
        front.y = sin(glm::radians(m_pitch));
        front.z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
        m_front = glm::normalize(front);
        // Also re-calculate the m_right and m_up vector
        m_right = glm::normalize(glm::cross(m_front, m_worldUp));  // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
        m_up = glm::normalize(glm::cross(m_right, m_front));
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


    Ray Camera::screenToWorldRay(float screenX, float screenY)
    {
        Ray ray;
        
        ray.origin = getPosition();

        glm::dvec3 remote = glm::unProject(glm::vec3(screenX, gui::WINDOW_HEIGHT - screenY, 1.0f), GetViewMatrix(),
            GetProjectionMatrix(gui::WINDOW_WIDTH, gui::WINDOW_HEIGHT), glm::vec4(0, 0, gui::WINDOW_WIDTH, gui::WINDOW_HEIGHT));
        ray.direction = glm::normalize(remote - ray.origin);
        
        return ray;
    }

    Ray Camera::mouseToWorldRay()
    {
        return screenToWorldRay(gui::mouse_x, gui::mouse_y);
    }

}
