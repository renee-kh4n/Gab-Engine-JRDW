#include "GDCamera.h"

GDCamera::GDCamera(GDWindow* mWindow, GDShader* mShader)
{
    this->nearClip = 0.1f;
    this->farClip = 100.0f;
	this->mWindow = mWindow;
    this->mShader = mShader;
}

glm::vec3 GDCamera::CamUp()
{
    return glm::cross(CamR(), CamF);
}

glm::vec3 GDCamera::CamR()
{
    return glm::normalize(glm::cross(CamF, WorldUp));
}

void GDCamera::RotateCam(float degree, glm::vec3 axis)
{
    CamF = glm::vec3(glm::rotate(glm::translate(glm::mat4(1), CamF), glm::radians(degree), axis) * glm::vec4(CamF, 0));
}

GDPerspectiveCamera::GDPerspectiveCamera(GDWindow* mWindow, GDShader* mShader) : GDCamera(mWindow, mShader)
{
}

glm::mat4 GDPerspectiveCamera::getproj()
{
    return glm::perspective(glm::radians(this->angles), (float)mWindow->win_y / mWindow->win_x, this->nearClip, this->farClip);
}

GDOrthographicCamera::GDOrthographicCamera(GDWindow* mWindow, GDShader* mShader) : GDCamera(mWindow, mShader)
{
}

glm::mat4 GDOrthographicCamera::getproj()
{
    return glm::ortho(-orthoRange, orthoRange, -orthoRange, orthoRange, this->nearClip, this->farClip);
}
