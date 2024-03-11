#include "GDCamera.h"

GDCamera::GDCamera(GDWindow* mWindow, GDShader* mShader)
{
	this->mWindow = mWindow;
    this->mShader = mShader;
}

void GDCamera::setPerspectiveFOV(float angles, float range)
{
    curFOV = angles;
    proj = glm::perspective(glm::radians(angles), (float)mWindow->win_y / mWindow->win_x, 0.1f, range);
}

glm::vec3 GDCamera::CamUp()
{
    return glm::cross(CamR(), CamF);
}

glm::vec3 GDCamera::CamR()
{
    return glm::cross(CamF, WorldUp);
}

void GDCamera::RotateCam(float degree, glm::vec3 axis)
{
    CamF = glm::vec3(glm::rotate(glm::translate(glm::mat4(1), CamF), glm::radians(degree), axis) * glm::vec4(CamF, 0));
}
