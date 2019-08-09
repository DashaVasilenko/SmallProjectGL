#include "camera.h"
#include "inputSystem.h"
#include <GLFW/glfw3.h>

Camera::Camera(const glm::vec3& position) {
    this->position = position;
    up = glm::vec3(0.0f, 1.0f, 0.0f);
    right = glm::vec3(1.0f, 0.0f, 0.0f);
    front = glm::vec3(0.0f, 0.0f, -1.0f);
}

void Camera::SetPitch(float pitch) {
    if (pitch > 89.0f) {
        this->pitch = 89.0f;
    }
    else if (pitch < -89.0f) {
        this->pitch = -89.0f;
    }
    else {
        this->pitch = pitch;
    }
}

void  Camera::SetFront(const glm::vec3& dir) {
    front = dir;
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
    yaw += InputSystem::deltaCursPosX*mouse_sense;
    pitch += InputSystem::deltaCursPosY*mouse_sense;

    if (pitch > 89.0f) pitch = 89.0f;
    if (pitch < -89.0f) pitch = -89.0f;

    glm::vec3 tmpFront;
    tmpFront.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    tmpFront.y = sin(glm::radians(pitch));
    tmpFront.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

    front = glm::normalize(tmpFront);
    right = glm::normalize(glm::cross(front, up));

    InputSystem::deltaCursPosX = 0.0f;
    InputSystem::deltaCursPosY = 0.0f;
}