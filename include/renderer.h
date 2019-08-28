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
#include "skybox.h"
#include "errors.h"

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
    ConsoleFunction GetDebugLightFunc() { return std::bind(&Renderer::DebugLight, this, std::placeholders::_1);}

private:
    void ClearResult();
    void ShadowMapPass(entt::registry& registry);
    void GeometryPass(entt::registry& registry);
    void LightPass(entt::registry& registry);
    void PostProcess();
    void SkyBoxRender(entt::registry& registry);
    //void BeginForwardRendering();
    void DebugLightDraw(entt::registry& registry);
 
    void BeginLightPass();
    void EndLightPass();
    void BeginStencilPass();
    
    bool WireFrame(const std::vector<std::string>& );
    bool ViewBuffer(const std::vector<std::string>& );
    bool DebugLight(const std::vector<std::string>& );

    glm::mat4 projection;
    glm::mat4 lightMatrix;
    const Camera* camera;
   
    FrameBuffer fbo;
    GBuffer gbuffer;
    ShadowBuffer shadowbuffer;
    PostProcessBuffer postprocessbuffer;
    //SkyBox skybox;

    Geometry* quad = Engine::geometryManager.Get("data/quad.obj");
    // PostProcessing programs
    ShaderProgram* toneMapPlusBrightness = Engine::programManager.Get("data/shaders/postprocess.json");
    ShaderProgram* textureView = Engine::programManager.Get("data/shaders/quad.json");
    ShaderProgram* gaussProgram = Engine::programManager.Get("data/shaders/gaussian_blur.json");
    ShaderProgram* bloomProgram = Engine::programManager.Get("data/shaders/bloom.json");

    unsigned int current_view_buffer;
    bool light_debug = false;

    static int width;
    static int height;
}; 

#endif /* End of __RENDERER__ */