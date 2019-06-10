#include <iostream>
#include <string>

#include "renderer.h"
#include "shaderProgram.h"
#include "camera.h"

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

	ShaderProgram program;
	program[GL_VERTEX_SHADER] = "glsl/vertex.glsl";
	program[GL_FRAGMENT_SHADER] = "glsl/fragment.glsl";
	program.Compile();
	program.Link();

	// кубик вершины
	GLfloat vertices[] = { 0.5f,  0.5f, -0.5f, 1.0, 0.0, 0.0, // Верхний правый угол
     					   0.5f, -0.5f, -0.5f, 0.0, 1.0, 0.0, // Нижний правый угол
   						  -0.5f, -0.5f, -0.5f, 0.0, 0.0, 1.0, // Нижний левый угол
   						  -0.5f,  0.5f, -0.5f, 1.0, 1.0, 0.0, // Верхний левый угол
						   0.5f,  0.5f, 0.5f, 1.0, 0.0, 1.0, // Верхний правый угол
     					   0.5f, -0.5f, 0.5f, 0.0, 1.0, 1.0, // Нижний правый угол
   						  -0.5f, -0.5f, 0.5f, 1.0, 1.0, 1.0, // Нижний левый угол
   						  -0.5f,  0.5f, 0.5f, 0.0, 0.0, 0.0 // Верхний левый угол
	};

	// кубик индексы вершин для треугольников
	GLuint indices[] = { 0, 1, 3,   // Первый треугольник
    					 1, 2, 3,   // Второй треугольник
						 1, 2, 6,
						 1, 5, 6,
						 1, 0, 4,
						 4, 5, 1,
						 7, 5, 6,
						 7, 4, 5,
						 7, 0, 3,
						 7, 0, 4,
						 3, 2, 6,
						 3, 6, 7
	};  

/* 
	// координаты и цвет вершин 1 треугольника
	float vertices[] = {-0.5, -0.5, 0.0, 1.0, 0.0, 0.0,
						0.5, -0.5, 0.0, 0.0, 1.0, 0.0,
						0.0, 0.5, 0.0, 0.0, 0.0, 1.0 
	};
*/

	GLuint EBO;
	glGenBuffers(1, &EBO); // создаем буфер
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO); // определяем тип буфера
	// копируем вершинные данные в буфер (тип буфера, количество данных в байтах, данные, режим работы с данными)
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW); 

	// создаем буфер VAO (vertex array object)
	GLuint VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	GLuint VBO;
	glGenBuffers(1, &VBO); // создаем буфер VBO (vertex buffer objects) 1 - кол-во буферов
	glBindBuffer(GL_ARRAY_BUFFER, VBO); // определяем тип буфера
	// копируем вершинные данные в буфер (тип буфера, количество данных в байтах, данные, режим работы с данными)
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	// сообщаем OpenGL как он должен интерпретировать вершинные данные
	// (какой аргумент шейдера мы хотим настроить(layout (location = 0)), размер аргумента в шейдере, тип данных,
	//  необходимость нормализовать входные данные, расстояние между наборами данных, смещение начала данных в буфере)
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0); // включаем атрибуты, т.е. передаем вершинному атрибуту позицию аргумента
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3*sizeof(GLfloat)) );
	glEnableVertexAttribArray(1);

	// в рендерер
	glm::mat4 projection = glm::perspective( 45.0f, (float)window.GetWidth()/(float)window.GetHeight(), 0.1f, 100.0f);

	Camera camera;
	double currentTime = 0.0;
	double lastTime = 0.0;


	glm::mat4 model = glm::mat4(1.0f);
	while (!glfwWindowShouldClose(window.GetPointer())) {
		//calculate time
		currentTime = glfwGetTime();
		float deltaTime = currentTime - lastTime;
		lastTime = currentTime;
		 
		// Renderer pass
		renderer.Update();

		glm::mat4 view = camera.GetViewMatrix();
	//	model = glm::rotate(model, 5.0f*deltaTime, glm::vec3(0.0f, 1.0f, 0.0f)); // вращение треугольника

		glm::mat4 mvp = projection*view*model;
		program.Run();
		program.SetUniform("MVP", mvp);

/*  // для треугольника
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);
*/
		//  для прямоугольника
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

		camera.Update(deltaTime);

		// заменяет цветовой буфер, который использовался для отрисовки на данной итерации и выводит результат на экран
		glfwSwapBuffers(window.GetPointer());
 		// проверяем события (ввод с клавиатуры, перемещение мыши) и вызываем функции обратного вызова(callback))
    	glfwPollEvents(); 
	}

	// delete program!
	program.Delete();
	window.Delete();
	return 0;
}