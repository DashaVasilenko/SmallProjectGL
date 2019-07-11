#ifndef __MESH__
#define __MESH__

#include "material.h"
#include "geometry.h"
#include <vector>

class SubMesh {
public:
    SubMesh(const Geometry* geometry, const Material* material);
    const Material* GetMaterial() { return material; }
    void Draw() const;
private:
    const Geometry* geometry;
    const Material* material;
};

class Mesh {
public:
    Mesh(const std::initializer_list<SubMesh>& t_subMeshes): subMeshes(t_subMeshes) {}
    void Draw(const glm::mat4& projection, const glm::mat4& view);
    void SetModelMatrix(const glm::mat4& model) { this->model = model; }
    glm::mat4 GetModelMatrix() const { return model; }
private:
    std::vector<SubMesh> subMeshes;
    glm::mat4 model = glm::mat4(1.0f);
};

#endif /* End of Mesh */