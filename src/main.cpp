#include <iostream>
#include <string>

#include "renderer.h"
#include "camera.h"
#include "mesh.h"
#include "texture.h"
#include "resourceManager.h"

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


	ResourceManager<Texture> textureManager;
	textureManager.Add("textures/brickAO.png");
	textureManager.Add("textures/brickAlbedo.png");
	textureManager.Add("textures/brickMetallic.png");
	textureManager.Add("textures/brickNormalMap.png");

	ResourceManager<Geometry> geometryManager;
	geometryManager.Add("data/dragon.obj");
	geometryManager.Add("data/sphere.obj");

	const Geometry* sphereGeo = geometryManager.Get("data/sphere.obj");
	const Geometry* dragonGeo = geometryManager.Get("data/dragon.obj");

	PhongTextureMaterial wood(&phong_text_program, 
							  textureManager.Get("textures/brickAO.png"), 
							  textureManager.Get("textures/brickAlbedo.png"),
							  textureManager.Get("textures/brickMetallic.png"),
							  textureManager.Get("textures/brickNormalMap.png"),
							  0.4);
	PhongMaterial emerald(&program, {0.0215, 0.1745, 0.0215}, {0.07568, 0.61424, 0.07568}, {0.633, 0.727811, 0.633}, 0.6f);


	Mesh dragon = { {dragonGeo, &emerald} };
	Mesh sphere = { {sphereGeo, &wood} };

	PerspectiveCamera camera; // (угол раствора камеры, ширина области просмотра/на высоту, ближняя и дальняя стенки)
	camera.SetAspect((float)window.GetWidth()/(float)window.GetHeight());
	camera.SetProjection(45.0f, camera.GetAspect(), 0.1f, 100.0f);
	camera.SetPosition(glm::vec3(0.0f, 10.0f, 20.0f));
	camera.SetPitch(-20.0f);

	renderer.SetActiveCamera(&camera);
	renderer.AddMesh(&sphere);
	renderer.AddMesh(&dragon);
	
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
    	glfwPollEvents(); // вызывает callback функции
	}
	// delete program!
	window.Delete(); // убрать в деструктор 
	return 0;
}