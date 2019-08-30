#include "framebuffers.h"
#include <iostream>

GBuffer::GBuffer() {
    GLCall(glGenFramebuffers(1, &descriptor)); // создаем gBuffer, 1 - кол-во буферов
}

// дописать эту функцию 
void GBuffer::BufferInit(int width, int  height) {
    GLCall(glBindFramebuffer(GL_FRAMEBUFFER, descriptor));

    // буфер позиций
    position.Bind();
    position.Init(width, height, GL_RGB16F, GL_RGB, GL_FLOAT, GL_NEAREST);
    position.BindSlot(GL_COLOR_ATTACHMENT0);

    // буфер нормалей
    normal.Bind();
    normal.Init(width, height, GL_RGB16F, GL_RGB, GL_FLOAT, GL_NEAREST);
    normal.BindSlot(GL_COLOR_ATTACHMENT1);

    // буфер для цвета + коэффициента зеркального отражения
    albedo.Bind();
    albedo.Init(width, height, GL_RGBA, GL_RGBA, GL_UNSIGNED_BYTE, GL_LINEAR);
    albedo.BindSlot(GL_COLOR_ATTACHMENT2);

    // прочая шняга
    metallRoughAO.Bind();
    metallRoughAO.Init(width, height, GL_RGB, GL_RGB, GL_UNSIGNED_BYTE, GL_LINEAR);
    metallRoughAO.BindSlot(GL_COLOR_ATTACHMENT3);

    result.Bind();
    result.Init(width, height, GL_RGBA16F, GL_RGBA, GL_UNSIGNED_BYTE, GL_NEAREST);
    result.BindSlot(GL_COLOR_ATTACHMENT4);
    
    // создание объекта рендербуфера для совмещенных буфера глубины и трафарета
    GLCall(glGenRenderbuffers(1, &rbo));
    GLCall(glBindRenderbuffer(GL_RENDERBUFFER, rbo)); 
    GLCall(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height));  
    // присоединяем объект рендербуфера к совмещенной точке прикрепления глубины и трафарета буфера кадра
    GLCall(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo));

    //проверяем текущий привязанный кадровый буфер на завершенность
    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	    std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
    Unbind();  // отвязываем объект буфера кадра, чтобы случайно не начать рендер не туда, куда предполагалось
}

void GBuffer::Bind() const {
    GLCall(glBindFramebuffer(GL_FRAMEBUFFER, descriptor));
}

void GBuffer::StartFrame() const {
    GLCall(glBindFramebuffer(GL_DRAW_FRAMEBUFFER, descriptor));
    GLCall(glDrawBuffer(GL_COLOR_ATTACHMENT4));
    GLCall(glClear(GL_COLOR_BUFFER_BIT));
}

void GBuffer::Unbind() const {
    GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0)); // отвязываем буфера и возвращаем базовый кадровый буфер на место активного  
}

GBuffer::~GBuffer() {
    GLCall(glDeleteFramebuffers(1, &descriptor));  // удаляем ненужный буфер
}


//------------------------------------------------------------------------------------------------
ShadowBuffer::ShadowBuffer() {
    GLCall(glGenFramebuffers(1, &descriptor)); // создаем ShadowBuffer, 1 - кол-во буферов
}

// дописать эту функцию 
void ShadowBuffer::BufferInit(int width, int  height) {
    Bind();

    // карта глубины
    depthMap.Bind();
    depthMap.InitDepthMap(size, size);
    depthMap.BindSlot(GL_DEPTH_ATTACHMENT);
    
    // указываем, что не будем рендерить цвет, так как нас интересует только глубины
    // glDrawBuffer(GL_NONE);  
    // glReadBuffer(GL_NONE);

    //проверяем текущий привязанный кадровый буфер на завершенность
    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	    std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
    Unbind();  
}

void ShadowBuffer::Bind() {
    GLCall(glBindFramebuffer(GL_FRAMEBUFFER, descriptor));
}

void ShadowBuffer::Unbind() const {
    GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0)); // отвязываем буфера и возвращаем базовый кадровый буфер на место активного  
}

ShadowBuffer::~ShadowBuffer() {
    GLCall(glDeleteFramebuffers(1, &descriptor));  // удаляем ненужный буфер
}

//--------------------------------------------------------------------------------------------------------------------------
PostProcessBuffer::PostProcessBuffer() {
    GLCall(glGenFramebuffers(1, &descriptor)); 
}

void PostProcessBuffer::BufferInit(int width, int height) {
    Bind();
    
    hdrMap.Bind();
    hdrMap.Init(width, height, GL_RGBA, GL_RGBA, GL_UNSIGNED_BYTE, GL_NEAREST);
    hdrMap.BindSlot(GL_COLOR_ATTACHMENT0);

    brightMap.Bind();
    brightMap.InitBrightMapGauss(width, height);
    brightMap.BindSlot(GL_COLOR_ATTACHMENT1);
 
    horizontalGauss.Bind();
    horizontalGauss.InitBrightMapGauss(width, height);
    horizontalGauss.BindSlot(GL_COLOR_ATTACHMENT2);

    verticalGauss.Bind();
    verticalGauss.InitBrightMapGauss(width, height);
    verticalGauss.BindSlot(GL_COLOR_ATTACHMENT3);

    bloom.Bind();
    bloom.Init(width, height, GL_RGBA, GL_RGBA, GL_UNSIGNED_BYTE, GL_NEAREST);
    bloom.BindSlot(GL_COLOR_ATTACHMENT4);

    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	    std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
    Unbind();  // отвязываем объект буфера кадра, чтобы случайно не начать рендер не туда, куда предполагалось
}

void PostProcessBuffer::Bind() const {
    GLCall(glBindFramebuffer(GL_FRAMEBUFFER, descriptor));
}

void PostProcessBuffer::Unbind() const {
    GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
}

PostProcessBuffer::~PostProcessBuffer() {
    GLCall(glDeleteFramebuffers(1, &descriptor));
}