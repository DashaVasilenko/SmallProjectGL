#define STB_IMAGE_IMPLEMENTATION
#include "texture.h"

Texture::Texture() {
    GLCall(glGenTextures(1, &descriptor)); // 1 - количество текстур для генерации
}

void Texture::Bind() const {
    GLCall(glBindTexture(GL_TEXTURE_2D, descriptor)); // привязка текстуры
}

void Texture::Bind(GLenum slot) const {
    GLCall(glActiveTexture(slot));
    GLCall(glBindTexture(GL_TEXTURE_2D, descriptor)); // привязка текстуры
}

void Texture::Load(const std::string& filename) {
    // (путь, ширина, высота, количество каналов при загрузке изображения, количество каналов для отображения)
	// каналы STBI_grey = 1, STBI_grey_alpha = 2, STBI_rgb = 3, STBI_rgb_alpha = 4
	image = stbi_load(filename.c_str(), &width, &height, &cnt, 3); // загружаем текстуру
    Init();
}

void Texture::Init() {
    Bind();
    // (текстурная цель, уровень мипмапа, формат текстуры, ширина, высота, 0, формат исходного изображения,
	//  тип данных исходного изображения, данные изображения)
	GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image)); // генерируем текстуру
	GLCall(glGenerateMipmap(GL_TEXTURE_2D)); // генерация всех необходимых мипмапов для текстуры
	stbi_image_free(image); // освобождения памяти
    Unbind(); // отвязка объекта текстуры
}

void Texture::Unbind() const { 
    GLCall(glBindTexture(GL_TEXTURE_2D, 0)); // отвязка объекта текстуры
}

Texture::~Texture() {
    GLCall(glDeleteTextures(1, &descriptor));
}

//-----------------------------------------------------------------------------------------------------------
CubeMap::CubeMap() {
    GLCall(glGenTextures(1, &descriptor));
}

void CubeMap::Bind() const {
    GLCall(glBindTexture(GL_TEXTURE_2D, descriptor)); // привязка текстуры
}

/*
void CubeMap::Load(const std::string& filename) {
    // (путь, ширина, высота, количество каналов при загрузке изображения, количество каналов для отображения)
	// каналы STBI_grey = 1, STBI_grey_alpha = 2, STBI_rgb = 3, STBI_rgb_alpha = 4
	image = stbi_load(filename.c_str(), &width, &height, &cnt, 3); // загружаем текстуру
    //Init();
}
*/

void CubeMap::Init(const std::array<std::string, 6>& fileNames) {
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

CubeMap::~CubeMap() {
    GLCall(glDeleteTextures(1, &descriptor));
} 