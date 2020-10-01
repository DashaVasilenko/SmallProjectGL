#include <iostream>
#include <string>
#include <random>

#include "engine.h"
#include "renderer.h"
#include "camera.h"
#include "mesh.h"
#include "transform.h"
#include "light.h"
#include "system_gui.h"
#include "errors.h"

#include "entt/entt.hpp"

float get_random() {
    static std::default_random_engine e;
    static std::uniform_real_distribution<float> dis(0, 1);
    return dis(e);
}

int main() {
	Window window;
	window.SetWidth(1024);
	window.SetHeight(768);
	window.SetName("Karl's window!");
	window.Init();

	SystemGUI gui;
	gui.Init(window.GetPointer());
	auto& console = gui.GetConsole();
	
	Renderer::SetWidth(window.GetDrawWidth());
	Renderer::SetHeight(window.GetDrawHeight());
	Renderer renderer;
	renderer.Init();

	// registering console functions
	console->AddCommand("wireframe", renderer.GetWireFrameFunc());
	console->AddCommand("viewbuffer", renderer.GetViewBufferFunc()); 
	console->AddCommand("debuglight", renderer.GetDebugLightFunc());
	
	entt::registry registry;

	// Sky
	std::array<std::string, 6> fileNames = { "textures/skybox/front.tga",
											 "textures/skybox/back.tga",
											 "textures/skybox/up.tga",
											  
											 "textures/skybox/down.tga",
											 "textures/skybox/right.tga",
											 "textures/skybox/left.tga"
											 };
	// Sky
	SkyBox skybox;
	skybox.Init(fileNames);
	DirectionalLight dl({3.0f, 3.0f, 3.0f}, {1.0f, -1.0f, 0.0f});

	auto sky = registry.create();
	registry.assign<DirectionalLight>(sky, dl);
	registry.assign<SkyBox>(sky, skybox);

	// Loading obj files
	const Geometry* cubeGeo = Engine::geometryManager.Get("data/cube.obj");
	const Geometry* sphereGeo = Engine::geometryManager.Get("data/sphere_highpoly.obj");
	const Geometry* dragonGeo = Engine::geometryManager.Get("data/dragon.obj");

	// Floor
	MaterialPBR floor_Material;
	floor_Material.SetAlbedo("textures/7/d.png");
	floor_Material.SetRoughness("textures/7/r.png");
	floor_Material.SetNormalMap("textures/7/n.png", NORMAL_MAP);
	floor_Material.SetScale(2.0f);
	floor_Material.Init();

	Mesh floor_mesh { {cubeGeo, &floor_Material} };

	auto floor = registry.create();
	Transform floor_transform;
	floor_transform.Scale({40.0f, 0.15f, 40.0f});
	registry.assign<Mesh>(floor, floor_mesh);
	registry.assign<Transform>(floor, floor_transform);
	
	// Dragon
	MaterialPBR dragon_Material;

	dragon_Material.SetAlbedo(glm::vec3(0.207, 0.556, 0.737));
	dragon_Material.SetRoughness(0.4f);
	dragon_Material.Init();

	Mesh dragon_mesh { {dragonGeo, &dragon_Material} };
	auto dragon = registry.create();
	Transform dragon_transform;

	registry.assign<Transform>(dragon, dragon_transform);
	registry.assign<Mesh>(dragon, dragon_mesh);
	
	
	// Spheres
	MaterialPBR sphere_Material_1;
	sphere_Material_1.SetAlbedo("textures/rustedironAlbedo.png");
	sphere_Material_1.SetNormalMap("textures/rustedironNormalMap.png", NORMAL_MAP);
	sphere_Material_1.SetRoughness("textures/rustedironRoughness.png");
	sphere_Material_1.SetRoughness("textures/rustedironMetallic.png");
	sphere_Material_1.Init();

	MaterialPBR sphere_Material_2;
	sphere_Material_2.SetAlbedo("textures/woodAlbedo.png");
	sphere_Material_2.SetNormalMap("textures/woodNormalMap.png", NORMAL_MAP);
	sphere_Material_2.SetRoughness("textures/woodRoughness.png");
	sphere_Material_2.SetMetallic("textures/woodMetallic.png");
	sphere_Material_2.Init();


	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < 10; j++) {
			auto sphere = registry.create();
			Transform sphere_transform;
			sphere_transform.Translate({-18.0f + 4 * i, 1.0f, -18.0f + 4 * j});
			registry.assign<Transform>(sphere, sphere_transform);
					
			if (j % 2 == 0) {
				Mesh sphere_mesh{{sphereGeo, &sphere_Material_1}};
				registry.assign<Mesh>(sphere, std::move(sphere_mesh));
			}
			else {
				Mesh sphere_mesh{{sphereGeo, &sphere_Material_2}};
				registry.assign<Mesh>(sphere, std::move(sphere_mesh));
			}
		}
	}
	

	MaterialPBR cube_Material;
	cube_Material.SetAlbedo("textures/5/d.png");
	cube_Material.SetRoughness("textures/5/r.png");
	cube_Material.SetNormalMap("textures/5/n.png", NORMAL_MAP);
	cube_Material.Init();

	Mesh cube_mesh { {cubeGeo, &cube_Material} };
	for (int i = 0; i < 9; i++) {
	 	Transform cube_transform;
	 	cube_transform.Scale({2.0f, 2.0f, 2.0f});
	 	cube_transform.Translate({-16.0f+4*i, 1.0f, 9.0f});
	 	auto cube = registry.create();
	 	registry.assign<Transform>(cube, cube_transform);
	 	registry.assign<Mesh>(cube, cube_mesh);
	}

	
	SpotLight sl({0.8f, 0.9f, 0.3f},{0.0f, 30.0f, 30.0f}, {0.0f, -1.0f, -1.0f}, 15.0f, 35.0f, 100.0f);
	auto spotlight = registry.create();
	registry.assign<SpotLight>(spotlight, sl);
	
	for (int i = 0; i < 10; i++) {
	 	for (int j = 0; j < 10; j++) {
			float r = get_random();
	 		float g = get_random();
	 		float b = get_random();
	 		auto pointLight = registry.create();
	 		PointLight pl({r,g,b}, {-18.0f+4*i, 2.0f, -18+4*j}, 100.0f);
	 		registry.assign<PointLight>(pointLight, pl);
	 	}
	}
	

	PerspectiveCamera camera; 
	camera.SetAspect((float)window.GetWidth()/(float)window.GetHeight());
	camera.SetProjection(45.0f, camera.GetAspect(), 0.1f, 100.0f);
	camera.SetPosition(glm::vec3(7.0f, 10.0f, 20.0f));
	camera.SetPitch(-20.0f);

	renderer.SetActiveCamera(&camera);
	
	double currentTime = 0.0;
	double lastTime = 0.0;
	
	// main loop
	while (!glfwWindowShouldClose(window.GetPointer())) {
		glfwPollEvents(); 

		currentTime = glfwGetTime();
		float deltaTime = currentTime - lastTime;
		lastTime = currentTime;
		camera.Update(deltaTime);
		renderer.Update(registry);
		gui.Update();
    	
 		glfwSwapBuffers(window.GetPointer());
	}	

	return 0;
}