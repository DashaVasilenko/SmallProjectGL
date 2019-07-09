#include <iostream>
#include <string>

#include "renderer.h"
#include "material.h"
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
	program[GL_VERTEX_SHADER] = "glsl/phong_notext_vertex.glsl"; // заполняем мапу
	program[GL_FRAGMENT_SHADER] = "glsl/phong_notext_pixel.glsl"; // заполняем мапу
	program.Compile();
	program.Link();

	PhongMaterial phong(&program, {0.19225, 0.19225, 0.19225}, {0.50754, 0.50754, 0.50754}, {0.50827, 0.50827, 0.50827}, 40.0f);

	Texture texture;
	texture.Load("textures/autumnleaves.jpg");
	texture.Init();

	Geometry dragon;
	dragon.Load("data/dragon.obj"); // добавить вывод ошибки, если файл не найден

	PerspectiveCamera camera; // (угол раствора камеры, ширина области просмотра/на высоту, ближняя и дальняя стенки)
	camera.SetAspect((float)window.GetWidth()/(float)window.GetHeight());
	camera.SetProjection(45.0f, camera.GetAspect(), 0.1f, 100.0f);

	double currentTime = 0.0;
	double lastTime = 0.0;
	 
	glm::mat4 projection = camera.GetProjectionMatrix();
	glm::mat4 model = glm::mat4(1.0f);

	// игровой цикл
	while (!glfwWindowShouldClose(window.GetPointer())) {
		//calculate time
		currentTime = glfwGetTime();
		float deltaTime = currentTime - lastTime;
		lastTime = currentTime;
		camera.Update(deltaTime);
		 
		// Renderer pass
		renderer.Update();
		glm::mat4 view = camera.GetViewMatrix();
		phong.Bind();
		phong.SetProjectionMatrix(projection);
		phong.SetViewMatrix(view);
		phong.SetModelMatrix(model);
		phong.SetInnerUniforms();
		dragon.Draw();
		
		// заменяет цветовой буфер, который использовался для отрисовки на данной итерации и выводит результат на экран
		glfwSwapBuffers(window.GetPointer());
 		// проверяем события (ввод с клавиатуры, перемещение мыши) и вызываем функции обратного вызова(callback))
    	glfwPollEvents(); 
	}
	// delete program!
	window.Delete(); // убрать в деструктор 
	return 0;
}