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

    // 深度检测相关
    bool mDepthTest{true};
    GLenum mDepthFunc{GL_LESS};
    bool mDepthWrite{true};
};


#endif //LEARNOPENGL_MATERIAL_H
