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

/*
class Texture2D : public Texture { // потом сделать наследование от Texture
public:
private:
};
*/

class CubeMap { // потом сделать наследование от Texture
public:
    CubeMap();
    void Bind() const;
    //void Load(const std::string& filename);
    void Init(const std::array<std::string, 6>& fileNames);
    ~CubeMap();

private:
    GLuint descriptor;
    unsigned char* image;
    int width;
    int height;
    int nrChannels;
};

class RenderTexture : public Texture {
public:
    RenderTexture();
    void Bind() const;
    void Bind(GLenum slot) const;
    void Init(int width, int height, GLuint internalformat, GLenum format, GLenum type, GLint param);
    void InitDepthMap(int width, int height);
    void InitBrightMapGauss(int width, int height);
    GLuint GetDescriptor() { return descriptor; }

private:
    GLuint descriptor;
    unsigned char* image;
    int width;
    int height;
    int cnt;
};

#endif /* End of __TEXTURE__ */