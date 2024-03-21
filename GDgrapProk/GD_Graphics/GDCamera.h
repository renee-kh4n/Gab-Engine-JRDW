#pragma once

#include "GDutil.h"
#include "GDWindow.h"
#include "GDShader.h"

/// <summary>
/// Generic camera class with assignable project matrix and utility functions for rendering.
/// </summary>
struct GDCamera {
    float nearClip;
    float farClip;

    virtual glm::mat4 getproj() = 0;
    GDWindow* mWindow;
    GDShader* mShader;

    GDCamera(GDWindow* mWindow, GDShader* mShader);
    
    glm::vec3 cameraPos = glm::vec3(0, 0, 12.0f);
    glm::vec3 WorldUp = glm::vec3(0, 1, 0);

    glm::vec3 CamF = glm::vec3(0, 0, -1);
    glm::vec3 CamUp();
    glm::vec3 CamR();

    void RotateCam(float degree, glm::vec3 axis);
};

struct GDOrthographicCamera : public GDCamera {
    float orthoRange;
    
    GDOrthographicCamera(GDWindow* mWindow, GDShader* mShader);

    // Inherited via GDCamera
    virtual glm::mat4 getproj() override;
};

struct GDPerspectiveCamera : public GDCamera {
    float angles;

    GDPerspectiveCamera(GDWindow* mWindow, GDShader* mShader);

    // Inherited via GDCamera
    virtual glm::mat4 getproj() override;
};