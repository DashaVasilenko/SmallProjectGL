#ifndef __TRANSFORM__
#define __TRANSFORM__


#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Transform {
public:
    inline glm::mat4 GetModelMatrix() const { return model; }
    void Scale(const glm::vec3&);
    void Rotate(const glm::vec3&, float);
    void Translate(const glm::vec3&);
private:
    void ReCalculateModel();
    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 scale = glm::mat4(1.0f);
    glm::mat4 rotation = glm::mat4(1.0f);
    glm::mat4 translation = glm::mat4(1.0f);
};

#endif /* End of Transform */