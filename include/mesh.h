#ifndef __MESH__
#define __MESH__

#include "material.h"
#include "geometry.h"
#include <vector>

class SubMesh {
public:
    SubMesh(const Geometry* geometry, const Material* material);
    const Material* GetMaterial() const { return material; }
    void Draw() const;
private:
    const Geometry* geometry;
    const Material* material;
};

class Mesh {
public:
    Mesh(const std::initializer_list<SubMesh>& t_subMeshes): subMeshes(t_subMeshes) {}
    void Draw(const glm::mat4& projection, const glm::mat4& view, const glm::mat4& model) const;
private:
    std::vector<SubMesh> subMeshes;
};

#endif /* End of Mesh */