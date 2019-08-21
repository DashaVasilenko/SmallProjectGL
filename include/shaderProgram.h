#ifndef __SHADER_PROGRAM__
#define __SHADER_PROGRAM__

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "json.hpp"
using json = nlohmann::json;

#include "window.h"
#include <map>
#include <unordered_map>
#include <string>

class ShaderProgram {
public:
    //std::string& operator[](const GLenum& );
    void Load(const std::string& filename);
    void Compile();
    void Link();
    void Delete();
    void Run() const;
    void SetUniform(const std::string&, const glm::mat4&);
    void SetUniform(const std::string&, const glm::mat3&);
    void SetUniform(const std::string&, const glm::vec4&);
    void SetUniform(const std::string&, const glm::vec3&);
    void SetUniform(const std::string&, const glm::vec2&);
    void SetUniform(const std::string&, float);
    void SetUniform(const std::string&, int);
    ~ShaderProgram();
private:
    std::string name;
    std::unordered_map<std::string, GLint> uniformCache;
    std::map<GLenum, std::string> mapSources;
    std::map<GLenum, GLuint> mapShaders;
    GLuint descriptor;
};

#endif /* End of __SHADER_PROGRAM__ */