#ifndef __TEXTURE__
#define __TEXTURE__

#include <string>
#include <GL/glew.h>
#include "stb_image.h"
#include "errors.h"


class Texture {
public:
    Texture();
    void Bind() const;
    void Bind(GLenum slot) const;
    void Load(const std::string& filename);

    void Unbind() const;
    ~Texture();

private:
    GLuint descriptor;
    unsigned char* image;
    int width;
    int height;
    int cnt;
    void Init();
};

#endif /* End of __TEXTURE__ */