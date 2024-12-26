// Not_afraid
// 2024/12/26

#include "assimpLoader.h"
#include "../glframework/tools/tools.h"
#include "../glframework/material/phongMaterial.h"
#include "../glframework/texture.h"

Object *AssimpLoader::load(const std::string &path) {
    Object* rootNode = new Object();

    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_GenNormals);

    // 验证读取是否正确顺利
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        std::cerr << "Error:Model Read Failed!" << std::endl;
        return nullptr;
    }

    processNode(scene->mRootNode, rootNode, scene);

    return rootNode;
}

void AssimpLoader::processNode(aiNode *ainode, Object* parent, const aiScene* scene) {
    Object* node = new Object();
    parent->addChild(node);

    glm::mat4 localMatrix = getMat4f(ainode->mTransformation);
    // 位置 旋转 缩放
    glm::vec3 position, eulerAngle, scale;
    Tools::decompose(localMatrix, position, eulerAngle, scale);
    node->setPosition(position);
    node->setAngleX(eulerAngle.x);
    node->setAngleY(eulerAngle.y);
    node->setAngleZ(eulerAngle.z);
    node->setScale(scale);

    // 检查有没有mesh，并解析
    for (int i = 0; i < ainode->mNumMeshes; ++i) {
        int meshID = ainode->mMeshes[i];
        aiMesh* aimesh = scene->mMeshes[meshID];
        auto mesh = processMesh(aimesh);
        node->addChild(mesh);
    }

    for (int i = 0; i < ainode->mNumChildren; ++i) {
        processNode(ainode->mChildren[i], node, scene);
    }
}

Mesh* AssimpLoader::processMesh(aiMesh *aimesh) {
    std::vector<float> positions;
    std::vector<float> normals;
    std::vector<float> uvs;
    std::vector<unsigned int> indices;

    for (int i = 0; i < aimesh->mNumVertices; ++i) {
        // 第i个顶点的位置
        positions.push_back(aimesh->mVertices[i].x);
        positions.push_back(aimesh->mVertices[i].y);
        positions.push_back(aimesh->mVertices[i].z);

        // 第i个顶点的法线
        normals.push_back(aimesh->mNormals[i].x);
        normals.push_back(aimesh->mNormals[i].y);
        normals.push_back(aimesh->mNormals[i].z);

        // 第i个顶点的uv
        // 关注其第0套uv，一般情况下是贴图uv
        if (aimesh->mTextureCoords[0]) {
            uvs.push_back(aimesh->mTextureCoords[0][i].x);
            uvs.push_back(aimesh->mTextureCoords[0][i].y);
        } else {
            uvs.push_back(0.0f);
            uvs.push_back(0.0f);
        }
    }

    // 解析mesh中的索引值
    for (int i = 0; i < aimesh->mNumFaces; ++i) {
        aiFace face = aimesh->mFaces[i];
        for (int j = 0; j < face.mNumIndices; ++j) {
            indices.push_back(face.mIndices[j]);
        }
    }

    auto geometry = new Geometry(positions, normals, uvs, indices);
    auto material = new PhongMaterial();
    material->mDiffuse = new Texture("assets/textures/grass.png", 0);

    return new Mesh(geometry, material);
}

glm::mat4 AssimpLoader::getMat4f(aiMatrix4x4 value) {
    glm::mat4 to(
            value.a1, value.a2, value.a3, value.a4,
            value.b1, value.b2, value.b3, value.b4,
            value.c1, value.c2, value.c3, value.c4,
            value.d1, value.d2, value.d3, value.d4
            );

    return to;
}