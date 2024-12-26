// Not_afraid
// 2024/11/24

#include "texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include "../application/stb_image.h"

//Texture *Texture::createTexture(const std::string &path, unsigned int unit) {
//    // 1 检查是否缓存过本路径对应的纹理对象
//    auto iter = mTextureCache.find(path);
//    if (iter != mTextureCache.end()) {
//        return iter->second;
//    }
//
//    // 2 如果本路径对应的texture没有生成过，则重新生成
//    auto texture = new Texture(path, unit);
//    mTextureCache[path] = texture;
//
//    return texture;
//}
//
//Texture *Texture::createTextureFromMemory(const std::string &path, unsigned int unit, unsigned char *dataIn, uint32_t widthIn,
//                                uint32_t heightIn) {
//    // 1 检查是否缓存过本路径对应的纹理对象
//    auto iter = mTextureCache.find(path);
//    if (iter != mTextureCache.end()) {
//        return iter->second;
//    }
//
//    // 2 如果本路径对应的texture没有生成过，则重新生成
//    auto texture = new Texture(unit, dataIn, widthIn, heightIn);
//    mTextureCache[path] = texture;
//
//    return texture;
//}

Texture::Texture(std::string path, unsigned int unit) {
    mUnit = unit;
    // 1.stb_image 读取图片
    int channels;

    // 反转y轴
    stbi_set_flip_vertically_on_load(true);

    unsigned char* data = stbi_load(path.c_str(), &mWidth, &mHeight, &channels, STBI_rgb_alpha);
    // 2.生成纹理并且激活单元绑定
    glGenTextures(1, &mTexture);
    // 激活纹理单元
    glActiveTexture(GL_TEXTURE0 + mUnit);
    // 绑定纹理对象
    glBindTexture(GL_TEXTURE_2D, mTexture);

    // 3.传输纹理数据,开辟显存
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, mWidth, mHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

    glGenerateMipmap(GL_TEXTURE_2D);

    // 释放数据
    stbi_image_free(data);

    // 4.设置纹理过滤方式
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);

    // 4.设置纹理包裹方式
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
}

Texture::Texture(unsigned int unit, unsigned char *dataIn, uint32_t widthIn, uint32_t heightIn) {
    mUnit = unit;
    // 1.stb_image 读取图片
    int channels;

    // 反转y轴
    stbi_set_flip_vertically_on_load(true);

    // 计算整张图片的大小
    // 如果内嵌纹理是png或者jpg压缩格式的话，height=0，width就代表了图片大小
    uint32_t dataInSize = 0;
    if (!heightIn) {
        dataInSize = widthIn;
    } else {
        // 如果内嵌图片不是压缩格式，height=正常高度，width=正常宽度
        dataInSize = widthIn * heightIn * 4;    // 字节为单位
    }

    unsigned char* data = stbi_load_from_memory(dataIn, dataInSize, &mWidth, &mHeight, &channels, STBI_rgb_alpha);
    // 2.生成纹理并且激活单元绑定
    glGenTextures(1, &mTexture);
    // 激活纹理单元
    glActiveTexture(GL_TEXTURE0 + mUnit);
    // 绑定纹理对象
    glBindTexture(GL_TEXTURE_2D, mTexture);

    // 3.传输纹理数据,开辟显存
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, mWidth, mHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

    glGenerateMipmap(GL_TEXTURE_2D);

    // 释放数据
    stbi_image_free(data);

    // 4.设置纹理过滤方式
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);

    // 4.设置纹理包裹方式
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
}

Texture::~Texture() {
    if (mTexture != 0) {
        glDeleteTextures(1, &mTexture);
    }
}

void Texture::bind() {
    //先切换纹理单元，然后绑定texture对象
    glActiveTexture(GL_TEXTURE0 + mUnit);
    glBindTexture(GL_TEXTURE_2D, mTexture);
}