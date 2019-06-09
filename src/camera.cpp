#include "camera.h"
#include "inputSystem.h"
#include <glfw/glfw3.h>

Camera::Camera(const glm::vec3& position) {
    this->position = position;
    up = glm::vec3(0.0f, 1.0f, 0.0f);
    right = glm::vec3(1.0f, 0.0f, 0.0f);
    front = glm::vec3(0.0f, 0.0f, -1.0f);
}

glm::mat4 Camera::GetViewMatrix() {
    return glm::lookAt(position, position + front, up);
}

void Camera::Update(const float& delta) {
    UpdatePosition(delta);
    UpdateVectors();
}

void Camera::UpdatePosition(const float& delta) {
    if (InputSystem::keys[GLFW_KEY_W]) {
        position += front*delta*speed;
    }
    if (InputSystem::keys[GLFW_KEY_S]) {
        position += -front*delta*speed;
    }
    if (InputSystem::keys[GLFW_KEY_D]) {
        position += right*delta*speed;
    }
    if (InputSystem::keys[GLFW_KEY_A]) {
        position += -right*delta*speed;
    }
    if (InputSystem::keys[GLFW_KEY_SPACE]) {
        position += up*delta*speed;
    }
    if (InputSystem::keys[GLFW_KEY_LEFT_SHIFT]) {
        position += -up*delta*speed;
    }
}

void Camera::UpdateVectors() {
    if (InputSystem::mouseMoved) {
        yaw += InputSystem::deltaCursPosX;
        pitch += InputSystem::deltaCursPosY;

        if (pitch > 90.0f) pitch = 90.0f;
        if (pitch < -90.0f) pitch = -90.0f;

        glm::quat pitch_quat = glm::angleAxis(pitch, right);
        glm::quat yaw_quat = glm::angleAxis(yaw, up);
        glm::quat result_quat = glm::cross(pitch_quat, yaw_quat);
        glm::normalize(result_quat);
        front = glm::rotate(result_quat, front);
        right = glm::rotate(result_quat, right);

        InputSystem::mouseMoved = false;
    }
}