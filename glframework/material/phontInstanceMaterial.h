// Not_afraid
// 2025/1/5

#ifndef LEARNOPENGL_PHONTINSTANCEMATERIAL_H
#define LEARNOPENGL_PHONTINSTANCEMATERIAL_H

#include "material.h"
#include "../texture.h"

class PhongInstanceMaterial : public Material{
public:
    PhongInstanceMaterial();
    ~PhongInstanceMaterial();

public:
    Texture* mDiffuse{nullptr};
    Texture* mSpecularMask{nullptr};
    float mShiness{1.0f};
};


#endif //LEARNOPENGL_PHONTINSTANCEMATERIAL_H
