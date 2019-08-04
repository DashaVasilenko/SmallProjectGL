#include <iostream>
#include <string>

#include "engine.h"
#include "renderer.h"
#include "camera.h"
#include "mesh.h"
#include "transform.h"
#include "light.h"
#include "system_gui.h"

#include "entt/entt.hpp"

int main() {
	Window window;
	window.SetWidth(1080);
	window.SetHeight(768);
	window.SetName("Karl's window!");
	window.Init();

	




	SystemGUI gui;
	gui.Init(window.GetPointer());
	auto& console = gui.GetConsole();

	
	Renderer::SetWidth(window.GetWidth());
	Renderer::SetHeight(window.GetHeight());
	Renderer renderer;
	renderer.Init();

	console->AddCommand("wireframe", renderer.GetWireFrameFunc());
	console->AddCommand("viewbuffer", renderer.GetViewBufferFunc());

	

	ShaderProgram* program = Engine::programManager.Get("data/shaders/phong.json");
	ShaderProgram* pbr_program = Engine::programManager.Get("data/shaders/pbr.json");

	// const Geometry* dragonGeo = geometryManager.Get("data/dragon.obj");
	const Geometry* cubeGeo = Engine::geometryManager.Get("data/cube.obj");
	const Geometry* sphereGeo = Engine::geometryManager.Get("data/sphere.obj");

	PhongMaterial emerald(program, {0.0215, 0.1745, 0.0215}, {0.07568, 0.61424, 0.07568}, {0.633, 0.727811, 0.633}, 76.8f);
	PhongMaterial ruby(program, {0.1745f, 0.01175f, 0.01175f}, {0.61424f, 0.04136f, 0.04136f}, {0.727811f, 0.626959f, 0.626959f}, 76.8f);
	PhongMaterial silver(program, {0.23125f, 0.23125f, 0.23125f}, {0.2775f, 0.2775f, 0.2775f}, {0.773911f, 0.773911f, 0.773911f}, 89.6f);

	PbrMaterial brick(pbr_program,
					 Engine::textureManager.Get("textures/brickAlbedo.png"),
					 Engine::textureManager.Get("textures/brickNormalMap.png"),
					 Engine::textureManager.Get("textures/brickMetallic.png"),
					 Engine::textureManager.Get("textures/brickRoughness.png"),
					 Engine::textureManager.Get("textures/brickAO.png")
					);

	PbrMaterial wood(pbr_program,
					 Engine::textureManager.Get("textures/woodAlbedo.png"),
					 Engine::textureManager.Get("textures/woodNormalMap.png"),
					 Engine::textureManager.Get("textures/woodMetallic.png"),
					 Engine::textureManager.Get("textures/woodRoughness.png"),
					 Engine::textureManager.Get("textures/woodAO.png")
					);	

	entt::registry registry;

	Mesh cube_mesh = { {cubeGeo, &wood} };
	Mesh sphere_mesh = { {sphereGeo, &brick} };

	glm::vec3 colors[9];
	colors[0] = {1.0f , 0.0f, 0.0f};
	colors[1] = {0.0f , 1.0f, 0.0f};
	colors[2] = {0.0f , 0.0f, 1.0f};
	colors[3] = {1.0f , 1.0f, 0.0f};
	colors[4] = {1.0f , 0.0f, 1.0f};
	colors[5] = {0.0f , 1.0f, 1.0f};
	colors[6] = {1.0f , 1.0f, 1.0f};
	colors[7] = {1.0f , 1.0f, 0.0f};
	colors[8] = {1.0f , 1.0f, 1.0f};

	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			auto sphere = registry.create();
			Transform sphere_transform;
			sphere_transform.Translate({static_cast<float>(i*3 - 5.0f), 1.0f, static_cast<float>(j*2) - 5.0f});

			PointLight pl(colors[3*i+j], {static_cast<float>(i*3 - 5.0f), 4.0f, static_cast<float>(j*2) - 5.0f}, 5.0f);
			pl.SetAttenuation(0.0f, 0.0f, 2.0f);
			registry.assign<Mesh>(sphere, sphere_mesh);
			registry.assign<Transform>(sphere, sphere_transform);
			registry.assign<PointLight>(sphere, pl);
		}
	}

	auto cube = registry.create();
	Transform cube_transform;

	cube_transform.Translate({7.0f, 0.0f, 7.0f});
	cube_transform.Scale({40.0f, 0.15f, 40.0f});
	registry.assign<Mesh>(cube, std::move(cube_mesh));
	registry.assign<Transform>(cube, std::move(cube_transform));
	

	PerspectiveCamera camera; // (угол раствора камеры, ширина области просмотра/на высоту, ближняя и дальняя стенки)
	camera.SetAspect((float)window.GetWidth()/(float)window.GetHeight());
	camera.SetProjection(45.0f, camera.GetAspect(), 0.1f, 100.0f);
	camera.SetPosition(glm::vec3(7.0f, 10.0f, 20.0f));
	camera.SetPitch(-20.0f);

	renderer.SetActiveCamera(&camera);
	
	double currentTime = 0.0;
	double lastTime = 0.0;
	
	

    
	// игровой цикл
	while (!glfwWindowShouldClose(window.GetPointer())) {
		//calculate time
		glfwPollEvents(); // вызывает callback функции
		
		currentTime = glfwGetTime();
		float deltaTime = currentTime - lastTime;
		lastTime = currentTime;
		camera.Update(deltaTime);
		// Renderer pass
		renderer.Update(registry);
		gui.Update();
    	

		// заменяет цветовой буфер, который использовался для отрисовки на данной итерации и выводит результат на экран

		glfwSwapBuffers(window.GetPointer());
 		// проверяем события (ввод с клавиатуры, перемещение мыши) и вызываем функции обратного вызова(callback))
    	
	}	
	window.Delete(); // убрать в деструктор 

	return 0;
}