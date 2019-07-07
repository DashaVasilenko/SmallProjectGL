#ifndef __TEXTURE__
#define __TEXTURE__

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <string>
#include <GL/glew.h>

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