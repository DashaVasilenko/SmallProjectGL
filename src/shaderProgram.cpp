#include "shaderProgram.h"
#include <iostream>
#include <fstream>
#include <streambuf>

void ShaderProgram::Load(const std::string& fileName) {
    std::ifstream input(fileName);
    json shaderData;
    input >> shaderData;

    name = fileName;
    bool flag = false;

    // разбираем json файл
    if (shaderData.find("vertex") != shaderData.end()) {
        mapSources[GL_VERTEX_SHADER] = shaderData["vertex"];
        flag = true;
    }
    
    if (shaderData.find("fragment") != shaderData.end()) {
        mapSources[GL_FRAGMENT_SHADER] = shaderData["fragment"];
    }

    if (shaderData.find("tesselation_evaluation") != shaderData.end()) {
        mapSources[GL_TESS_EVALUATION_SHADER] = shaderData["tesselation_evaluation"];
    }

    if (shaderData.find("tesselation_control") != shaderData.end()) {
        mapSources[GL_TESS_CONTROL_SHADER] = shaderData["tesselation_control"];
    }

    if (shaderData.find("geometry") != shaderData.end()) {
        mapSources[GL_GEOMETRY_SHADER] = shaderData["geometry"];
    }

    // разбираемся с дефайнами до компиляции
    auto it = shaderData.find("defines");
    if (it != shaderData.end()) {
        for (json::iterator iter = it->begin(); iter != it->end(); ++iter) {
            defines_source += "#define " + iter->get<std::string>() + std::string("\n");
        }
    }

    if (flag) {
        Compile();
        Link();
        DeleteShaders();
    }
}

void ShaderProgram::Compile() {
    for (auto& element: mapSources) {
        std::ifstream t(element.second);
	    std::string source_cpp;
	    t.seekg(0, std::ios::end);   
	    source_cpp.reserve(t.tellg());
	    t.seekg(0, std::ios::beg);
	    source_cpp.assign((std::istreambuf_iterator<char>(t)), std::istreambuf_iterator<char>());

        GLCall(mapShaders[element.first] = glCreateShader(element.first)); // создать объект шейдера (в скобках тип шейдера)
        GLuint& shader_descriptor = mapShaders[element.first];
	    
	   
        // TODO:: Убрать хардкод версии шейдера - перенести версию в json файл
        const char *sources[3] = { "#version 330 core\n", defines_source.c_str(), source_cpp.c_str() };
	    GLCall(glShaderSource(shader_descriptor, 3, sources, NULL));  
	    GLCall(glCompileShader(shader_descriptor)); // компилируем шейдер

	    // проверка на ошибки при сборке шейдера
	    GLint success;
	    GLCall(glGetShaderiv(shader_descriptor, GL_COMPILE_STATUS, &success));
	    if(!success) {
            GLint logLen;
            GLCall(glGetShaderiv(shader_descriptor, GL_INFO_LOG_LENGTH, &logLen));
            if (logLen > 0) {
                char *infoLog = new char[logLen];
                GLCall(glGetShaderInfoLog(shader_descriptor, logLen, NULL, infoLog));
                std::cout << "ERROR::SHADER::COMPILATION_FAILED::" << name << "\n" << infoLog << std::endl;
                delete[] infoLog;
            }
	    }
    }
}

void ShaderProgram::Link() {
	GLCall(descriptor = glCreateProgram()); // создание программы 

    for (auto& element: mapShaders) {
        GLCall(glAttachShader(descriptor, element.second)); // присоединяем собранные шейдеры к программе
    }
	GLCall(glLinkProgram(descriptor)); // связываем

	// проверка на ошибки при связывании
	GLint success;
	GLCall(glGetProgramiv(descriptor, GL_LINK_STATUS, &success));
	if (!success) {
        GLint logLen;
        GLCall(glGetProgramiv(descriptor, GL_INFO_LOG_LENGTH, &logLen));
        if (logLen > 0) {
            char *infoLog = new char[logLen];
		    GLCall(glGetProgramInfoLog(descriptor, logLen, NULL, infoLog));
            std::cout << "ERROR::SHADER::LINKING_FAILED::" << name << "\n"<< infoLog << std::endl;
            delete[] infoLog;
        }
	}
}

void ShaderProgram::Run() const {
    GLCall(glUseProgram(descriptor)); // использование созданной программы
}

void ShaderProgram::SetUniform(const std::string& name, const glm::mat4& matrix) {
    // получить индекс формы (переменная программы-шейдера, название формы, определенной внутри этой программы)
    GLint location = -1;
    if (uniformCache.find(name) != uniformCache.end()) {
        location = uniformCache[name];
    }
    else {
        GLCall(location = glGetUniformLocation(descriptor, name.c_str())); 
        uniformCache[name] = location;
    }
     
    if (location != -1) {
        GLCall(glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix)));
    }
    else {
        std::cerr << "Uniform  " << name << " not found" << std::endl;
    }
}

void ShaderProgram::SetUniform(const std::string& name, const glm::mat3& matrix) {
    GLint location = -1;
    if (uniformCache.find(name) != uniformCache.end()) {
        location = uniformCache[name];
    }
    else {
        GLCall(location = glGetUniformLocation(descriptor, name.c_str())); 
        uniformCache[name] = location;
    }
     
    if (location != -1) {
        GLCall(glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(matrix))); // установка значения формы
    }
    else {
        std::cerr << "Uniform  " << name << " not found" << std::endl;
    }
}

void ShaderProgram::SetUniform(const std::string& name, const glm::vec4& vector) {
    GLint location = -1;
    if (uniformCache.find(name) != uniformCache.end()) {
        location = uniformCache[name];
    }
    else {
        GLCall(location = glGetUniformLocation(descriptor, name.c_str())); 
        uniformCache[name] = location;
    }

    if (location != -1) {
        GLCall(glUniform4f(location, vector.x, vector.y, vector.z, vector.w));
    }
    else {
        std::cerr << "Uniform  " << name << " not found" << std::endl;
    }
}

void ShaderProgram::SetUniform(const std::string& name, const glm::vec3& vector) {
    GLint location = -1;
    if (uniformCache.find(name) != uniformCache.end()) {
        location = uniformCache[name];
    }
    else {
        GLCall(location = glGetUniformLocation(descriptor, name.c_str())); 
        uniformCache[name] = location;
    }

    if (location != -1) {
        glUniform3f(location, vector.x, vector.y, vector.z); //!!!!!!!!!!!!!!! тут GLCall вылетает с ошибкой
    }
    else {
        std::cerr << "Uniform  " << name << " not found" << std::endl;
    }
}

void ShaderProgram::SetUniform(const std::string& name, const glm::vec2& vector) {
    GLint location = -1;
    if (uniformCache.find(name) != uniformCache.end()) {
        location = uniformCache[name];
    }
    else {
        GLCall(location = glGetUniformLocation(descriptor, name.c_str())); 
        uniformCache[name] = location;
    }
    
    if (location != -1) {
        GLCall(glUniform2f(location, vector.x, vector.y));
    }
    else {
        std::cerr << "Uniform  " << name << " not found" << std::endl;
    }
}

void ShaderProgram::SetUniform(const std::string& name, float value) {
    GLint location = -1;
    if (uniformCache.find(name) != uniformCache.end()) {
        location = uniformCache[name];
    }
    else {
        GLCall(location = glGetUniformLocation(descriptor, name.c_str())); 
        uniformCache[name] = location;
    }

    if (location != -1) {
        glUniform1f(location, value); //!!!!!!!!!!!!! GLCall вылетает с ошибкой
    }
    else {
        std::cerr << "Uniform  " << name << " not found" << std::endl;
    }
}

void ShaderProgram::SetUniform(const std::string& name, int value) {
    GLint location = -1;
    if (uniformCache.find(name) != uniformCache.end()) {
        location = uniformCache[name];
    }
    else {
        GLCall(location = glGetUniformLocation(descriptor, name.c_str())); 
        uniformCache[name] = location;
    }
   
    if (location != -1) {
        GLCall(glUniform1i(location, value));
    }
    else {
        std::cerr << "Uniform  " << name << " not found" << std::endl;
    }
}

void ShaderProgram::DeleteShaders() {
    for (auto& element: mapShaders) {
	    GLCall(glDeleteShader(element.second));
    }
}

ShaderProgram::~ShaderProgram(){
    GLCall(glDeleteProgram(descriptor));
}