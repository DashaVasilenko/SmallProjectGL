#include "renderer.h"
#include <iostream>

void Renderer::SetWidth(int width) {
    this->width = width;
}

void Renderer::SetHeight(int height) {
    this->height = height;
}

void Renderer::SetFrameBuffer(GLuint descriptor) {
    glBindFramebuffer(GL_FRAMEBUFFER, descriptor);  // привязываем или отвязываем свой буфер кадр 
}

void Renderer::SetActiveCamera(const Camera* camera) {
    this->camera = camera;
    this->projection = camera->GetProjectionMatrix();
}
 
void Renderer::Init(const ShaderProgram* qprogram, const Geometry* qgeometry) {
	glViewport(0, 0, width, height); // позиция нижнего левого угла окна и размер области в окне, в котором рисуем
   
    //fbo.BufferInit(width, height); // создаем буфер кадра
    gbuffer.BufferInit(width, height);

    current_view_buffer = gbuffer.GetAlbedoDescriptor();

    quad_program = qprogram;
    quad_geometry = qgeometry;

    quad_program->Run();
    
    quad_program->SetUniform("ScreenWidthHeight", glm::vec2(width, height));
 
    quad_program->SetUniform("positionMap", 0);
    quad_program->SetUniform("normalMap", 1);
    quad_program->SetUniform("albedoMap", 2);
    quad_program->SetUniform("mraoMap", 3);
}

void Renderer::Update(entt::registry& registry) {
    // здесь нужно сказать что мы рисуем в текстуру  
    //fbo.Bind(); 
    gbuffer.Bind();

    glEnable(GL_DEPTH_TEST); // тест глубины

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // очищаем буферы
	glClearColor(0.0f, 0.0f, 1.0f, 1.0f); // буфер цвета очищаем синим цветом

    glm::mat4 viewMatrix = camera->GetViewMatrix();
    auto view = registry.view<Mesh, Transform>();
    for (auto entity: view) {
        auto& mesh = view.get<Mesh>(entity);
        auto& transform = view.get<Transform>(entity);
        mesh.Draw(projection, viewMatrix, transform.GetModelMatrix());
    }

    //fbo.Unbind();
    gbuffer.Unbind();

    glDisable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT); // очищаем буферы
	glClearColor(0.0f, 0.0f, 1.0f, 1.0f); // буфер цвета очищаем синим цветом
    
    quad_program->Run();
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::scale(model, glm::vec3(3.0, 3.0, 3.0));
    //glm::mat3 model3x3 = model;

    quad_program->SetUniform("View", viewMatrix);
    quad_program->SetUniform("Projection", projection);
    quad_program->SetUniform("Model", model);
    //quad_program->SetUniform("NormalMatrix", glm::transpose(glm::inverse(model3x3)));

    
    // Debug
    //glActiveTexture(GL_TEXTURE0);
    //glBindTexture(GL_TEXTURE_2D, current_view_buffer);
   
    
    //PBR SET PBR quad  
    quad_program->SetUniform("light_direction", glm::normalize(glm::vec3((camera->GetViewMatrix()*glm::vec4(0.0f, 1.0f, 0.0f, 0.0f)))));
    
    //glActiveTexture(GL_TEXTURE0);
    //glBindTexture(GL_TEXTURE_2D, fbo.GetTexDescriptor());

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, gbuffer.GetPositionDescriptor());

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, gbuffer.GetNormalDescriptor());

    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, gbuffer.GetAlbedoDescriptor());

    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D, gbuffer.GetMetallRoughAODescriptor());

    quad_geometry->Draw();
}

bool Renderer::WireFrame(const std::vector<std::string>& arguments) {
    if (arguments.size() == 1) {
        bool flag = std::stoi(arguments[0]);
        if (flag) {
            glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
        }
        else {
            glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
        }
        return true;
    }
    return false;
} 


bool Renderer::ViewBuffer(const std::vector<std::string>& arguments) {
    if (arguments.size() == 1) {
        if (arguments[0] == "normal") {
            current_view_buffer = gbuffer.GetNormalDescriptor();
            return true;
        }
        else if (arguments[0] == "position") {
            current_view_buffer = gbuffer.GetPositionDescriptor();
            return true;
        }
        else if (arguments[0] == "albedo") {
            current_view_buffer = gbuffer.GetAlbedoDescriptor();
            return true;
        }
        else if (arguments[0] == "mrao") {
            current_view_buffer = gbuffer.GetMetallRoughAODescriptor();
            return true;
        }
        return false;
    }
    return false;
}
