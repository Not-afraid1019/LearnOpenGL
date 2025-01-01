// Not_afraid
// 2024/11/24

#ifndef LEARNOPENGL_TEXTURE_H
#define LEARNOPENGL_TEXTURE_H

#include "core.h"
#include <string>

class Texture {
public:
    static Texture* createTexture(const std::string& path, unsigned int unit);
    static Texture* createTextureFromMemory(const std::string& path, unsigned int unit, unsigned char* dataIn, uint32_t widthIn, uint32_t heightIn);
    static Texture* createColorAttachment(unsigned int width, unsigned int height, unsigned int unit);
    static Texture* createDepthStencilAttachment(unsigned int width, unsigned int height, unsigned int unit);

    Texture();
    Texture(const std::string& path, unsigned int unit);
    Texture(unsigned int unit, unsigned char* dataIn, uint32_t widthIn, uint32_t heightIn);
    Texture(unsigned int width, unsigned int height, unsigned int unit);
    ~Texture();

    void bind();
    void setUnit(unsigned int unit) { mUnit = unit; }

    int getWidth() const { return mWidth; }
    int getHeight() const { return mHeight; }
    GLuint getTexture() const { return mTexture; }

private:
    GLuint mTexture{0};
    int mWidth{0};
    int mHeight{0};
    unsigned int mUnit{0};

    static std::map<std::string, Texture*> mTextureCache;
};


#endif //LEARNOPENGL_TEXTURE_H
