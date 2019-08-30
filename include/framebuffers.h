#ifndef __FRAMEBUFFERS__
#define __FRAMEBUFFERS__

#include <GL/glew.h>
#include "errors.h"
#include "texture.h"

class GBuffer {
public:
    friend class Renderer;
    GBuffer();
    GLuint GetDescriptor() { return descriptor; }
    void BufferInit(int width, int  height);
    void StartFrame() const;
    void Bind() const;
    void Unbind() const;

    ~GBuffer();

private:
    GLuint descriptor;
    RenderTexture position; 
    RenderTexture normal;
    RenderTexture albedo;
    RenderTexture metallRoughAO;
    RenderTexture result;
    unsigned int rbo;
};

class ShadowBuffer {
public:
    friend class Renderer;
    ShadowBuffer();
    void BufferInit(int width, int  height);
    void Bind();
    void Unbind() const;

    unsigned int GetSize() { return size; }

    ~ShadowBuffer();

private:
    GLuint descriptor;
    RenderTexture depthMap;
    unsigned int size = 4096; // размер мапы
};

class PostProcessBuffer {
public:
    friend class Renderer;
    PostProcessBuffer();
    void BufferInit(int width, int  height);
    void Bind() const;
    void Unbind() const;
    ~PostProcessBuffer();
    
private:
    GLuint descriptor;
    RenderTexture hdrMap;
    RenderTexture brightMap;
    RenderTexture horizontalGauss;
    RenderTexture verticalGauss;
    RenderTexture bloom;
    //unsigned int depthMap;
    RenderTexture depthMap; // чет оно нигде не используется. я не нашла где
};

#endif /* End of __FRAMEBUFFERS__ */