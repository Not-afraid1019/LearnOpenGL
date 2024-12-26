// Not_afraid
// 2024/11/25

#ifndef LEARNOPENGL_CAMERA_H
#define LEARNOPENGL_CAMERA_H

#include "../../glframework/core.h"

class Camera {
public:
    Camera();
    ~Camera();

    glm::mat4 getViewMatrix();

    virtual glm::mat4 getProjectionMatrix() = 0;
    virtual void scale(float deltaScale);

public:
    glm::vec3 mPosition{0.0f, 0.0f, 8.0f};
    glm::vec3 mUp{0.0f, 1.0f, 0.0f};
    glm::vec3 mRight{1.0f, 0.0f, 0.0f};
};


#endif //LEARNOPENGL_CAMERA_H
