#include <iostream>
#include <string>

#include "renderer.h"
#include "camera.h"
#include "mesh.h"
#include "texture.h"
#include "light.h"

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

	ShaderProgram pbr_program;
	pbr_program[GL_VERTEX_SHADER] = "glsl/pbr_vertex.glsl";
	pbr_program[GL_FRAGMENT_SHADER] = "glsl/pbr_pixel.glsl";
	pbr_program.Compile();
	pbr_program.Link();

	//DirectLight l1(&program, {0.5, 0.5, 0.5}, {0.5, 0.5, 0.5}, {0.5, 0.5, 0.5}, {0.0f, -1.0f, 0.0f});
	//l1.SetInnerUniforms();

	//PointLight l2(&program, {1.0, 1.0, 1.0}, {1.0, 1.0, 1.0}, {1.0, 1.0, 1.0}, {20.0f, 10.0f, 0.0f});
	//l2.setAttenuation(1.0f, 0.02f, 0.001f);
 	//l2.SetInnerUniforms();

	//SpotLight l3(&program, {0.5, 0.5, 0.5}, {0.5, 0.5, 0.5}, {0.5, 0.5, 0.5}, {0.0f, 30.0f, 0.0f}, {0.0f, -1.0f, 0.0f}, 30.0f);
	//l3.setAttenuation(15.0f);
	//l3.SetInnerUniforms();

	Texture ambient;
	ambient.Load("textures/brickAO.png");
	ambient.Init();

	Texture diffuse;
	diffuse.Load("textures/brickAlbedo.png");
	diffuse.Init();

	Texture specular;
	specular.Load("textures/brickMetallic.png");
	specular.Init();

	Texture normals;
	normals.Load("textures/brickNormalMap.png");
	normals.Init();

	//////////////////////////////////////////////

	Texture albedo;
	albedo.Load("textures/brickAlbedo.png");
	albedo.Init();

	Texture normalPBR;
	normalPBR.Load("textures/brickNormalMap.png");
	normalPBR.Init();

	Texture metallic;
	metallic.Load("textures/brickMetallic.png");
	metallic.Init();

	Texture roughness;
	roughness.Load("textures/brickRoughness.png");
	roughness.Init();

	Texture ao;
	ao.Load("textures/brickAO.png");
	ao.Init();

	////////////////////////////////////////////////////////////////////
	
	Texture wood_albedo;
	wood_albedo.Load("textures/woodAlbedo.png");
	wood_albedo.Init();

	Texture wood_normalPBR;
	wood_normalPBR.Load("textures/woodNormalMap.png");
	wood_normalPBR.Init();

	Texture wood_metallic;
	wood_metallic.Load("textures/woodMetallic.png");
	wood_metallic.Init();

	Texture wood_roughness;
	wood_roughness.Load("textures/woodRoughness.png");
	wood_roughness.Init();

	Texture wood_ao;
	wood_ao.Load("textures/woodAO.png");
	wood_ao.Init();


	PhongTextureMaterial wood(&phong_text_program, &ambient, &diffuse, &specular, &normals, 0.4);
	PhongMaterial emerald(&program, {0.0215, 0.1745, 0.0215}, {0.07568, 0.61424, 0.07568}, {0.633, 0.727811, 0.633}, 0.6f);
	PbrMaterial brick(&pbr_program, &albedo, &normalPBR, &metallic, &roughness, &ao);
	//PbrMaterial brick(&pbr_program, &diffuse, &normals, &specular, &roughness, &ambient);
	PbrMaterial woodPBR(&pbr_program, &wood_albedo, &wood_normalPBR, &wood_metallic, &wood_roughness, &wood_ao);


	Geometry dragonGeometry;
	dragonGeometry.Load("data/dragon.obj");  //добавить вывод ошибки, если файл не найден

	Geometry cubeGeometry;
	cubeGeometry.Load("data/cube.obj");

	Geometry sphereGeometry;
	sphereGeometry.Load("data/sphere.obj");

	Mesh dragon = { {&dragonGeometry, &emerald} };

	//Mesh cube = { {&cubeGeometry, &wood} };
	//Mesh cube = { {&cubeGeometry, &emerald} };
	Mesh cube = { {&cubeGeometry, &brick} };
	//Mesh cube = { {&cubeGeometry, &woodPBR} };
	cube.SetModelMatrix(glm::scale(glm::mat4(1.0f), glm::vec3(15.0f, 0.15f, 15.0f)));

	//Mesh sphere = { {&sphereGeometry, &wood} };
	Mesh sphere = { {&sphereGeometry, &woodPBR} };
	//Mesh sphere = { {&sphereGeometry, &brick} };
	sphere.SetModelMatrix(glm::translate(glm::mat4(1.0f), glm::vec3(-10.0f, 10.0f, 0.0f))); 	 	  
	//sphere.SetModelMatrix(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 10.0f, 0.0f))); 	 	  

//  исправить под эту версию прог
//  OrthoCamera camera; // (левая, правая, нижняя, верхняя, ближняя, задняя стенки)
//  camera.SetAspect((float)window.GetWidth()/(float)window.GetHeight());
//  camera.SetProjection(-20.0f*camera.GetAspect(), 20.0f*camera.GetAspect(), -20.0f, 20.0f, 0.1f, 100.0f);

//	glm::mat4 projection = camera.GetProjectionMatrix();
//	glm::mat4 model = glm::mat4(1.0f);
//	glm::mat4 model2 = glm::mat4(1.0f);
//	model2 = glm::translate(model2, glm::vec3(0.0f, 0.0f, -50.0f)); // сдвиг на вектор 
//	//model2 = glm::rotate(model2, 90.0f, glm::vec3(0.0, 0.0, 1.0)); // поворот на 90 градусов вдоль Оz
//	//model2 = glm::scale(model2, glm::vec3(0.5, 0.5, 0.5));  // масштабирование

	PerspectiveCamera camera; // (угол раствора камеры, ширина области просмотра/на высоту, ближняя и дальняя стенки)
	camera.SetAspect((float)window.GetWidth()/(float)window.GetHeight());
	camera.SetProjection(45.0f, camera.GetAspect(), 0.1f, 100.0f);
	camera.SetPosition(glm::vec3(0.0f, 10.0f, 20.0f));
	camera.SetPitch(-20.0f);

	renderer.SetActiveCamera(&camera);
 	renderer.AddMesh(&dragon);
	renderer.AddMesh(&cube);
	renderer.AddMesh(&sphere);

	double currentTime = 0.0;
	double lastTime = 0.0;
	 
	// игровой цикл
	while (!glfwWindowShouldClose(window.GetPointer())) {
		//calculate time
		currentTime = glfwGetTime();
		float deltaTime = currentTime - lastTime;
		lastTime = currentTime;
		camera.Update(deltaTime);
		//sphere.SetModelMatrix(glm::rotate(sphere.GetModelMatrix(), deltaTime*1.0f, glm::vec3(0.0f, 1.0f, 0.0f)));
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