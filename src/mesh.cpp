#include "mesh.h"

SubMesh::SubMesh(const Geometry* geometry, const Material* material) {
    this->geometry = geometry;
    this->material = material;
}

void SubMesh::Draw() const {
    material->SetInnerUniforms();
    geometry->Draw();
}

void Mesh::Draw(const glm::mat4& projection, const glm::mat4& view, const glm::mat4& model) const {
    for (const auto& subMesh: subMeshes) {
        auto material = subMesh.GetMaterial();
        material->Bind();
        material->SetProjectionMatrix(projection);
        material->SetViewMatrix(view);
        material->SetModelMatrix(model);
        glm::mat3 modelView = view*model;
        glm::mat3 normalMatrix = glm::transpose(glm::inverse(modelView));
        material->SetNormalMatrix(normalMatrix);
        subMesh.Draw();
    }
}