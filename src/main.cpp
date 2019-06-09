#include <iostream>
#include <string>

#include "renderer.h"
#include "shaderProgram.h"

int main() {
	Window window;
	window.SetWidth(1080);
	window.SetHeight(768);
	window.SetName("Karl's window!");
	window.Init();

	Renderer renderer;
	renderer.SetWidth(window.GetWidth());
	renderer.SetHeight(window.GetHeight());
	renderer.Init();


	std::map<GLenum, std::string> mapSources;
	mapSources[GL_VERTEX_SHADER] = "glsl/vertex.glsl";
	mapSources[GL_FRAGMENT_SHADER] = "glsl/fragment.glsl";

	ShaderProgram program;
	program.Init(mapSources);
	program.Compile();
	program.Link();


	// координаты и цвет вершин 1 треугольника
	float vertices[] = {0.66, 0.13, 0.0, 1.0, 0.0, 0.0,
						0.33, 0.67, 0.0, 0.0, 1.0, 0.0,
						0.97, 0.97, 0.0, 0.0, 0.0, 1.0 };

	// создаем буфер VAO (vertex array object)
	GLuint VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	// создаем буфер VBO (vertex buffer objects)
	GLuint VBO;
	glGenBuffers(1, &VBO);
	// привязываем буфер
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// копирования вершинных данных в этот буфер
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
 
	// сообщаем OpenGL как он должен интерпретировать вершинные данные
	// (какой аргумент шейдера мы хотим настроить(layout (location = 0)), размер аргумента в шейдере, тип данных,
	//  необходимость нормализовать входные данные, расстояние между наборами данных, смещение начала данных в буфере)
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0); // включаем атрибуты, т.е. передаем вершинному атрибуту позицию аргумента
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3*sizeof(GLfloat)) );
	glEnableVertexAttribArray(1);



	while (!glfwWindowShouldClose(window.GetPointer())) {
		// Renderer pass
		renderer.Update();
		program.Run();

		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		/////////////////

		glfwSwapBuffers(window.GetPointer());
    	glfwPollEvents();
	}


	// delete program!
	program.Delete();
	window.Destroy();
	return 0;
}