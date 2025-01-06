// Not_afraid
// 2025/1/5

#ifndef LEARNOPENGL_INSTANCEMESH_H
#define LEARNOPENGL_INSTANCEMESH_H

#include "mesh.h"

class InstanceMesh : public Mesh {
public:
    InstanceMesh(Geometry* geometry, Material* material, unsigned int instanceCount);
    ~InstanceMesh();

    void updateMatrices();
    void sortMatrices(glm::mat4 viewMatrix);
public:
    unsigned int mInstanceCount{0};
    std::vector<glm::mat4> mInstanceMatrices{};
    unsigned int mMatrixVbo{0};
};


#endif //LEARNOPENGL_INSTANCEMESH_H
