#include "Camera.h"

namespace gde {
    using namespace rendering;

    Camera::Camera(Window* mWindow, Shader* mShader)
    {
        this->nearClip = 0.1f;
        this->farClip = 100.0f;
        this->mWindow = mWindow;
        this->mShader = mShader;
    }

    glm::mat4 Camera::GetViewMat()
    {
        auto world = this->World();

        auto this_pos = (glm::vec3)world->position;
        return glm::lookAt(this_pos, (glm::vec3)world->Forward + this_pos, (glm::vec3)world->Up);
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
        return glm::ortho(-orthoRange, orthoRange, -orthoRange, orthoRange, this->nearClip, this->farClip);
    }

}