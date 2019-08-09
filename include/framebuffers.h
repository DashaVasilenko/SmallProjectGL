#ifndef __FRAMEBUFFERS__
#define __FRAMEBUFFERS__

#include <GL/glew.h>

class GBuffer {
public:
    GBuffer();
    GLuint GetDescriptor() { return descriptor; }
    void BufferInit(int width, int  height);
    void StartFrame() const;
    void Bind() const;
    void GeometryPassBind() const;
    void StencilPassBind() const;
    void LightPassBind() const;
    void FinalPassBind() const;
    void Unbind() const;

    int GetPositionDescriptor() { return this->position; }
    int GetNormalDescriptor() { return this->normal; }
    int GetAlbedoDescriptor() { return this->albedo; }
    int GetMetallRoughAODescriptor() { return this->metallRoughAO; }

    ~GBuffer();
private:
    GLuint descriptor;
    unsigned int position;
    unsigned int normal; 
    unsigned int albedo;
    unsigned int metallRoughAO;
    unsigned int result;
    unsigned int rbo;
};

class ShadowBuffer {
public:
    ShadowBuffer();
    GLuint GetDescriptor() { return descriptor; }
    void BufferInit(int width, int  height);
    void Bind();
    void BindDepth();
    void Unbind() const;
    void LightPassBind() const;

    unsigned int GetSize() { return size; }

    ~ShadowBuffer();
private:
    GLuint descriptor;
    unsigned int depthMap;
    unsigned int size = 1024; // размер мапы
};

#endif /* End of __FRAMEBUFFERS__ */