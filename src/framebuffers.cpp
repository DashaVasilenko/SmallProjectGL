#include "framebuffers.h"
#include <iostream>

GBuffer::GBuffer() {
    glGenFramebuffers(1, &descriptor); // создаем gBuffer, 1 - кол-во буферов
}

// дописать эту функцию 
void GBuffer::BufferInit(int width, int  height) {
    glBindFramebuffer(GL_FRAMEBUFFER, descriptor);
    //unsigned int gPosition, gNormal, gColorSpec;

    // буфер позиций
    glGenTextures(1, &position);
    glBindTexture(GL_TEXTURE_2D, position);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    // присоединение текстуры к объекту текущего кадрового буфера
    // (тип объекта кадра, тип прикрепления, тип текстуры, объект текстуры, используемый для вывода МИП-уровень)
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, position, 0);

    // буфер нормалей
    glGenTextures(1, &normal);
    glBindTexture(GL_TEXTURE_2D, normal);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, normal, 0);

    // буфер для цвета + коэффициента зеркального отражения
    glGenTextures(1, &albedo);
    glBindTexture(GL_TEXTURE_2D, albedo);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, albedo, 0);

    // прочая шняга
    glGenTextures(1, &metallRoughAO);
    glBindTexture(GL_TEXTURE_2D, metallRoughAO);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT3, GL_TEXTURE_2D, metallRoughAO, 0);

    glGenTextures(1, &result);
    glBindTexture(GL_TEXTURE_2D, result);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT4, GL_TEXTURE_2D, result, 0);
    
    // создание объекта рендербуфера для совмещенных буфера глубины и трафарета

    glGenRenderbuffers(1, &rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo); 
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);  
    // присоединяем объект рендербуфера к совмещенной точке прикрепления глубины и трафарета буфера кадра
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

    //проверяем текущий привязанный кадровый буфер на завершенность
    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	    std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
    Unbind();  // отвязываем объект буфера кадра, чтобы случайно не начать рендер не туда, куда предполагалось
}

void GBuffer::Bind() const {
    glBindFramebuffer(GL_FRAMEBUFFER, descriptor);
}

void GBuffer::GeometryPassBind() const {
    glBindFramebuffer(GL_FRAMEBUFFER, descriptor);  // привязываем как текущий активный буфер кадра 
    unsigned int attachments[4] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 };
    glDrawBuffers(4, attachments);
}

void GBuffer::StencilPassBind() const {
    glDrawBuffer(GL_NONE);
}

void GBuffer::LightPassBind() const {
    //glBindBuffer(GL_FRAMEBUFFER, descriptor);
    glDrawBuffer(GL_COLOR_ATTACHMENT4);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, position);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, normal);

    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, albedo);

    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D, metallRoughAO);
    
}

void GBuffer::FinalPassBind() const {
    glDrawBuffer(GL_COLOR_ATTACHMENT4);
    glBindFramebuffer(GL_READ_FRAMEBUFFER, descriptor);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
    glReadBuffer(GL_COLOR_ATTACHMENT4);
}

void GBuffer::StartFrame() const {
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, descriptor);
    glDrawBuffer(GL_COLOR_ATTACHMENT4);
    glClear(GL_COLOR_BUFFER_BIT);
}

void GBuffer::Unbind() const {
    glBindFramebuffer(GL_FRAMEBUFFER, 0); // отвязываем буфера и возвращаем базовый кадровый буфер на место активного  
}

GBuffer::~GBuffer() {
    glDeleteFramebuffers(1, &descriptor);  // удаляем ненужный буфер
}


//------------------------------------------------------------------------------------------------
ShadowBuffer::ShadowBuffer() {
    glGenFramebuffers(1, &descriptor); // создаем ShadowBuffer, 1 - кол-во буферов
}

// дописать эту функцию 
void ShadowBuffer::BufferInit(int width, int  height) {
    Bind();
    //unsigned int gPosition, gNormal, gColorSpec;

    // буфер позиций
    glGenTextures(1, &depthMap);
    glBindTexture(GL_TEXTURE_2D, depthMap);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, size, size, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // присоединение текстуры к объекту текущего кадрового буфера
    // (тип объекта кадра, тип прикрепления, тип текстуры, объект текстуры, используемый для вывода МИП-уровень)
    //Bind();
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
    // указываем, что не будем рендерить цвет, так как нас интересует только глубины
    glDrawBuffer(GL_NONE);  
    glReadBuffer(GL_NONE);

    //проверяем текущий привязанный кадровый буфер на завершенность
    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	    std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
    Unbind();  // отвязываем объект буфера кадра, чтобы случайно не начать рендер не туда, куда предполагалось
}

void ShadowBuffer::Bind() {
    glBindFramebuffer(GL_FRAMEBUFFER, descriptor);
}

void ShadowBuffer::BindDepth() {
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, depthMap);
}

void ShadowBuffer::Unbind() const {
    glBindFramebuffer(GL_FRAMEBUFFER, 0); // отвязываем буфера и возвращаем базовый кадровый буфер на место активного  
}

void ShadowBuffer::LightPassBind() const {
    glActiveTexture(GL_TEXTURE4);
    glBindTexture(GL_TEXTURE_2D, depthMap);
}

ShadowBuffer::~ShadowBuffer() {
    glDeleteFramebuffers(1, &descriptor);  // удаляем ненужный буфер
}