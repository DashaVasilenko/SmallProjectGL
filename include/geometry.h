#ifndef __GEOMETRY__
#define __GEOMETRY__

#include "buffers.h"
#include "errors.h"

#include <string>

class Geometry {
public:
    void Load(const std::string& filename);
    void Draw() const;

private:
    VertexBuffer vbo;
    VertexArray vao;
    //BufferLayout layout;
    unsigned int count = 0;

    void Init(const std::vector<GLfloat>&, const BufferLayout&); 
};

#endif /* End of __GEOMETRY__ */