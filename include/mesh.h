#ifndef __MESH__
#define __MESH__

#include "material.h"
#include "geometry.h"
#include <vector>

class SubMesh {
public:
    SubMesh(const Geometry* geometry, Material* material);
    Material* GetMaterial() { return material; }
    void Draw();
private:
    const Geometry* geometry;
    Material* material;
};

class Mesh {
public:
    Mesh(const std::initializer_list<SubMesh>& t_subMeshes): subMeshes(t_subMeshes) {}
    void Draw(const glm::mat4& projection, const glm::mat4& view, const glm::mat4& model);
private:
    std::vector<SubMesh> subMeshes;
};

#endif /* End of Mesh */