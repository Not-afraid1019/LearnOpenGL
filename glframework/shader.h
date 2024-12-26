// Not_afraid
// 2024/11/23

#ifndef LEARNOPENGL_SHADER_H
#define LEARNOPENGL_SHADER_H

#include "core.h"
#include <string>

class Shader {
public:
    Shader(const char *vertexPath, const char *fragmentPath);

    ~Shader();

    void start();

    void end();

    void setFloat(const std::string name, const float value);

    void setVector3(const std::string name, const float x, const float y, const float z);
    void setVector3(const std::string name, const float* values);
    void setVector3(const std::string name, const glm::vec3 value);

    void setInt(const std::string name, const int value);

    void setMatrix4x4(const std::string name, glm::mat4 value);
    void setMatrix3x3(const std::string name, glm::mat3 value);

    GLuint getProgram() const {
        return mProgram;
    }

private:
    // shader program
    // type: COMPILE LINK
    void checkShaderErrors(GLuint target, std::string type);

private:
    GLuint mProgram{0};
};


#endif //LEARNOPENGL_SHADER_H
