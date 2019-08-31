#define STB_IMAGE_IMPLEMENTATION
#include "texture.h"

Texture::Texture() {
    GLCall(glGenTextures(1, &descriptor)); // 1 - количество текстур для генерации
}

void Texture::Bind() const {
    GLCall(glBindTexture(GL_TEXTURE_2D, descriptor)); // привязка текстуры
}

void Texture::Unbind() const { 
    GLCall(glBindTexture(GL_TEXTURE_2D, 0)); // отвязка объекта текстуры
}

Texture::~Texture() {
    GLCall(glDeleteTextures(1, &descriptor));
}

//-----------------------------------------------------------------------------------------------------------
void Texture2D::CreateAttachment(GLenum slot) const {
    GLCall(glActiveTexture(slot));
    GLCall(glBindTexture(GL_TEXTURE_2D, descriptor)); // привязка текстуры
}

void Texture2D::Load(const std::string& filename) {
    // (путь, ширина, высота, количество каналов при загрузке изображения, количество каналов для отображения)
	// каналы STBI_grey = 1, STBI_grey_alpha = 2, STBI_rgb = 3, STBI_rgb_alpha = 4
	image = stbi_load(filename.c_str(), &width, &height, &cnt, 3); // загружаем текстуру
    Init();
}

void Texture2D::Init() {
    Bind();
    // (текстурная цель, уровень мипмапа, формат текстуры, ширина, высота, 0, формат исходного изображения,
	//  тип данных исходного изображения, данные изображения)
	GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image)); // генерируем текстуру
	GLCall(glGenerateMipmap(GL_TEXTURE_2D)); // генерация всех необходимых мипмапов для текстуры
	stbi_image_free(image); // освобождения памяти
    Unbind(); // отвязка объекта текстуры
}

//-----------------------------------------------------------------------------------------------------------
void CubeMap::Init(const std::array<std::string, 6>& fileNames) {
    Bind();
    for(unsigned int i = 0; i < fileNames.size(); i++)
    {
        image = stbi_load(fileNames[i].c_str(), &width, &height, &nrChannels, 0);
        if (image) {
            GLCall(glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image));
        }
        else {
            std::cout << "Cubemap texture failed to load at path: " << fileNames[i] << std::endl;
            stbi_image_free(image);
        }
    }

    GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
    GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
    GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
    GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
    GLCall(glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE)); 
}

//------------------------------------------------------------------------------------------------------
void RenderTexture:: CreateAttachment(GLenum slot) const {
    // присоединение текстуры к объекту текущего кадрового буфера
    // (тип объекта кадра, тип прикрепления, тип текстуры, объект текстуры, используемый для вывода МИП-уровень)
    GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, slot, GL_TEXTURE_2D, descriptor, 0)); 
}

void RenderTexture::Init(int width, int height, GLuint internalformat, GLenum format, GLenum type, GLint param) {
    Bind();
    this->width = width;
    this->height = height;
    GLCall(glTexImage2D(GL_TEXTURE_2D, 0, internalformat, width, height, 0, format, type, NULL));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, param));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, param));
}

void RenderTexture::InitDepthMap(int width, int height) {
    Bind();
    this->width = width;
    this->height = height;
    GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER));
    float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f }; 
    GLCall(glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor)); // задаем цвет границы
}

void RenderTexture::InitBrightMapGauss(int width, int height) {
    Bind();
    this->width = width;
    this->height = height;
    GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
}