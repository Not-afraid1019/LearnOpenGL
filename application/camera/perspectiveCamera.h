// Not_afraid
// 2024/11/25

#ifndef LEARNOPENGL_PERSPECTIVECAMERA_H
#define LEARNOPENGL_PERSPECTIVECAMERA_H

#include "camera.h"

class PerspectiveCamera : public Camera {
public:
    PerspectiveCamera(float fovy, float aspect, float near, float far);
    ~PerspectiveCamera();

    glm::mat4 getProjectionMatrix() override;

    void scale(float deltaScale) override;

private:
    float mFovy = 0.0f;
    float mAspect = 0.0f;
    float mNear = 0.0f;
    float mFar = 0.0f;
};


#endif //LEARNOPENGL_PERSPECTIVECAMERA_H
