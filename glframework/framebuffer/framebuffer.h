// Not_afraid
// 2025/1/1

#ifndef LEARNOPENGL_FRAMEBUFFER_H
#define LEARNOPENGL_FRAMEBUFFER_H

#include "../core.h"
#include "../texture.h"

class Framebuffer {
public:
    Framebuffer(unsigned int width, unsigned int height);
    ~Framebuffer();
public:
    unsigned int mWidth{0};
    unsigned int mHeight{0};

    unsigned int mFBO{0};
    Texture* mColorAttachment{nullptr};
    Texture* mDepthStencilAttachment{nullptr};
};


#endif //LEARNOPENGL_FRAMEBUFFER_H
