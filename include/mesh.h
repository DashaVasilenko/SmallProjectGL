#ifndef __MESH__
#define __MESH__

#include "material.h"
#include "geometry.h"
#include "engine.h"
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
    Mesh(const std::initializer_list<SubMesh>& t_subMeshes): subMeshes(t_subMeshes) {
        depthProgram = Engine::programManager.Get("data/shaders/depth.json");
    }
    void Draw(const glm::mat4& projection, const glm::mat4& view, const glm::mat4& model);
    void DepthPass(const glm::mat4& lightMatrix, const glm::mat4& model);
private:
    std::vector<SubMesh> subMeshes;
    ShaderProgram* depthProgram;
};

#endif /* End of Mesh */