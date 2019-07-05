#ifndef __CAMERA__
#define __CAMERA__

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>

class Camera {
public:
    Camera(const glm::vec3& = glm::vec3(0.0f, 0.0f, 3.0f) );
    void Update(const float&);
    inline glm::mat4 GetViewMatrix() { return glm::lookAt(position, position + front, up); }
    inline glm::vec3 GetPosition() { return position; }
protected:
  //  Camera(const glm::vec3& = glm::vec3(0.0f, 0.0f, 3.0f) );
private:
    void UpdatePosition(const float&);
    void UpdateVectors();

    glm::vec3 up;
    glm::vec3 right;
    glm::vec3 front;

    float pitch = 0.0f; // тангаж (наклон вверх вниз)
    float roll = 0.0f;  // крен
    float yaw = -90.0f; // рыскание (поворот влево вправо)

    glm::vec3 position;
    float speed = 10.0f;
    float mouse_sense = 0.1f;
};

class OrthoCamera : public Camera {
public:
    void SetLeftPlane(float a) { leftPlane = a; }
    void SetRightPlane(float a) { rightPlane = a; }
    void SetBottomPlane(float a) { bottomPlane = a; }
    void SetTopPlane(float a) { topPlane = a; }
    void SetNearPlane(float a) { nearPlane = a; }
    void SetFarPlane(float a) { farPlane = a; }
    // создание ортографической камеры (левая, правая, нижняя, верхняя, ближняя, задняя стенки)
    glm::mat4 GetProjectionMatrix() { 
        return glm::ortho(leftPlane, rightPlane, bottomPlane, topPlane, nearPlane, farPlane);
     }
private:
    float leftPlane;
    float rightPlane;
    float bottomPlane; 
    float topPlane; 
    float nearPlane;
    float farPlane;
};

class PerspectiveCamera : public Camera {
public:
    void SetFieldOfView(float a) { fov = a; }
    void SetWidthToHeight(float a) { width_to_height = a; }
    void SetNearPlane(float a) { nearPlane = a; }
    void SetFarPlane(float a) { farPlane = a; }
    // проекционная камера (угол раствора камеры, ширина области просмотра/на высоту, ближняя и дальняя стенки)
    glm::mat4 GetProjectionMatrix() { 
        return glm::perspective(fov, width_to_height, nearPlane, farPlane);
    }
private:
    float fov;
    float width_to_height;
    float nearPlane;
    float farPlane;
 };

#endif /* End of __CAMERA__ */