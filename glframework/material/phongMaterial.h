// Not_afraid
// 2024/12/24

#ifndef LEARNOPENGL_PHONGMATERIAL_H
#define LEARNOPENGL_PHONGMATERIAL_H

#include "material.h"
#include "../texture.h"

class PhongMaterial : public Material {
public:
    PhongMaterial();
    ~PhongMaterial();

public:
    Texture* mDiffuse{nullptr};
    Texture* mSpecularMask{nullptr};
    float mShiness{1.0f};
};


#endif //LEARNOPENGL_PHONGMATERIAL_H
