#ifndef __GEOMETRY__
#define __GEOMETRY__

#include "buffers.h"

#include <string>

class Geometry {
public:
    void Load(const std::string& filename);
    void Draw();
private:
    void Init(const std::vector<GLfloat>&, const BufferLayout&); 

    VertexBuffer vbo;
    VertexArray vao;
    //BufferLayout layout;
    unsigned int count = 0;
};


#endif