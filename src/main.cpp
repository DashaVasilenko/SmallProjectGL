#include <iostream>
#include <string>

#include "renderer.h"
#include "camera.h"
#include "mesh.h"
#include "texture.h"
#include "resourceManager.h"
#include "transform.h"
#include "light.h"

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

	const Geometry* dragonGeo = geometryManager.Get("data/dragon.obj");
	const Geometry* cubeGeo = geometryManager.Get("data/cube.obj");

	PhongTextureMaterial wood(phong_text_program, 
							  textureManager.Get("textures/brickAO.png"), 
							  textureManager.Get("textures/brickAlbedo.png"),
							  textureManager.Get("textures/brickMetallic.png"),
							  textureManager.Get("textures/brickNormalMap.png"),
							  0.4);
	PhongMaterial emerald(program, {0.0215, 0.1745, 0.0215}, {0.07568, 0.61424, 0.07568}, {0.633, 0.727811, 0.633}, 0.6f);

	PointLight light(program, {1.0f, 1.0f, 1.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.5f, 0.0f});
	light.setAttenuation(0.7f, 0.14f, 0.02f);
	light.SetInnerUniforms();

	entt::registry registry;
	Mesh dragon_mesh = { {dragonGeo, &emerald} };
	Mesh cube_mesh = { {cubeGeo, &emerald} };

	auto dragon = registry.create();
	auto dragon2 = registry.create();
	auto cube = registry.create();


	Transform dragon_transform;
	dragon_transform.Rotate({0.0f, 1.0f, 0.0f}, 90.0f);
	dragon_transform.Translate({-7.0f, 0.0f, 0.0f});

	Transform dragon_transform2;
	dragon_transform2.Rotate({0.0f, 1.0f, 0.0f}, 90.0f);
	dragon_transform2.Translate({7.0f, 0.0f, 0.0f});

	Transform cube_transform;
	cube_transform.Scale({25.0f, 0.15f, 25.0f});

	registry.assign<Mesh>(dragon, dragon_mesh );
	registry.assign<Transform>(dragon, dragon_transform);
	registry.assign<Mesh>(dragon2, dragon_mesh );
	registry.assign<Transform>(dragon2, dragon_transform2);
	registry.assign<Mesh>(cube, cube_mesh);
	registry.assign<Transform>(cube, cube_transform);

	PerspectiveCamera camera; // (угол раствора камеры, ширина области просмотра/на высоту, ближняя и дальняя стенки)
	camera.SetAspect((float)window.GetWidth()/(float)window.GetHeight());
	camera.SetProjection(45.0f, camera.GetAspect(), 0.1f, 100.0f);
	camera.SetPosition(glm::vec3(0.0f, 10.0f, 20.0f));
	camera.SetPitch(-20.0f);

	renderer.SetActiveCamera(&camera);
	
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