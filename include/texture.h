#ifndef __TEXTURE__
#define __TEXTURE__

#include <iostream>
#include <string>
#include <array>
#include <GL/glew.h>
#include "stb_image.h"
#include "errors.h"

class Texture {
public:
    Texture();
    void Bind() const;
    GLuint GetDescriptor() { return descriptor; }
    void Unbind() const;
    ~Texture();

protected:
    GLuint descriptor;
    unsigned char* image;
    int width;
    int height;
    int cnt;
};


class Texture2D : public Texture { // потом сделать наследование от Texture
public:
    void BindSlot(GLenum slot) const;
    void Load(const std::string& filename);
    void Init();
};

class CubeMap : public Texture{ // потом сделать наследование от Texture
public:
    void Init(const std::array<std::string, 6>& fileNames);

private:
    int nrChannels;
};

class RenderTexture : public Texture {
public:
    void BindSlot(GLenum slot) const;
    void Init(int width, int height, GLuint internalformat, GLenum format, GLenum type, GLint param);
    void InitDepthMap(int width, int height);
    void InitBrightMapGauss(int width, int height);
};

#endif /* End of __TEXTURE__ */