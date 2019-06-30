#include <iostream>
#include <string>

#include "renderer.h"
#include "shaderProgram.h"
#include "camera.h"
#include "geometry.h"

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


	Geometry dragon;
	dragon.Load("data/dragon.obj");

	// в камеру
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
		//model = glm::rotate(model, 5.0f*deltaTime, glm::vec3(0.0f, 1.0f, 0.0f)); // вращение треугольника

		program.Run();
		program.SetUniform("Model", model);
		program.SetUniform("View", view);
		program.SetUniform("Projection", projection);
		glm::mat3 model3x3 = model;
		program.SetUniform("NormalMatrix", glm::transpose(glm::inverse(model3x3)));
		program.SetUniform("cameraPos", camera.GetPosition());
		dragon.Draw();

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