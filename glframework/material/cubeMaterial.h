// Not_afraid
// 2025/1/1

#ifndef LEARNOPENGL_CUBEMATERIAL_H
#define LEARNOPENGL_CUBEMATERIAL_H

#include "material.h"
#include "../texture.h"

class CubeMaterial : public Material {
public:
    CubeMaterial();
    ~CubeMaterial();

public:
    Texture* mDiffuse{nullptr};
};


#endif //LEARNOPENGL_CUBEMATERIAL_H
