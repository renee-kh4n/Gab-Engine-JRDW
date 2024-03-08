#pragma once

#include "GDutil.h"
#include "GDWindow.h"

struct GDCamera {
    glm::mat4 proj_ortho;
    glm::mat4 proj_pers;
    float curFOV = 90.0f;
    GDWindow* mWindow;

    GDCamera(GDWindow* mWindow);

    void setPerspectiveFOV(float angles);
    glm::vec3 cameraPos = glm::vec3(0, 0, 12.0f);
    glm::vec3 WorldUp = glm::vec3(0, 1, 0);

    glm::vec3 CamF = glm::vec3(0, 0, -1);
    glm::vec3 CamUp();
    glm::vec3 CamR();

    void RotateCam(float degree, glm::vec3 axis);
};