#include "transform.h"

void Transform::ReCalculateModel() {
    model = translation*rotation*scale;
}

void Transform::Scale(const glm::vec3& size) {
    scale = glm::scale(glm::mat4(1.0f), size);
    ReCalculateModel();
}

void Transform::Rotate(const glm::vec3& axis, float angle) {
    rotation = glm::rotate(glm::mat4(1.0f), glm::radians(angle), axis);
    ReCalculateModel();
}

void Transform::Translate(const glm::vec3& offset) {
    translation = glm::translate(glm::mat4(1.0f), offset);
    ReCalculateModel();
} 