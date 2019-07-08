#ifndef __TEXTURE__
#define __TEXTURE__

#include <string>
#include <GL/glew.h>
#include "stb_image.h"


class Texture {
public:
    Texture();
    void Bind() const;
    void Load(const std::string& filename);
    void Init();
    void Unbind() const;

private:
    GLuint descriptor;
    unsigned char* image;
    int width;
    int height;
    int cnt;
};

#endif /* End of __TEXTURE__ */