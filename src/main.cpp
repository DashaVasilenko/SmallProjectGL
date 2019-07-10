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

	ShaderProgram phong_text_program;
	phong_text_program[GL_VERTEX_SHADER] = "glsl/phong_text_vertex.glsl";
	phong_text_program[GL_FRAGMENT_SHADER] = "glsl/phong_text_pixel.glsl";
	phong_text_program.Compile();
	phong_text_program.Link();

	Texture ambient;
	ambient.Load("textures/woodAO.png");
	ambient.Init();

	Texture diffuse;
	diffuse.Load("textures/woodAlbedo.png");
	diffuse.Init();

	Texture specular;
	specular.Load("textures/woodAlbedo.png");
	specular.Init();

	PhongTextureMaterial wood(&phong_text_program, &ambient, &diffuse, &specular, 0.4);
	PhongMaterial emerald(&program, {0.0215, 0.1745, 0.0215}, {0.07568, 0.61424, 0.07568}, {0.633, 0.727811, 0.633}, 0.6f);

	Geometry dragonGeometry;
	dragonGeometry.Load("data/dragon.obj");  //добавить вывод ошибки, если файл не найден

	//Mesh dragon = { {&dragonGeometry, &wood} };
	//dragon.SetModelMatrix(glm::scale(glm::mat4(1.0f), {0.1f, 0.1f, 0.1f}));

	//Geometry tableGeometry;
	//tableGeometry.Load("data/table.obj");

	Geometry cubeGeometry;
	cubeGeometry.Load("data/cube.obj");

	Mesh cube = { {&cubeGeometry, &wood} };
	cube.SetModelMatrix(glm::scale(glm::mat4(1.0f), {1.0f, 1.0f, 1.0f}));

	//OrthoCamera camera; // (левая, правая, нижняя, верхняя, ближняя, задняя стенки)
	//camera.SetAspect((float)window.GetWidth()/(float)window.GetHeight());
	//camera.SetProjection(-20.0f*camera.GetAspect(), 20.0f*camera.GetAspect(), -20.0f, 20.0f, 0.1f, 100.0f);

	PerspectiveCamera camera; // (угол раствора камеры, ширина области просмотра/на высоту, ближняя и дальняя стенки)
	camera.SetAspect((float)window.GetWidth()/(float)window.GetHeight());
	camera.SetProjection(45.0f, camera.GetAspect(), 0.1f, 100.0f);

//	glm::mat4 projection = camera.GetProjectionMatrix();
//	glm::mat4 model = glm::mat4(1.0f);
//	glm::mat4 model2 = glm::mat4(1.0f);;
//	model2 = glm::translate(model2, glm::vec3(0.0f, 0.0f, -50.0f)); // сдвиг на вектор 
//	//model2 = glm::rotate(model2, 90.0f, glm::vec3(0.0, 0.0, 1.0)); // поворот на 90 градусов вдоль Оz
//	//model2 = glm::scale(model2, glm::vec3(0.5, 0.5, 0.5));  // масштабирование

	renderer.SetActiveCamera(&camera);
	renderer.AddMaterial(&wood);
	renderer.AddMesh(cube);
	
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