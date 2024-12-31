// Not_afraid
// 2024/12/30

#ifndef LEARNOPENGL_OPACITYMASKMATERIAL_H
#define LEARNOPENGL_OPACITYMASKMATERIAL_H

#include "material.h"
#include "../texture.h"

class OpacityMaskMaterial : public Material {
public:
    OpacityMaskMaterial();
    ~OpacityMaskMaterial();

public:
    Texture* mDiffuse{nullptr};
    Texture* mOpacityMask{nullptr};
    float mShiness{1.0f};
};


#endif //LEARNOPENGL_OPACITYMASKMATERIAL_H
