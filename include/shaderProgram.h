#ifndef __SHADER_PROGRAM__
#define __SHADER_PROGRAM__

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "window.h"
#include <map>

class ShaderProgram {
public:
    std::string& operator[](const GLenum& );
    void Compile();
    void Link();
    void Run();
    void SetUniform(const char*, const glm::mat4&);
    void SetUniform(const char*, const glm::mat3&);
    void SetUniform(const char*, const glm::vec4&);
    void SetUniform(const char*, const glm::vec3&);
    void Delete();
private:
    std::map<GLenum, std::string> mapSources;
    std::map<GLenum, GLuint> mapShaders;
    GLuint descriptor;
};

#endif /* End of __SHADER_PROGRAM__ */