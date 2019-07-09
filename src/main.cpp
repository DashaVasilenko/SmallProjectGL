#include <iostream>
#include <string>

#include "renderer.h"
#include "camera.h"
#include "mesh.h"
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

	PhongMaterial silver(&program, {0.19225, 0.19225, 0.19225}, {0.50754, 0.50754, 0.50754}, {0.50827, 0.50827, 0.50827}, 0.4f);
	PhongMaterial emerald(&program, {0.0215, 0.1745, 0.0215}, {0.07568, 0.61424, 0.07568}, {0.633, 0.727811, 0.633}, 0.6f);

	Geometry dragonGeometry;
	dragonGeometry.Load("data/dragon.obj"); // добавить вывод ошибки, если файл не найден

	Mesh dragon = { {&dragonGeometry, &silver} };
	Mesh dragon2 = { {&dragonGeometry, &emerald} };
	dragon2.SetModelMatrix(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 15.0f, 0.0f)));

	Texture texture;
	texture.Load("textures/autumnleaves.jpg");
	texture.Init();

	PerspectiveCamera camera; // (угол раствора камеры, ширина области просмотра/на высоту, ближняя и дальняя стенки)
	camera.SetAspect((float)window.GetWidth()/(float)window.GetHeight());
	camera.SetProjection(45.0f, camera.GetAspect(), 0.1f, 100.0f);

	renderer.SetActiveCamera(&camera);
	renderer.AddMaterial(&silver);
	renderer.AddMaterial(&emerald);
	renderer.AddMesh(dragon);
	renderer.AddMesh(dragon2);

	double currentTime = 0.0;
	double lastTime = 0.0;
	 
	// игровой цикл
	while (!glfwWindowShouldClose(window.GetPointer())) {
		//calculate time
		currentTime = glfwGetTime();
		float deltaTime = currentTime - lastTime;
		lastTime = currentTime;
		camera.Update(deltaTime);
		 
		// Renderer pass
		renderer.Update();
		
		// заменяет цветовой буфер, который использовался для отрисовки на данной итерации и выводит результат на экран
		glfwSwapBuffers(window.GetPointer());
 		// проверяем события (ввод с клавиатуры, перемещение мыши) и вызываем функции обратного вызова(callback))
    	glfwPollEvents(); 
	}
	// delete program!
	window.Delete(); // убрать в деструктор 
	return 0;
}