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
        return glm::cross(CamR(), (glm::vec3)this->Forward());
    }

    glm::vec3 Camera::CamR()
    {
        return glm::normalize(glm::cross((glm::vec3)this->Forward(), WorldUp));
    }

    glm::mat4 Camera::GetViewMat()
    {
        auto this_pos = (glm::vec3)this->World()->position;
        return glm::lookAt(this_pos, (glm::vec3)this->Forward() + this_pos, this->WorldUp);
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