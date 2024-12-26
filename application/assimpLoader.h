// Not_afraid
// 2024/12/26

#ifndef LEARNOPENGL_ASSIMPLOADER_H
#define LEARNOPENGL_ASSIMPLOADER_H

#include "../glframework/core.h"
#include "../glframework/object.h"

#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"

#include "../glframework/mesh.h"

class AssimpLoader {
public:
    static Object* load(const std::string& path);

private:
    static void processNode(aiNode* ainode, Object* parent, const aiScene* scene);
    static Mesh* processMesh(aiMesh* aimesh);

    static glm::mat4 getMat4f(aiMatrix4x4 value);
};


#endif //LEARNOPENGL_ASSIMPLOADER_H
