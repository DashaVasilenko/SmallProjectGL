#ifndef __RENDERER__
#define __RENDERER__

#include "console.h"
#include "window.h"
#include "camera.h"
#include "mesh.h"
#include "transform.h"
#include "entt/entt.hpp"
#include "buffers.h"
#include "geometry.h"
#include "shaderProgram.h"
#include "resourceManager.h"

class Renderer {
public:
    void SetFrameBuffer(GLuint);
    void SetActiveCamera(const Camera* camera);
    void AddMesh(const Mesh*);
    void Init();
    void Update(entt::registry& registry);


    static void SetWidth(int w) {
        width = w;
    }

    static void SetHeight(int h) {
        height = h;
    }

    static int GetWidth() {
        return width;
    }

    static int GetHeight() {
        return height;
    }

    ConsoleFunction GetWireFrameFunc() { return std::bind(&Renderer::WireFrame, this, std::placeholders::_1); }
    ConsoleFunction GetViewBufferFunc() { return std::bind(&Renderer::ViewBuffer, this, std::placeholders::_1);}

private:
    bool WireFrame(const std::vector<std::string>& );
    bool ViewBuffer(const std::vector<std::string>& );
    glm::mat4 projection;
    const Camera* camera;
   

    FrameBuffer fbo;
    GBuffer gbuffer;

    unsigned int current_view_buffer;


    static int width;
    static int height;
};



#endif /* End of __RENDERER__ */