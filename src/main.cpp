#include <iostream>
#include <string>

#include "renderer.h"
#include "camera.h"
#include "mesh.h"
#include "texture.h"
#include "resourceManager.h"

#include "entt/entt.hpp"

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

	ResourceManager<ShaderProgram> programManager;
	ResourceManager<Texture> textureManager;
	ResourceManager<Geometry> geometryManager;

	const ShaderProgram* program = programManager.Get("data/shaders/phong.json");
	const ShaderProgram* phong_text_program = programManager.Get("data/shaders/phong_texture.json");

	const Geometry* sphereGeo = geometryManager.Get("data/sphere.obj");
	const Geometry* dragonGeo = geometryManager.Get("data/dragon.obj");

	PhongTextureMaterial wood(phong_text_program, 
							  textureManager.Get("textures/brickAO.png"), 
							  textureManager.Get("textures/brickAlbedo.png"),
							  textureManager.Get("textures/brickMetallic.png"),
							  textureManager.Get("textures/brickNormalMap.png"),
							  0.4);
	PhongMaterial emerald(program, {0.0215, 0.1745, 0.0215}, {0.07568, 0.61424, 0.07568}, {0.633, 0.727811, 0.633}, 0.6f);

	
	entt::registry registry;
	Mesh dragon = { {dragonGeo, &emerald} };
	Mesh sphere = { {sphereGeo, &wood} };

	registry.assign<Mesh>(registry.create(), std::move(dragon) );
	registry.assign<Mesh>(registry.create(), std::move(sphere) );

	PerspectiveCamera camera; // (угол раствора камеры, ширина области просмотра/на высоту, ближняя и дальняя стенки)
	camera.SetAspect((float)window.GetWidth()/(float)window.GetHeight());
	camera.SetProjection(45.0f, camera.GetAspect(), 0.1f, 100.0f);
	camera.SetPosition(glm::vec3(0.0f, 10.0f, 20.0f));
	camera.SetPitch(-20.0f);

	renderer.SetActiveCamera(&camera);
	
	double currentTime = 0.0;
	double lastTime = 0.0;
	
	struct pos {
		double x;
		double y;
	};
	
	// игровой цикл
	while (!glfwWindowShouldClose(window.GetPointer())) {
		//calculate time
		currentTime = glfwGetTime();
		float deltaTime = currentTime - lastTime;
		lastTime = currentTime;
		camera.Update(deltaTime);
		// Renderer pass
		renderer.Update(registry);
		// заменяет цветовой буфер, который использовался для отрисовки на данной итерации и выводит результат на экран
		glfwSwapBuffers(window.GetPointer());
 		// проверяем события (ввод с клавиатуры, перемещение мыши) и вызываем функции обратного вызова(callback))
    	glfwPollEvents(); // вызывает callback функции
	}
	// delete program!
	window.Delete(); // убрать в деструктор 
	return 0;
}