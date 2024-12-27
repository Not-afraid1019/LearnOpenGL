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
#include "../glframework/texture.h"

class AssimpLoader {
public:
    static Object* load(const std::string& path);

private:
    static void processNode(aiNode* ainode, Object* parent, const aiScene* scene, const std::string& rootPath);
    static Mesh* processMesh(aiMesh* aimesh, const aiScene* scene, const std::string& rootPath);

    static Texture* processTexture(const aiMaterial* aiMat, const aiTextureType& type, const aiScene* scene, const std::string& rootPath);

    static glm::mat4 getMat4f(aiMatrix4x4 value);
};


#endif //LEARNOPENGL_ASSIMPLOADER_H
