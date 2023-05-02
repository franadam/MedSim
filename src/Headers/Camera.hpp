#pragma once
#ifndef _CAMERA_
#define _CAMERA_

// System Headers
#include "glitter.hpp"

// Local Headers
#include "Geometry.hpp"
#include "Gui.hpp"

namespace renderer
{

    // Defines several possible options for camera movement. Used as abstraction to stay away from window-system specific input methods
    enum Camera_Movement {
        FORWARD = 1,
        BACKWARD = 2,
        LEFT = 4,
        RIGHT = 8,
        UP = 16,
        DOWN = 32
    };

    // Default camera values
    const GLfloat YAW = -90.0f;
    const GLfloat PITCH = 0.0f;
    const GLfloat SPEED = 0.5f;
    const GLfloat SENSITIVTY = 0.5f;
    const GLfloat ZOOM = 45.0f;


    // An abstract camera class that processes input and calculates the corresponding Eular Angles, Vectors and Matrices for use in OpenGL
    class Camera
    {
    public:
        // Camera Attributes
        glm::vec3 m_position;
        glm::vec3 m_front;
        glm::vec3 m_up;
        glm::vec3 m_right;
        glm::vec3 m_worldUp;
        // Eular Angles
        GLfloat m_yaw;
        GLfloat m_pitch;
        // Camera options
        GLfloat m_movementSpeed;
        GLfloat m_mouseSensitivity;
        GLfloat m_zoom;

        // Constructor with vectors
        Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), GLfloat yaw = YAW, GLfloat pitch = PITCH);
        // Constructor with scalar values
        Camera(GLfloat posX, GLfloat posY, GLfloat posZ, GLfloat upX, GLfloat upY, GLfloat upZ, GLfloat yaw, GLfloat pitch);

        // Returns the view matrix calculated using Eular Angles and the LookAt Matrix
        glm::mat4 GetViewMatrix();
        // Returns the projection matrix
        glm::mat4 GetProjectionMatrix(int width, int height);

        glm::vec3 getPosition();
        void setPosition(glm::vec3 position);

        void cameraControl();

        // Processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
        void ProcessKeyboard(unsigned int  direction, GLfloat deltaTime);

        // Processes input received from a mouse input system. Expects the offset value in both the x and y direction.
        void ProcessMouseMovement(GLfloat xoffset, GLfloat yoffset, GLboolean constrainPitch = true);

        // Processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
        void ProcessMouseScroll(GLfloat yoffset);
        Ray computeCurrentRay();
        Ray computeCurrentRay2();

        Ray screenToWorldRay(float screenX, float screenY);
        Ray mouseToWorldRay();

    private:
        // Calculates the front vector from the Camera's (updated) Eular Angles
        void updateCameraVectors();
        glm::vec3 normalisedCoordinates(double xpos, double ypos);
        glm::vec4 eyeCoordinates(glm::vec4  homogeneousClip);
        glm::vec3 worldCoordinates(glm::vec4  ray_eye);

        

    };
}
#endif 