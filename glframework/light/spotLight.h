// Not_afraid
// 2024/12/25

#ifndef LEARNOPENGL_SPOTLIGHT_H
#define LEARNOPENGL_SPOTLIGHT_H

#include "light.h"
#include "../object.h"

class SpotLight : public Light, public Object {
public:
    SpotLight();
    ~SpotLight();

public:
    glm::vec3 mTargetDirection{-1.0f};
    float mInnerAngle{30.0f};
    float mOuterAngle{45.0f};
};


#endif //LEARNOPENGL_SPOTLIGHT_H
