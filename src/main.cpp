#include <iostream>
#include <string>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

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
	program[GL_VERTEX_SHADER] = "glsl/vertex.glsl"; // заполняем мапу
	program[GL_FRAGMENT_SHADER] = "glsl/fragment.glsl"; // заполняем мапу
	program.Compile();
	program.Link();

	Geometry dragon;
	dragon.Load("data/dragon.obj");


	float aspect = (float)window.GetWidth()/(float)window.GetHeight();


	OrthoCamera camera;
	camera.SetLeftPlane(-1.0f);
    camera.SetRightPlane(1.0f);  
    camera.SetBottomPlane(-1.0f*aspect);  
    camera.SetTopPlane(1.0f*aspect);  
    camera.SetNearPlane(0.1f);  
    camera.SetFarPlane(100.0f); 

/* 
	PerspectiveCamera camera;
	camera.SetFieldOfView(45.0f); 
    camera.SetWidthToHeight(aspect); 
    camera.SetNearPlane(0.1f);  
    camera.SetFarPlane(100.0f);  
*/
	double currentTime = 0.0;
	double lastTime = 0.0;

	// проекционная камера (угол раствора камеры, ширина области просмотра/на высоту, ближняя и дальняя стенки)
	//glm::mat4 projection = glm::perspective( 45.0f, (float)window.GetWidth()/(float)window.GetHeight(), 0.1f, 100.0f);
	// создание ортографической камеры (левая, правая, нижняя, верхняя, ближняя, задняя стенки)
	//glm::mat4 projection = glm::ortho(-30.0f, 30.0f, -30.0f, 30.0f, 0.1f, 100.0f);
	 
	glm::mat4 projection = camera.GetProjectionMatrix();

	glm::mat4 model = glm::mat4(1.0f);

	glm::mat4 model2 = glm::mat4(1.0f);;
	model2 = glm::translate(model2, glm::vec3(0.0f, 0.0f, -50.0f)); // сдвиг на вектор 
	//model2 = glm::rotate(model2, 90.0f, glm::vec3(0.0, 0.0, 1.0)); // поворот на 90 градусов вдоль Оz
	//model2 = glm::scale(model2, glm::vec3(0.5, 0.5, 0.5));  // масштабирование

	//glm::mat4 model2 = glm::mat4(1.0f);
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
		program.SetUniform("cameraPos", camera.GetPosition());
		dragon.Draw();

 		program.SetUniform("Model", model2); 
		glm::mat3 model3x3_2 = model2;
 		program.SetUniform("NormalMatrix", glm::transpose(glm::inverse(model3x3_2)));
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