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

    // stencil相关
    bool mStencilTest{false};

    unsigned int mSFail{GL_KEEP};
    unsigned int mZFail{GL_KEEP};
    unsigned int mZPass{GL_KEEP};

    unsigned int mStencilMask{0xFF};

    unsigned int mStencilFunc{GL_ALWAYS};
    unsigned int mStencilRef{0};
    unsigned int mStencilFuncMask{0xFF};
};


#endif //LEARNOPENGL_MATERIAL_H
