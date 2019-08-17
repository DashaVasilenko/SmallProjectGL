#ifndef __FRAMEBUFFERS__
#define __FRAMEBUFFERS__

#include <GL/glew.h>

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
    unsigned int position;
    unsigned int normal; 
    unsigned int albedo;
    unsigned int metallRoughAO;
    unsigned int result;
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
    unsigned int depthMap;
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
    unsigned int hdrMap;
    unsigned int brightMap;
    unsigned int horizontalGauss;
    unsigned int verticalGauss;
    unsigned int bloom;
    unsigned int depthMap;
};

#endif /* End of __FRAMEBUFFERS__ */