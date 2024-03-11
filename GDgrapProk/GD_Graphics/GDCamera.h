#pragma once

#include "GDutil.h"
#include "GDWindow.h"
#include "GDShader.h"

struct GDCamera {
    glm::mat4 proj;
    float curFOV = 90.0f;
    GDWindow* mWindow;
    GDShader* mShader;

    GDCamera(GDWindow* mWindow, GDShader* mShader);

    void setPerspectiveFOV(float angles, float range);
    glm::vec3 cameraPos = glm::vec3(0, 0, 12.0f);
    glm::vec3 WorldUp = glm::vec3(0, 1, 0);

    glm::vec3 CamF = glm::vec3(0, 0, -1);
    glm::vec3 CamUp();
    glm::vec3 CamR();

    void RotateCam(float degree, glm::vec3 axis);
};