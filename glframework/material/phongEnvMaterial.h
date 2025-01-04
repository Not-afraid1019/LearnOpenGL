// Not_afraid
// 2025/1/4

#ifndef LEARNOPENGL_PHONGENVMATERIAL_H
#define LEARNOPENGL_PHONGENVMATERIAL_H

#include "material.h"
#include "../texture.h"

class PhongEnvMaterial : public Material {
public:
    PhongEnvMaterial();
    ~PhongEnvMaterial();

public:
    Texture* mDiffuse{nullptr};
    Texture* mSpecularMask{nullptr};
    Texture* mEnv{nullptr};
    float mShiness{1.0f};
};


#endif //LEARNOPENGL_PHONGENVMATERIAL_H
