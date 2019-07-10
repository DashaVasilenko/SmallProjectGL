#ifndef __CAMERA__
#define __CAMERA__

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera {
public:
    void Update(const float&);
    inline glm::mat4 GetProjectionMatrix() const { return projection; }
    inline glm::mat4 GetViewMatrix() const { return glm::lookAt(position, position + front, up); }
    inline glm::vec3 GetPosition() const { return position; }
    inline void SetPosition(const glm::vec3& position) { this->position = position; }
    inline void SetYaw(float yaw) { this->yaw = yaw; }
    void SetPitch(float);
    void SetAspect(float a) { aspect = a; }
    float GetAspect() { return this->aspect; }
protected:
    Camera(const glm::vec3& = glm::vec3(0.0f, 0.0f, 3.0f) );
    glm::mat4 projection;
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
    float aspect; 
    float speed = 5.0f;
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
    // (левая, правая, нижняя, верхняя, ближняя, задняя стенки)
    void SetProjection (float left, float right, float bottom, float top, float near, float far) {
        leftPlane = left;
        rightPlane = right;
        bottomPlane = bottom; 
        topPlane = top; 
        nearPlane = near;
        farPlane = far;
        projection = glm::ortho(leftPlane, rightPlane, bottomPlane, topPlane, nearPlane, farPlane);
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
    // (угол раствора камеры, ширина области просмотра/на высоту, ближняя и дальняя стенки)
    void SetProjection(float f, float aspect, float near, float far) {
        fov = f;
        width_to_height = aspect;
        nearPlane = near;
        farPlane = far;
        projection = glm::perspective(fov, width_to_height, nearPlane, farPlane);
    }
private:
    float fov;
    float width_to_height;
    float nearPlane;
    float farPlane;
 };

#endif /* End of __CAMERA__ */