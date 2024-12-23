// Not_afraid
// 2024/11/24

#ifndef LEARNOPENGL_TEXTURE_H
#define LEARNOPENGL_TEXTURE_H

#include "core.h"
#include <string>

class Texture {
public:
    Texture(std::string path, unsigned int unit);

    ~Texture();

    void bind();

private:
    GLuint mTexture{0};
    int mWidth{0};
    int mHeight{0};
    unsigned int mUnit{0};
};


#endif //LEARNOPENGL_TEXTURE_H
