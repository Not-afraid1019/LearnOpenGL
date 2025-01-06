// Not_afraid
// 2024/12/24

#ifndef LEARNOPENGL_MATERIAL_H
#define LEARNOPENGL_MATERIAL_H

#include "../core.h"

enum class MaterialType {
    PhongMaterial,
    WhiteMaterial,
    DepthMaterial,
    OpacityMaskMaterial,
    ScreenMaterial,
    CubeMaterial,
    PhongEnvMaterial,
    PhongInstanceMaterial,
    GrassInstanceMaterial
};

class Material {
public:
    Material();
    ~Material();

public:
    MaterialType mType;

    // 深度检测相关
    bool mDepthTest{true};
    GLenum mDepthFunc{GL_LEQUAL};
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

    // 颜色混合相关
    bool mBlend{false};
    unsigned int mSFactor{GL_SRC_ALPHA};
    unsigned int mDFactor{GL_ONE_MINUS_SRC_ALPHA};

    float mOpacity{1.0f};

    // 面剔除
    bool mFaceCulling{false};
    unsigned int mFrontFace{GL_CCW};
    unsigned int mCullFace{GL_BACK};
};


#endif //LEARNOPENGL_MATERIAL_H
