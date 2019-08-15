#ifndef __RENDERER__
#define __RENDERER__

#include "console.h"
#include "window.h"
#include "camera.h"
#include "mesh.h"
#include "transform.h"
#include "entt/entt.hpp"
//#include "buffers.h"
#include "framebuffers.h"
#include "geometry.h"
#include "shaderProgram.h"
#include "resourceManager.h"
#include "engine.h"

class Renderer {
public:
    void SetFrameBuffer(GLuint);
    void SetActiveCamera(const Camera* camera);
    void Init();
    void Update(entt::registry& registry);
   

    static void SetWidth(int w) { width = w; }
    static void SetHeight(int h) { height = h; }
    static int GetWidth() { return width; }
    static int GetHeight() { return height; }

    ConsoleFunction GetWireFrameFunc() { return std::bind(&Renderer::WireFrame, this, std::placeholders::_1); }
    ConsoleFunction GetViewBufferFunc() { return std::bind(&Renderer::ViewBuffer, this, std::placeholders::_1);}

private:
    void ShadowMapPass(entt::registry& registry);
    void GeometryPass(entt::registry& registry);
    void LightPass(entt::registry& registry);
    void ClearResult();
    void BeginLightPass();
    void EndLightPass();
    void BeginStencilPass();
    void PostProcess();




    bool WireFrame(const std::vector<std::string>& );
    bool ViewBuffer(const std::vector<std::string>& );
    glm::mat4 projection;
    glm::mat4 lightMatrix;
    const Camera* camera;
   
    FrameBuffer fbo;
    GBuffer gbuffer;
    ShadowBuffer shadowbuffer;
    PostProcessBuffer postprocessbuffer;


    Geometry* quad = Engine::geometryManager.Get("data/quad.obj");
    // PostProcessing programs
    ShaderProgram* toneMapPlusBrightness = Engine::programManager.Get("data/shaders/postprocess.json");
    ShaderProgram* textureView = Engine::programManager.Get("data/shaders/quad.json");

    unsigned int current_view_buffer;

    static int width;
    static int height;
};

#endif /* End of __RENDERER__ */