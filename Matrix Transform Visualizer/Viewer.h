#pragma once
#include "helpfile.h"
#include "Object.h"
class Viewer : public Object
{
private:
    glm::vec3 targetPosition = glm::vec3(0.0);
    glm::vec3 up = glm::vec3(0.0, 1.0, 0.0);
    float distanceToTarget = 3.0f;
    glm::mat4 projection = glm::mat4(1.0f);
    glm::mat4 view = glm::mat4(1.0f);
    float fov = 45.0f;
    float pitch = 17.0f;
    float yaw = 60.0f;
    float near = 0.1f;
    float far = 100.0f;
    float aspect = width / height;
#pragma region Properties Funcs
public:
    const float& GetFov() { return fov; }
    void SetFov(const float& value) { fov = ClampFloat(value, 0.0f, 90.0f); }

    const float& GetPitch() { return pitch; }
    void SetPitch(const float& value) { pitch = ClampFloat(value, -180.0f, 180.0f); }

    const float& GetYaw() { return yaw; }
    void SetYaw(const float& value) { yaw = ClampFloat(value, -180.0f, 180.0f); }

    const float& GetDistanceToTarget() { return distanceToTarget; }
    void SetDistanceToTarget(const float& value) { distanceToTarget = ClampFloat(value, 0.5f, 50.0f); }

    const float& GetNear() { return near; }
    void SetNear(const float& value) { near = ClampFloat(value, 0.1f, 10.0f); }

    const float& GetFar() { return far; }
    void SetFar(const float& value) { far = ClampFloat(value, 0.1f, 500.0f); }

    const float& GetAspect() { return aspect; }
    void SetAspect(const float& value) { aspect = ClampFloat(value, 0.0f, 4.0f); }

    const glm::vec3& GetPosition() { return position; }
    void SetPosition(const glm::vec3& value) { position = ClampVec3(value, -10, 10); }

    const glm::vec3& GetTargetPosition() { return targetPosition; }
    void SetTargetPosition(const glm::vec3& value) { targetPosition = ClampVec3(value, -10, 10); }

    const glm::mat4& GetView() { return view; }
    const glm::mat4& GetProjection() { return projection; }
#pragma endregion
#pragma region Properties
    __declspec(property(get = GetFov, put = SetFov))                           float     Fov;
    __declspec(property(get = GetPitch, put = SetPitch))                       float     Pitch;
    __declspec(property(get = GetYaw, put = SetYaw))                           float     Yaw;
    __declspec(property(get = GetDistanceToTarget, put = SetDistanceToTarget)) float     DistanceToTarget;
    __declspec(property(get = GetNear, put = SetNear))                         float     Near;
    __declspec(property(get = GetFar, put = SetFar))                           float     Far;
    __declspec(property(get = GetAspect, put = SetAspect))                     float     Aspect;
    __declspec(property(get = GetPosition, put = SetPosition))                 glm::vec3 Position;
    __declspec(property(get = GetTargetPosition, put = SetTargetPosition))     glm::vec3 TargetPosition;
    __declspec(property(get = GetView))                                        glm::mat4 View;
    __declspec(property(get = GetProjection))                                  glm::mat4 Projection;
#pragma endregion

    void Update() override
    {
        position = { distanceToTarget * cos(glm::radians(pitch)) * cos(glm::radians(yaw)),
                     distanceToTarget * sin(glm::radians(pitch)),
                     distanceToTarget * cos(glm::radians(pitch)) * sin(glm::radians(yaw)) };
        view = glm::lookAt(position, targetPosition, up);
        projection = glm::perspective(fov, aspect, near, far);
    }

    void ResetSpherical()
    {
        distanceToTarget = 3.0f;
        pitch = 17.0f;
        yaw = 60.0f;
    }

    void ResetProjection()
    {
        aspect = width / height;
        fov = 45.0f;
        near = 0.1;
        far = 100.f;
    }
};