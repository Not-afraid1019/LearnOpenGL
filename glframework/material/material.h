// Not_afraid
// 2024/12/24

#ifndef LEARNOPENGL_MATERIAL_H
#define LEARNOPENGL_MATERIAL_H

#include "../core.h"

enum class MaterialType {
    PhongMaterial,
    WhiteMaterial,
    DepthMaterial
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

    // polygonOffset相关
    bool mPolygonOffset{false};
    unsigned int mPolygonOffsetType{GL_POLYGON_OFFSET_FILL};
    float mFactor{0.0f};
    float mUnit{0.0f};
};


#endif //LEARNOPENGL_MATERIAL_H
