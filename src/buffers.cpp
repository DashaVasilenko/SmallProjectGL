#include "buffers.h"
#include <iostream>

IndexBuffer::IndexBuffer() {
    GLCall(glGenBuffers(1, &descriptor));
}

void IndexBuffer::BufferData(int* data, size_t count) {
    Bind();
    GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count*sizeof(int), data, GL_STATIC_DRAW));
}

void IndexBuffer::Bind() const {
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, descriptor));
}

void IndexBuffer::Unbind() const {
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}

IndexBuffer::~IndexBuffer() {
    GLCall(glDeleteBuffers(1, &descriptor));
}

//--------------------------------------------------------------------------
VertexBuffer::VertexBuffer() {
    GLCall(glGenBuffers(1, &descriptor));
}

void VertexBuffer::BufferData(void* data, size_t size) {
    Bind();
    GLCall(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
}

void VertexBuffer::Bind() const {
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, descriptor)); 
}

void VertexBuffer::Unbind() const {
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
}

VertexBuffer::~VertexBuffer() {
    GLCall(glDeleteBuffers(1, &descriptor));
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

        default: exit(EXIT_FAILURE); // switch to assert
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

        default: exit(EXIT_FAILURE); // switch to assert
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

        default: exit(EXIT_FAILURE); // switch to assert
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
    GLCall(glGenVertexArrays(1, &descriptor));
}

void VertexArray::AddAttributes(const VertexBuffer& vertexBuffer, const BufferLayout& layout) {
    
    vertexBuffer.Bind();
    Bind();

    for (const auto& bufferElement: layout.elements) {
        GLCall(glVertexAttribPointer(
                                freeAttribNum, bufferElement.GetCount(), 
                                bufferElement.GetOpenglType(), GL_FALSE, 
                                layout.GetStride(), (GLvoid*)bufferElement.offset
                             ));
	    GLCall(glEnableVertexAttribArray(freeAttribNum));
        freeAttribNum++;
    }
}

void VertexArray::Bind() const {
    GLCall(glBindVertexArray(descriptor));
}

VertexArray::~VertexArray() {
    GLCall(glDeleteVertexArrays(1, &descriptor));
}