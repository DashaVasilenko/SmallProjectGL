#ifndef __RENDERER__
#define __RENDERER__

#include "console.h"
#include "window.h"
#include "camera.h"
#include "mesh.h"
#include "transform.h"
#include "entt/entt.hpp"
#include "buffers.h"

class Renderer {
public:
    void SetWidth(int);
    void SetHeight(int);
    void SetFrameBuffer(GLuint);
    void SetActiveCamera(const Camera* camera);
    void AddMesh(const Mesh*);
    void Init();
    void Update(entt::registry& registry);

    ConsoleFunction GetWireFrameFunc() { return std::bind(&Renderer::WireFrame, this, std::placeholders::_1); }
private:
    bool WireFrame(const std::vector<std::string>& );
    glm::mat4 projection;
    const Camera* camera;
    int width;
    int height;

    FrameBuffer fbo;
};

#endif /* End of __RENDERER__ */