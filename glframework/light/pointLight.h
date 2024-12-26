// Not_afraid
// 2024/12/25

#ifndef LEARNOPENGL_POINTLIGHT_H
#define LEARNOPENGL_POINTLIGHT_H

#include "light.h"
#include "../object.h"

class PointLight : public Light, public Object {
public:
    PointLight();
    ~PointLight();

public:
    float mK2 = 1.0f;
    float mK1 = 1.0f;
    float mKc = 1.0f;
};


#endif //LEARNOPENGL_POINTLIGHT_H