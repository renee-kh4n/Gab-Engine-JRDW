#include "Camera.h"

namespace gde {
    Camera::Camera(Window* mWindow, Shader* mShader)
    {
        this->nearClip = 0.1f;
        this->farClip = 100.0f;
        this->mWindow = mWindow;
        this->mShader = mShader;
    }

    glm::vec3 Camera::CamUp()
    {
        return glm::cross(CamR(), CamF);
    }

    glm::vec3 Camera::CamR()
    {
        return glm::normalize(glm::cross(CamF, WorldUp));
    }

    void Camera::RotateCam(float degree, glm::vec3 axis)
    {
        CamF = glm::vec3(glm::rotate(glm::translate(glm::mat4(1), CamF), glm::radians(degree), axis) * glm::vec4(CamF, 0));
    }

    PerspectiveCamera::PerspectiveCamera(Window* mWindow, Shader* mShader) : Camera(mWindow, mShader)
    {
    }

    glm::mat4 PerspectiveCamera::getproj()
    {
        return glm::perspective(glm::radians(this->angles), (float)mWindow->win_y / mWindow->win_x, this->nearClip, this->farClip);
    }

    OrthographicCamera::OrthographicCamera(Window* mWindow, Shader* mShader) : Camera(mWindow, mShader)
    {
    }

    glm::mat4 OrthographicCamera::getproj()
    {
        return glm::ortho(orthoRange, -orthoRange, -orthoRange, orthoRange, this->nearClip, this->farClip);
    }

}