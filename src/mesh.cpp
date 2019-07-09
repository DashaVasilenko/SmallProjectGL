#include "mesh.h"

SubMesh::SubMesh(const Geometry* geometry, const Material* material) {
    this->geometry = geometry;
    this->material = material;
}

void SubMesh::Draw() const {
    material->Bind();
    material->SetInnerUniforms();
    geometry->Draw();
}

void Mesh::Draw() {
    for (auto& subMesh: subMeshes) {
        subMesh.SetModelMatrix(this->model);
        subMesh.Draw();
    }
}