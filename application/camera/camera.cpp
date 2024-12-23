// Not_afraid
// 2024/11/25

#include "camera.h"

Camera::Camera() {

}

Camera::~Camera() {

}

glm::mat4 Camera::getViewMatrix() {
    glm::vec3 front = glm::cross(mUp, mRight);
    glm::vec3 center = mPosition + front;

    return glm::lookAt(mPosition, center, mUp);
}

void Camera::scale(float deltaScale) {

}