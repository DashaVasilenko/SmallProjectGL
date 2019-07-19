#ifndef __BUFFERS__
#define __BUFFERS__

#include <GL/glew.h>
#include <string>
#include <vector>

class IndexBuffer {
public:
    IndexBuffer();
    void BufferData(int* data, size_t count);
    void Bind() const;
    void Unbind() const;
    ~IndexBuffer();
private:
    GLuint descriptor;
};

class VertexBuffer {
public:
    VertexBuffer();
    void BufferData(void* data, size_t size);
    void Bind() const;
    void Unbind() const;
    ~VertexBuffer();
private:
    GLuint descriptor;
};

enum ShaderDataType { None = 0, Float2, Float3, Float4, Int2, Int3, Int4 };

struct BufferElement {
    std::string name;
    ShaderDataType type;
    size_t size;
    size_t offset;

    BufferElement (const std::string& t_name, ShaderDataType t_type): 
        name(t_name), type(t_type), size(GetSizeOfType(type)), offset(0) {}

    size_t GetCount() const;
    GLenum GetOpenglType() const;

    static size_t GetSizeOfType(ShaderDataType type);
};

class BufferLayout {
public:
    std::vector<BufferElement> elements;
    BufferLayout(const std::initializer_list<BufferElement>& t_elements): elements(t_elements) {
        CalculateStrideAndOffset();
    }
    void CalculateStrideAndOffset();
    size_t GetStride() const { return stride; }

private:
    size_t stride = 0; // какой-то шаг step
};

class VertexArray {
public:
    VertexArray();
    void Bind() const;
    void AddAttributes(const VertexBuffer&, const BufferLayout&);
    ~VertexArray();
private:
    GLuint descriptor;
    GLuint freeAttribNum = 0;
};

class FrameBuffer {
public:
    FrameBuffer();
    //void SetWidth(int width) { this->width = width; } // пока не используется
    //void SetHeight(int height) { this->height = height; } // пока не используется
    void BufferInit(int width, int  height);
    void Bind() const;
    void Unbind() const;
    ~FrameBuffer();
private:
    GLuint descriptor;
    //int width = 640; // пока не используется
    //int height = 480; // пока не используется
};

#endif /* End of __BUFFERS__ */