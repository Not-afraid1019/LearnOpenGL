// Not_afraid
// 2025/1/6

#ifndef LEARNOPENGL_GRASSINSTANCEMATERIAL_H
#define LEARNOPENGL_GRASSINSTANCEMATERIAL_H

#include "material.h"
#include "../texture.h"

class GrassInstanceMaterial : public Material {
public:
    GrassInstanceMaterial();
    ~GrassInstanceMaterial();

public:
    Texture* mDiffuse{nullptr};
    Texture* mOpacityMask{nullptr};
    float mShiness{1.0f};

    // 草地贴图特性
    float mUVScale{1.0f};
    float mBrightness{1.0f};

    // 风力相关
    float mWindScale{1.0f};
    glm::vec3 mWindDirection{1.0, 1.0, 1.0};
    float mPhaseScale{1.0f};

    // 云朵相关
    Texture* mCloudMask{nullptr};
    glm::vec3 mCloudWhiteColor{1.0, 0.0, 0.0};
    glm::vec3 mCloudBlackColor{0.0, 0.0, 1.0};
    float mCloudUVScale{1.0f};
    float mCloudSpeed{0.1f};
    float mCloudLerp{0.1f};
};


#endif //LEARNOPENGL_GRASSINSTANCEMATERIAL_H
