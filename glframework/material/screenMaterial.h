// Not_afraid
// 2024/12/31

#ifndef LEARNOPENGL_SCREENMATERIAL_H
#define LEARNOPENGL_SCREENMATERIAL_H

#include "material.h"
#include "../texture.h"

class ScreenMaterial : public Material{
public:
    ScreenMaterial();
    ~ScreenMaterial();

public:
    Texture* mScreenTexture{nullptr};
};


#endif //LEARNOPENGL_SCREENMATERIAL_H
