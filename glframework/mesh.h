// Not_afraid
// 2024/12/24

#ifndef LEARNOPENGL_MESH_H
#define LEARNOPENGL_MESH_H

#include "object.h"
#include "geometry.h"
#include "material/material.h"

class Mesh : public Object {
public:
    Mesh(Geometry* geometry, Material* material);
    ~Mesh();

public:
    Geometry* mGeometry{nullptr};
    Material* mMaterial{nullptr};
};


#endif //LEARNOPENGL_MESH_H
