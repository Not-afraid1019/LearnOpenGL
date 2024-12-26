// Not_afraid
// 2024/12/24

#ifndef LEARNOPENGL_MATERIAL_H
#define LEARNOPENGL_MATERIAL_H

#include "../core.h"

enum class MaterialType {
    PhongMaterial,
    WhiteMaterial
};

class Material {
public:
    Material();
    ~Material();

public:
    MaterialType mType;
};


#endif //LEARNOPENGL_MATERIAL_H
