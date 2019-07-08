#include <iostream>
#include <string>

#include "renderer.h"
#include "shaderProgram.h"
#include "camera.h"
#include "geometry.h"
#include "texture.h"

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
	//program[GL_VERTEX_SHADER] = "glsl/vertex.glsl"; // заполняем мапу
	//program[GL_FRAGMENT_SHADER] = "glsl/fragment.glsl"; // заполняем мапу
	//program[GL_VERTEX_SHADER] = "glsl/phong_notext_vertex.glsl"; // заполняем мапу
	//program[GL_FRAGMENT_SHADER] = "glsl/phong_notext_pixel.glsl"; // заполняем мапу
	program[GL_VERTEX_SHADER] = "glsl/cook_torrance_notext_vertex.glsl"; // заполняем мапу
	program[GL_FRAGMENT_SHADER] = "glsl/cook_torrance_notext_pixel.glsl"; // заполняем мапу

	program.Compile();
	program.Link();

	Texture texture;
	texture.Load("textures/autumnleaves.jpg");
	texture.Init();

	Geometry dragon;
	dragon.Load("data/dragon.obj"); // добавить вывод ошибки, если файл не найден

	//Geometry cube;
	//cube.Load("data/cubetex.obj");

	//OrthoCamera camera; // (левая, правая, нижняя, верхняя, ближняя, задняя стенки)
	//camera.SetAspect((float)window.GetWidth()/(float)window.GetHeight());
	//camera.SetProjection(-20.0f*camera.GetAspect(), 20.0f*camera.GetAspect(), -20.0f, 20.0f, 0.1f, 100.0f);

	PerspectiveCamera camera; // (угол раствора камеры, ширина области просмотра/на высоту, ближняя и дальняя стенки)
	camera.SetAspect((float)window.GetWidth()/(float)window.GetHeight());
	camera.SetProjection(45.0f, camera.GetAspect(), 0.1f, 100.0f);

	double currentTime = 0.0;
	double lastTime = 0.0;
	 
	glm::mat4 projection = camera.GetProjectionMatrix();
	glm::mat4 model = glm::mat4(1.0f);
	glm::mat4 model2 = glm::mat4(1.0f);;
	model2 = glm::translate(model2, glm::vec3(0.0f, 0.0f, -50.0f)); // сдвиг на вектор 
	//model2 = glm::rotate(model2, 90.0f, glm::vec3(0.0, 0.0, 1.0)); // поворот на 90 градусов вдоль Оz
	//model2 = glm::scale(model2, glm::vec3(0.5, 0.5, 0.5));  // масштабирование

	// игровой цикл
	while (!glfwWindowShouldClose(window.GetPointer())) {
		//calculate time
		currentTime = glfwGetTime();
		float deltaTime = currentTime - lastTime;
		lastTime = currentTime;
		 
		// Renderer pass
		renderer.Update();

		glm::mat4 view = camera.GetViewMatrix();
		//model = glm::rotate(model, 5.0f*deltaTime, glm::vec3(0.0f, 1.0f, 0.0f)); // вращение треугольника

		program.Run();
		program.SetUniform("Model", model); // масштабирование повороты и смещение
		program.SetUniform("View", view); // перегоняет координаты в пространство камеры
		program.SetUniform("Projection", projection); // перегоняет в координаты экрана
		//program.SetUniform("Orthographic", orthographic); // перегоняет в координаты экрана
		glm::mat3 model3x3 = model;
		program.SetUniform("NormalMatrix", glm::transpose(glm::inverse(model3x3)));
 		//program.SetUniform("cameraPos", camera.GetPosition()); // для Ламберта это надо закомментить
		dragon.Draw();
		//cube.Draw();

 		//program.SetUniform("Model", model2); 
		//glm::mat3 model3x3_2 = model2;
 		//program.SetUniform("NormalMatrix", glm::transpose(glm::inverse(model3x3_2)));
 		//dragon.Draw();
 
		camera.Update(deltaTime);

		// заменяет цветовой буфер, который использовался для отрисовки на данной итерации и выводит результат на экран
		glfwSwapBuffers(window.GetPointer());
 		// проверяем события (ввод с клавиатуры, перемещение мыши) и вызываем функции обратного вызова(callback))
    	glfwPollEvents(); 
	}

	// delete program!
	program.Delete(); // убрать в деструктор 
	window.Delete(); // убрать в деструктор 
	return 0;
}