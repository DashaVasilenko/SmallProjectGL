#ifndef __FRAMEBUFFERS__
#define __FRAMEBUFFERS__

#include <GL/glew.h>
#include "errors.h"
#include "texture.h"

class FrameBuffer {
public:
    FrameBuffer();
    void BufferInit(int width, int  height);
    void Bind() const;
    void Unbind() const;
    GLuint GetDescriptor() { return descriptor; }
    GLuint GetTexDescriptor() { return tex_color_buf.GetDescriptor(); }
    ~FrameBuffer();
    
protected:
    GLuint descriptor;
    
private:
    RenderTexture tex_color_buf;
    unsigned int rbo;
};

class GBuffer : public FrameBuffer{
public:
    friend class Renderer;
    void BufferInit(int width, int  height);
    void StartFrame() const;

private:
    RenderTexture position; 
    RenderTexture normal;
    RenderTexture albedo;
    RenderTexture metallRoughAO;
    RenderTexture result;
    unsigned int rbo;
};

class ShadowBuffer : public FrameBuffer {
public:
    friend class Renderer;
    void BufferInit(int width, int  height);
    unsigned int GetSize() { return size; }

private:
    RenderTexture depthMap;
    unsigned int size = 4096; // размер мапы
};

class PostProcessBuffer : public FrameBuffer {
public:
    friend class Renderer;
    void BufferInit(int width, int  height);
    
private:
    RenderTexture hdrMap;
    RenderTexture brightMap;
    RenderTexture horizontalGauss;
    RenderTexture verticalGauss;
    RenderTexture bloom;
    //unsigned int depthMap;
    RenderTexture depthMap; // чет оно нигде не используется. я не нашла где
};

#endif /* End of __FRAMEBUFFERS__ */