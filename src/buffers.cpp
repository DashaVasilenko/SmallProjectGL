#include "buffers.h"
#include <iostream>

IndexBuffer::IndexBuffer() {
    glGenBuffers(1, &descriptor); // создаем буфер IBO (1 - кол-во буферов)
}

void IndexBuffer::BufferData(int* data, size_t count) {
    Bind();
    // копируем вершинные данные в буфер (тип буфера, количество данных в байтах, данные, режим работы с данными)
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, count*sizeof(int), data, GL_STATIC_DRAW);
}

void IndexBuffer::Bind() const {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, descriptor); // определяем тип буфера
}

// !!! где-то надо отвязать (не знаю, где)
void IndexBuffer::Unbind() const {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

IndexBuffer::~IndexBuffer() {
    glDeleteBuffers(1, &descriptor);
}

//--------------------------------------------------------------------------
VertexBuffer::VertexBuffer() {
    glGenBuffers(1, &descriptor); // создаем буфер VBO (vertex buffer objects) 1 - кол-во буферов
}

void VertexBuffer::BufferData(void* data, size_t size) {
    Bind();
    // копируем вершинные данные в буфер (тип буфера, количество данных в байтах, данные, режим работы с данными)
    glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
}

void VertexBuffer::Bind() const {
    glBindBuffer(GL_ARRAY_BUFFER, descriptor); // определяем тип буфера
}

// !!! где-то надо отвязать (не знаю, где)
void VertexBuffer::Unbind() const {
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

VertexBuffer::~VertexBuffer() {
    glDeleteBuffers(1, &descriptor);
}

//--------------------------------------------------------------------------
size_t BufferElement::GetCount() const {
    switch(type) {
        case Float2: return 2;
        case Float3: return 3;
        case Float4: return 4;

        case Int2: return 2;
        case Int3: return 3;
        case Int4: return 4;

        default:
                // switch to assert
            exit(EXIT_FAILURE);
    }
}

GLenum BufferElement::GetOpenglType() const {
    switch(type) {
        case Float2: return GL_FLOAT;
        case Float3: return GL_FLOAT;
        case Float4: return GL_FLOAT;

        case Int2: return GL_INT;
        case Int3: return GL_INT;
        case Int4: return GL_INT;

        default:
            // switch to assert
            exit(EXIT_FAILURE);
    }
}

size_t BufferElement::GetSizeOfType(ShaderDataType type) {
    switch(type) {
        case Float2: return sizeof(GLfloat)*2;
        case Float3: return sizeof(GLfloat)*3;
        case Float4: return sizeof(GLfloat)*4;

        case Int2: return sizeof(GLint)*2;
        case Int3: return sizeof(GLint)*3;
        case Int4: return sizeof(GLint)*4;

        default:
            // switch to assert
            exit(EXIT_FAILURE);
    }
}
//--------------------------------------------------------------------------

void BufferLayout::CalculateStrideAndOffset() {
    size_t current_offset = 0;
    for (auto& bufferElement: elements) {
        bufferElement.offset = current_offset;
        current_offset += bufferElement.size;

        stride += bufferElement.size;
    }
}

//--------------------------------------------------------------------------
VertexArray::VertexArray() {
    glGenVertexArrays(1, &descriptor);
}

void VertexArray::AddAttributes(const VertexBuffer& vertexBuffer, const BufferLayout& layout) {

    vertexBuffer.Bind();
    Bind();

    for (const auto& bufferElement: layout.elements) {
        // сообщаем OpenGL как он должен интерпретировать вершинные данные
	    // (какой аргумент шейдера мы хотим настроить(layout (location = 0)), размер аргумента в шейдере, тип данных,
	    //  необходимость нормализовать входные данные, расстояние между наборами данных, смещение начала данных в буфере)
        glVertexAttribPointer(
                                freeAttribNum, bufferElement.GetCount(), 
                                bufferElement.GetOpenglType(), GL_FALSE, 
                                layout.GetStride(), (GLvoid*)bufferElement.offset
                             );
	    glEnableVertexAttribArray(freeAttribNum); // включаем атрибуты, т.е. передаем вершинному атрибуту позицию аргумента
        freeAttribNum++;
    }
}

void VertexArray::Bind() const {
    glBindVertexArray(descriptor);
}

VertexArray::~VertexArray() {
    glDeleteVertexArrays(1, &descriptor);
}

//--------------------------------------------------------------------------
FrameBuffer::FrameBuffer() {
    glGenFramebuffers(1, &descriptor); // создаем буфер FBO (frame buffer objects) 1 - кол-во буферов
}

// дописать эту функцию 
void FrameBuffer::BufferInit(int width, int  height) {
    Bind();

    // используем текстурные прикрепления для создания объектра буфера цвета
    unsigned int tex_color_buf;
    glGenTextures(1, &tex_color_buf);
    glBindTexture(GL_TEXTURE_2D, tex_color_buf);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0); // как только мы запросили память для объекта, можно его отвязывать.

    // присоединение текстуры к объекту текущего кадрового буфера
    // (тип объекта кадра, тип прикрепления, тип текстуры, объект текстуры, используемый для вывода МИП-уровень)
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tex_color_buf, 0); 
    
    // создание объекта рендербуфера для совмещенных буфера глубины и трафарета
    unsigned int rbo;
    glGenRenderbuffers(1, &rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, rbo); 
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);  
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
    // присоединяем объект рендербуфера к совмещенной точке прикрепления глубины и трафарета буфера кадра
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

    //проверяем текущий привязанный кадровый буфер на завершенность
    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	    std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
    Unbind();  // отвязываем объект буфера кадра, чтобы случайно не начать рендер не туда, куда предполагалось
}

void FrameBuffer::Bind() const {
    glBindFramebuffer(GL_FRAMEBUFFER, descriptor);  // привязываем как текущий активный буфер кадра 
}

// !!! где-то надо отвязать (не знаю, где)
void FrameBuffer::Unbind() const {
    glBindFramebuffer(GL_FRAMEBUFFER, 0); // отвязываем буфера и возвращаем базовый кадровый буфер на место активного  
}

FrameBuffer::~FrameBuffer() {
    glDeleteFramebuffers(1, &descriptor);  // удаляем ненужный буфер
}
