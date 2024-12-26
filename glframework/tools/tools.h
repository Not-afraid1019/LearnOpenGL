// Not_afraid
// 2024/12/26

#ifndef LEARNOPENGL_TOOLS_H
#define LEARNOPENGL_TOOLS_H

#include "../core.h"

class Tools {
public:
    // 传入一个矩阵，解构其中的位置、旋转信息XYZ、缩放信息
    static void decompose(glm::mat4 matrix, glm::vec3& position, glm::vec3& eulerAngle, glm::vec3& scale);
};


#endif //LEARNOPENGL_TOOLS_H
