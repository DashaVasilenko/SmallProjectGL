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