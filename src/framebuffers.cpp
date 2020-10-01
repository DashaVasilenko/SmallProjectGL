#include "framebuffers.h"
#include <iostream>

//--------------------------------------------------------------------------
FrameBuffer::FrameBuffer() {
    GLCall(glGenFramebuffers(1, &descriptor)); 
}

void FrameBuffer::BufferInit(int width, int  height) {
    Bind();

    tex_color_buf.Init(width, height, GL_RGB, GL_RGB, GL_UNSIGNED_BYTE, GL_LINEAR);
    tex_color_buf.CreateAttachment(GL_COLOR_ATTACHMENT0);
    
    GLCall(glGenRenderbuffers(1, &rbo));
    GLCall(glBindRenderbuffer(GL_RENDERBUFFER, rbo)); 
    GLCall(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height));  
    GLCall(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo));

    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	    std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
    Unbind(); 
}

void FrameBuffer::Bind() const {
    GLCall(glBindFramebuffer(GL_FRAMEBUFFER, descriptor)); 
}

void FrameBuffer::Unbind() const {
    GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
}

FrameBuffer::~FrameBuffer() {
    GLCall(glDeleteFramebuffers(1, &descriptor));
}

//--------------------------------------------------------------------------------------------------------
void GBuffer::BufferInit(int width, int  height) {
    GLCall(glBindFramebuffer(GL_FRAMEBUFFER, descriptor));

    position.Init(width, height, GL_RGB16F, GL_RGB, GL_FLOAT, GL_NEAREST);
    position.CreateAttachment(GL_COLOR_ATTACHMENT0);

    normal.Init(width, height, GL_RGB16F, GL_RGB, GL_FLOAT, GL_NEAREST);
    normal.CreateAttachment(GL_COLOR_ATTACHMENT1);

    albedo.Init(width, height, GL_RGBA, GL_RGBA, GL_UNSIGNED_BYTE, GL_LINEAR);
    albedo.CreateAttachment(GL_COLOR_ATTACHMENT2);

    metallRoughAO.Init(width, height, GL_RGB, GL_RGB, GL_UNSIGNED_BYTE, GL_LINEAR);
    metallRoughAO.CreateAttachment(GL_COLOR_ATTACHMENT3);

    result.Init(width, height, GL_RGBA16F, GL_RGBA, GL_UNSIGNED_BYTE, GL_NEAREST);
    result.CreateAttachment(GL_COLOR_ATTACHMENT4);
    
    GLCall(glGenRenderbuffers(1, &rbo));
    GLCall(glBindRenderbuffer(GL_RENDERBUFFER, rbo)); 
    GLCall(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height));  
    GLCall(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo));

    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	    std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
    Unbind();  // отвязываем объект буфера кадра, чтобы случайно не начать рендер не туда, куда предполагалось
}

void GBuffer::StartFrame() const {
    GLCall(glBindFramebuffer(GL_DRAW_FRAMEBUFFER, descriptor));
    GLCall(glDrawBuffer(GL_COLOR_ATTACHMENT4));
    GLCall(glClear(GL_COLOR_BUFFER_BIT));
}

//------------------------------------------------------------------------------------------------
void ShadowBuffer::BufferInit(int width, int  height) {
    Bind();

    depthMap.InitDepthMap(size, size);
    depthMap.CreateAttachment(GL_DEPTH_ATTACHMENT);
    
    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	    std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
    Unbind();  
}

//--------------------------------------------------------------------------------------------------------------------------
void PostProcessBuffer::BufferInit(int width, int height) {
    Bind();
    
    hdrMap.Init(width, height, GL_RGBA, GL_RGBA, GL_UNSIGNED_BYTE, GL_NEAREST);
    hdrMap.CreateAttachment(GL_COLOR_ATTACHMENT0);

    brightMap.InitBrightMapGauss(width, height);
    brightMap.CreateAttachment(GL_COLOR_ATTACHMENT1);
 
    horizontalGauss.InitBrightMapGauss(width, height);
    horizontalGauss.CreateAttachment(GL_COLOR_ATTACHMENT2);

    verticalGauss.InitBrightMapGauss(width, height);
    verticalGauss.CreateAttachment(GL_COLOR_ATTACHMENT3);

    bloom.Init(width, height, GL_RGBA, GL_RGBA, GL_UNSIGNED_BYTE, GL_NEAREST);
    bloom.CreateAttachment(GL_COLOR_ATTACHMENT4);

    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	    std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
    Unbind();
}