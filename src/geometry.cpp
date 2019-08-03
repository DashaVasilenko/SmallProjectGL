#include "geometry.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

void Geometry::Load(const std::string& filename) {
    bool texture_flag = false;
    bool normal_flag = false;
    std::vector<GLfloat> vertices;
    Assimp::Importer Importer;
    const aiScene* pScene = Importer.ReadFile(filename.c_str(), aiProcess_Triangulate | aiProcess_CalcTangentSpace);
    if (pScene) {
        for (uint i = 0; i < pScene->mNumMeshes; i++) {
            const aiMesh* mesh = pScene->mMeshes[i];
            count += mesh->mNumVertices;
        }
        //vertices.reserve(3*count);

        for (uint i = 0; i < pScene->mNumMeshes; i++) {
            const aiMesh* mesh = pScene->mMeshes[i];
            for (uint i = 0; i < mesh->mNumVertices; i++) {
                vertices.push_back(mesh->mVertices[i].x);
                vertices.push_back(mesh->mVertices[i].y);
                vertices.push_back(mesh->mVertices[i].z);
                if (mesh->HasNormals()) {
                    vertices.push_back(mesh->mNormals[i].x);
                    vertices.push_back(mesh->mNormals[i].y);
                    vertices.push_back(mesh->mNormals[i].z);
                    normal_flag = true;
                }
                for (int j = 0; mesh->HasTextureCoords(j); j++) {
                    vertices.push_back(mesh->mTextureCoords[j][i].x);
                    vertices.push_back(mesh->mTextureCoords[j][i].y);
                    texture_flag = true;
                }
                if (mesh->HasTangentsAndBitangents()) {
                    vertices.push_back(mesh->mTangents[i].x);
                    vertices.push_back(mesh->mTangents[i].y);
                    vertices.push_back(mesh->mTangents[i].z);
                    vertices.push_back(mesh->mBitangents[i].x);
                    vertices.push_back(mesh->mBitangents[i].y);
                    vertices.push_back(mesh->mBitangents[i].z);
                }
            }
        }
        // Determine layout of Mesh!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
        if (normal_flag) {
            if (texture_flag) {
                BufferLayout layout = { {"Position", Float3}, {"Normals", Float3} , {"Textures", Float2}, {"Tangents", Float3}, {"Bitangents", Float3}};
                Init(vertices, layout); 
            }
            else {
                BufferLayout layout = { {"Position", Float3}, {"Normals", Float3} };
                Init(vertices, layout); 
            }
        }
        else {
            if (texture_flag) {
                BufferLayout layout = { {"Position", Float3}, {"Textures", Float2} };
                Init(vertices, layout);
            }
            else {
                BufferLayout layout = { {"Position", Float3} };
                Init(vertices, layout);
            }
        }
        //Init(vertices, layout);
    }
}

void Geometry::Init(const std::vector<GLfloat>& vertices, const BufferLayout& layout) {
    vbo.BufferData((void *)vertices.data(), vertices.size()*sizeof(GLfloat));
    vao.AddAttributes(vbo, layout);
}

void Geometry::Draw() const {
    vao.Bind();
    glDrawArrays(GL_TRIANGLES, 0, count);
}