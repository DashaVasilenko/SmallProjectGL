#ifndef __SHADER_PROGRAM__
#define __SHADER_PROGRAM__

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "json.hpp"
using json = nlohmann::json;

#include "window.h"
#include <map>

class ShaderProgram {
public:
    std::string& operator[](const GLenum& );
    void Load(const std::string& filename);
    void Compile();
    void Link();
    void Run() const;
    void SetUniform(const char*, const glm::mat4&) const;
    void SetUniform(const char*, const glm::mat3&) const;
    void SetUniform(const char*, const glm::vec4&) const;
    void SetUniform(const char*, const glm::vec3&) const;
    void SetUniform(const char*, float) const;
    void SetUniform(const char*, int) const;
    ~ShaderProgram();
private:
    std::map<GLenum, std::string> mapSources;
    std::map<GLenum, GLuint> mapShaders;
    GLuint descriptor;
};

#endif /* End of __SHADER_PROGRAM__ */