#include <iostream>
#include <string>

#include "renderer.h"
#include "camera.h"
#include "mesh.h"
#include "texture.h"
#include "resourceManager.h"
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

	ResourceManager<ShaderProgram> programManager;
	ResourceManager<Texture> textureManager;
	ResourceManager<Geometry> geometryManager;

	Renderer renderer;
	renderer.SetWidth(window.GetWidth());
	renderer.SetHeight(window.GetHeight());
	renderer.Init(programManager.Get("data/shaders/quad.json"), geometryManager.Get("data/quad.obj"));

	console->AddCommand("wireframe", renderer.GetWireFrameFunc());

	const ShaderProgram* program = programManager.Get("data/shaders/phong.json");
	const ShaderProgram* pbr_program = programManager.Get("data/shaders/pbr.json");

	const Geometry* cubeGeo = geometryManager.Get("data/cube.obj");
	const Geometry* sphereGeo = geometryManager.Get("data/sphere.obj");

	PhongMaterial emerald(program, {0.0215, 0.1745, 0.0215}, {0.07568, 0.61424, 0.07568}, {0.633, 0.727811, 0.633}, 76.8f);
	PhongMaterial ruby(program, {0.1745f, 0.01175f, 0.01175f}, {0.61424f, 0.04136f, 0.04136f}, {0.727811f, 0.626959f, 0.626959f}, 76.8f);
	PhongMaterial silver(program, {0.23125f, 0.23125f, 0.23125f}, {0.2775f, 0.2775f, 0.2775f}, {0.773911f, 0.773911f, 0.773911f}, 89.6f);

	PbrMaterial brick(pbr_program,
					 textureManager.Get("textures/brickAlbedo.png"),
					 textureManager.Get("textures/brickNormalMap.png"),
					 textureManager.Get("textures/brickMetallic.png"),
					 textureManager.Get("textures/brickRoughness.png"),
					 textureManager.Get("textures/brickAO.png")
					);

	PbrMaterial wood(pbr_program,
					 textureManager.Get("textures/woodAlbedo.png"),
					 textureManager.Get("textures/woodNormalMap.png"),
					 textureManager.Get("textures/woodMetallic.png"),
					 textureManager.Get("textures/woodRoughness.png"),
					 textureManager.Get("textures/woodAO.png")
					);

			
	SpotLight light(program, {0.7, 0.7, 0.0}, {0.7, 0.7, 0.0}, {0.7, 0.7, 0.0}, {0.0f, 25.0f, 0.0f}, {0.0f, -1.0f, 0.0f}, 30.0f);
	light.setAttenuation(1.0f);
	light.SetInnerUniforms();
	

	entt::registry registry;

	Mesh cube_mesh = { {cubeGeo, &wood} };
	Mesh sphere_mesh = { {sphereGeo, &brick} };


	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < 10; j++) {
			auto sphere = registry.create();
			Transform sphere_transform;
			sphere_transform.Translate({static_cast<float>(i*3 - 5.0f), 1.0f, static_cast<float>(j*2) - 5.0f});
			registry.assign<Mesh>(sphere, sphere_mesh);
			registry.assign<Transform>(sphere, sphere_transform);
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