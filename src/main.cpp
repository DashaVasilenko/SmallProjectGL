#include <iostream>
#include <string>
#include <fstream>
#include <streambuf>
#include "renderer.h"

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

//////////////////////////////////////////////////////////////////////////////////
	std::ifstream t("glsl/vertex.glsl");
	std::string str;
	t.seekg(0, std::ios::end);   
	str.reserve(t.tellg());
	t.seekg(0, std::ios::beg);
	str.assign((std::istreambuf_iterator<char>(t)), std::istreambuf_iterator<char>());

	GLuint vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	const char* vertexSource =  str.c_str();
	glShaderSource(vertexShader, 1, &vertexSource, NULL);
	glCompileShader(vertexShader);

	GLint success;
	GLchar infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

	if(!success) {
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
////////////////////////////////////////////////////////////////////////////////////////

	std::ifstream t2("glsl/fragment.glsl");
	std::string str2;
	t2.seekg(0, std::ios::end);   
	str2.reserve(t2.tellg());
	t2.seekg(0, std::ios::beg);
	str2.assign((std::istreambuf_iterator<char>(t2)), std::istreambuf_iterator<char>());

	GLuint fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	const char* fragmentSource =  str2.c_str();
	glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
	glCompileShader(fragmentShader);


	GLint success2;
	GLchar infoLog2[512];
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success2);

	if(!success2) {
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog2);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog2 << std::endl;
	}


	GLuint shaderProgram;
	shaderProgram = glCreateProgram();

	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);


	GLint success3;
	GLchar infoLog3[512];
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success3);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog3);
		std::cout << "ERROR::SHADER::LINKING_FAILED\n" << infoLog3 << std::endl;
	}



	//////////////////////////////////////////////////////////////////////////////////////////

	float vertices[] = {0.66, 0.13, 0.0, 1.0, 0.0, 0.0,
						0.33, 0.67, 0.0, 0.0, 1.0, 0.0,
						0.97, 0.97, 0.0, 0.0, 0.0, 1.0 };

	GLuint VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	GLuint VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
 
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3*sizeof(GLfloat)) );
	glEnableVertexAttribArray(1);



	while (!glfwWindowShouldClose(window.GetPointer())) {
		renderer.Update();

		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);


		glfwSwapBuffers(window.GetPointer());
    	glfwPollEvents();
	}

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	// delete program!
	window.Destroy();
	return 0;
}