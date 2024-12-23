// Not_afraid
// 2024/12/23

#include "geometry.h"
#include <vector>

Geometry::Geometry() {

}

Geometry::~Geometry() {
    if (mVao != 0) {
        glDeleteVertexArrays(1, &mVao);
    }
    if (mPosVbo != 0) {
        glDeleteBuffers(1, &mPosVbo);
    }
    if (mUvVbo != 0) {
        glDeleteBuffers(1, &mUvVbo);
    }
    if (mEbo != 0) {
        glDeleteBuffers(1, &mEbo);
    }
}

Geometry *Geometry::createBox(float size) {
    Geometry* geometry = new Geometry();
    geometry->mIndicesCount = 36;

    float halfSize = size / 2.0f;
    // 1 生成顶点数据
    float positions[] = {
            // Front face
            -halfSize, -halfSize,  halfSize, halfSize, -halfSize,  halfSize,halfSize,  halfSize,  halfSize,-halfSize,  halfSize,  halfSize,

            // Back face
            -halfSize, -halfSize, -halfSize, halfSize, -halfSize, -halfSize,halfSize, halfSize, -halfSize,-halfSize,  halfSize, -halfSize,

            // Top face
            -halfSize,  halfSize, -halfSize,halfSize,  halfSize, -halfSize,halfSize, halfSize,  halfSize,-halfSize,  halfSize,  halfSize, // 11

            // Bottom face
            -halfSize, -halfSize, -halfSize, halfSize, -halfSize, -halfSize, halfSize, -halfSize,  halfSize,-halfSize, -halfSize,  halfSize, // 15

            // Right face
            halfSize, -halfSize, -halfSize, halfSize,  halfSize, -halfSize,halfSize, halfSize,  halfSize,halfSize, -halfSize,  halfSize, // 19

            // Left face
            -halfSize, -halfSize, -halfSize,-halfSize,  halfSize, -halfSize,-halfSize, halfSize,  halfSize,-halfSize, -halfSize,  halfSize  // 23
    };

    float uvs[] = {
            // Front face
            0.0, 0.0, 1.0, 0.0,1.0, 1.0,0.0, 1.0,  // 3

            // Back face
            0.0, 0.0,1.0, 0.0,1.0, 1.0,0.0, 1.0,  // 7

            // Top face
            0.0, 0.0,1.0, 0.0,1.0, 1.0, 0.0, 1.0,  // 11

            // Bottom face
            0.0, 0.0,1.0, 0.0,1.0, 1.0,0.0, 1.0,  // 15

            // Right face
            0.0, 0.0,1.0, 0.0,1.0, 1.0,0.0, 1.0,  // 19

            // Left face
            0.0, 0.0,1.0, 0.0,1.0, 1.0,0.0, 1.0   // 23
    };

    unsigned int indices[] = {
            // Front face
            0, 2, 1,0, 3, 2,

            // Back face
            4, 5, 6,4, 6, 7,

            // Top face
            8, 9, 10,8, 10, 11,

            // Bottom face
            12, 14, 13,12, 15, 14,

            // Right face
            16, 18, 17,16, 19, 18,

            // Left face
            20, 21, 22,20, 22, 23
    };

    // 2 VBO创建
    GLuint& posVbo = geometry->mPosVbo, uvVbo = geometry->mUvVbo;
    glGenBuffers(1, &posVbo);
    glBindBuffer(GL_ARRAY_BUFFER, posVbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_STATIC_DRAW);

    glGenBuffers(1, &uvVbo);
    glBindBuffer(GL_ARRAY_BUFFER, uvVbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(uvs), uvs, GL_STATIC_DRAW);

    // 3 创建EBO
    glGenBuffers(1, &geometry->mEbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, geometry->mEbo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // 4 创建VAO
    glGenVertexArrays(1, &geometry->mVao);
    glBindVertexArray(geometry->mVao);

    glBindBuffer(GL_ARRAY_BUFFER, posVbo);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);

    glBindBuffer(GL_ARRAY_BUFFER, uvVbo);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, (void*)0);

    // 5 加入ebo到当前的vao
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, geometry->mEbo);

    glBindVertexArray(0);

    return geometry;
}

Geometry *Geometry::createSphere(float radius) {
    Geometry* geometry = new Geometry();

    // 目标：1 位置 2 uv 3 索引
    // 1 主要变量声明
    std::vector<GLfloat> positions{};
    std::vector<GLfloat> uvs{};
    std::vector<GLuint> indices{};

    // 声明纬线与经线的数量
    int numLatLines = 60;
    int numLongLines = 60;

    // 2 通过两层循环（纬线在外，经线在内） -> 位置、uv
    for (int i = 0; i <= numLatLines; ++i) {
        for (int j = 0; j <= numLongLines; ++j) {
            float phi = i * glm::pi<float>() / numLatLines;
            float theta = j * 2 * glm::pi<float>() / numLongLines;

            float y = radius * cos(phi);
            float x = radius * sin(phi) * cos(theta);
            float z = radius * sin(phi) * sin(theta);

            positions.push_back(x);
            positions.push_back(y);
            positions.push_back(z);

            float u = 1.0f - (float)j / (float)numLongLines;
            float v = 1.0f - (float)i / (float)numLatLines;

            uvs.push_back(u);
            uvs.push_back(v);
        }
    }

    // 3 通过两层循环 -> 顶点索引
    for (int i = 0; i < numLatLines; ++i) {
        for (int j = 0; j < numLongLines; ++j) {
            int p1 = i * (numLongLines + 1) + j;
            int p2 = p1 + numLongLines + 1;
            int p3 = p1 + 1;
            int p4 = p2 + 1;

            indices.push_back(p1);
            indices.push_back(p2);
            indices.push_back(p3);

            indices.push_back(p3);
            indices.push_back(p2);
            indices.push_back(p4);
        }
    }

    // 4 生成vbo与vao
    GLuint& posVbo = geometry->mPosVbo, uvVbo = geometry->mUvVbo;
    glGenBuffers(1, &posVbo);
    glBindBuffer(GL_ARRAY_BUFFER, posVbo);
    glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(float), positions.data(), GL_STATIC_DRAW);

    glGenBuffers(1, &uvVbo);
    glBindBuffer(GL_ARRAY_BUFFER, uvVbo);
    glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(float), uvs.data(), GL_STATIC_DRAW);

    glGenBuffers(1, &geometry->mEbo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, geometry->mEbo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);

    glGenVertexArrays(1, &geometry->mVao);
    glBindVertexArray(geometry->mVao);

    glBindBuffer(GL_ARRAY_BUFFER, posVbo);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 3, (void*)0);

    glBindBuffer(GL_ARRAY_BUFFER, uvVbo);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, (void*)0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, geometry->mEbo);

    glBindVertexArray(0);

    geometry->mIndicesCount = indices.size();

    return geometry;
}