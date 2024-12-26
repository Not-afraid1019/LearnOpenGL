// Not_afraid
// 2024/12/24

#ifndef LEARNOPENGL_DIRECTIONALLIGHT_H
#define LEARNOPENGL_DIRECTIONALLIGHT_H

#include "light.h"

class DirectionalLight : public Light {
public:
    DirectionalLight();
    ~DirectionalLight();

public:
    glm::vec3 mDirection{-1.0};
};


#endif //LEARNOPENGL_DIRECTIONALLIGHT_H
