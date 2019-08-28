#include "framebuffers.h"
#include <iostream>

GBuffer::GBuffer() {
    GLCall(glGenFramebuffers(1, &descriptor)); // создаем gBuffer, 1 - кол-во буферов
}

// дописать эту функцию 
void GBuffer::BufferInit(int width, int  height) {
    GLCall(glBindFramebuffer(GL_FRAMEBUFFER, descriptor));
    //unsigned int gPosition, gNormal, gColorSpec;

    // буфер позиций
    GLCall(glGenTextures(1, &position));
    GLCall(glBindTexture(GL_TEXTURE_2D, position));
    GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, NULL));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
    // присоединение текстуры к объекту текущего кадрового буфера
    // (тип объекта кадра, тип прикрепления, тип текстуры, объект текстуры, используемый для вывода МИП-уровень)
    GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, position, 0));

    // буфер нормалей
    GLCall(glGenTextures(1, &normal));
    GLCall(GLCall(glBindTexture(GL_TEXTURE_2D, normal));
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, NULL));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
    GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, normal, 0));

    // буфер для цвета + коэффициента зеркального отражения
    GLCall(glGenTextures(1, &albedo));
    GLCall(glBindTexture(GL_TEXTURE_2D, albedo));
    GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
    GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, albedo, 0));

    // прочая шняга
    GLCall(glGenTextures(1, &metallRoughAO));
    GLCall(glBindTexture(GL_TEXTURE_2D, metallRoughAO));
    GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
    GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT3, GL_TEXTURE_2D, metallRoughAO, 0));

    GLCall(glGenTextures(1, &result));
    GLCall(glBindTexture(GL_TEXTURE_2D, result));
    GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
    GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT4, GL_TEXTURE_2D, result, 0));
    
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
    //unsigned int gPosition, gNormal, gColorSpec;

    // карта глубины
    GLCall(glGenTextures(1, &depthMap));
    GLCall(glBindTexture(GL_TEXTURE_2D, depthMap));
    GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, size, size, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER));
    float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f }; 
    GLCall(glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor)); // задаем цвет границы
    // присоединение текстуры к объекту текущего кадрового буфера
    // (тип объекта кадра, тип прикрепления, тип текстуры, объект текстуры, используемый для вывода МИП-уровень)
   GLCall( glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0));
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
    GLCall(glGenTextures(1, &hdrMap));
    GLCall(glBindTexture(GL_TEXTURE_2D, hdrMap));
    GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
    GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, hdrMap, 0));

    GLCall(glGenTextures(1, &brightMap));
    GLCall(glBindTexture(GL_TEXTURE_2D, brightMap));
    GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
    GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, brightMap, 0));
 
    GLCall(glGenTextures(1, &horizontalGauss));
    GLCall(glBindTexture(GL_TEXTURE_2D, horizontalGauss));
    GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
    GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, horizontalGauss, 0));

    GLCall(glGenTextures(1, &verticalGauss));
    GLCall(glBindTexture(GL_TEXTURE_2D, verticalGauss));
    GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
    GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT3, GL_TEXTURE_2D, verticalGauss, 0));

    GLCall(glGenTextures(1, &bloom));
    GLCall(glBindTexture(GL_TEXTURE_2D, bloom));
    GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
    GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT4, GL_TEXTURE_2D, bloom, 0));

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
