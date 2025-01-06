// Not_afraid
// 2025/1/5

#ifndef LEARNOPENGL_ASSIMPINSTANCELOADER_H
#define LEARNOPENGL_ASSIMPINSTANCELOADER_H

#include "../glframework/core.h"
#include "../glframework/object.h"

#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"

#include "../glframework/mesh/mesh.h"
#include "../glframework/mesh/instanceMesh.h"
#include "../glframework/texture.h"

class AssimpInstanceLoader {
public:
    static Object* load(const std::string& path, int instanceCount);

private:
    static void processNode(aiNode* ainode, Object* parent, const aiScene* scene, const std::string& rootPath, int instanceCount);
    static InstanceMesh* processMesh(aiMesh* aimesh, const aiScene* scene, const std::string& rootPath, int instanceCount);

    static Texture* processTexture(const aiMaterial* aiMat, const aiTextureType& type, const aiScene* scene, const std::string& rootPath);

    static glm::mat4 getMat4f(aiMatrix4x4 value);
};


#endif //LEARNOPENGL_ASSIMPINSTANCELOADER_H