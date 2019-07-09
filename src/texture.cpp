#define STB_IMAGE_IMPLEMENTATION
#include "texture.h"

Texture::Texture() {
    glGenTextures(1, &descriptor); // 1 - количество текстур для генерации
}

void Texture::Bind() const {
    glBindTexture(GL_TEXTURE_2D, descriptor); // привязка текстуры
 }

void Texture::Load(const std::string& filename) {
    Bind();
    // (путь, ширина, высота, количество каналов при загрузке изображения, количество каналов для отображения)
	// каналы STBI_grey = 1, STBI_grey_alpha = 2, STBI_rgb = 3, STBI_rgb_alpha = 4
	image = stbi_load(filename.c_str(), &width, &height, &cnt, 0); // загружаем текстуру
}

void Texture::Init() {
    // (текстурная цель, уровень мипмапа, формат текстуры, ширина, высота, 0, формат исходного изображения,
	//  тип данных исходного изображения, данные изображения)
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image); // генерируем текстуру
	glGenerateMipmap(GL_TEXTURE_2D); // генерация всех необходимых мипмапов для текстуры
	stbi_image_free(image); // освобождения памяти
}

// !!! где-то надо отвязать (не знаю, где)
void Texture::Unbind() const { 
    glBindTexture(GL_TEXTURE_2D, 0); // отвязка объекта текстуры
}

Texture::~Texture() {
    glDeleteTextures(1, &descriptor);
}
