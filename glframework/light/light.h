// Not_afraid
// 2024/12/24

#ifndef LEARNOPENGL_LIGHT_H
#define LEARNOPENGL_LIGHT_H

#include "../core.h"

class Light {
public:
    Light();
    ~Light();

public:
    glm::vec3 mColor{1.0f};
    float mSpecularIntensity{1.0f};
    float mIntensity{1.0f};
};


#endif //LEARNOPENGL_LIGHT_H
