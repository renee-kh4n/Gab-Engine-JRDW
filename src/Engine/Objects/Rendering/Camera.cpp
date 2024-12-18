#include "Camera.h"

namespace gbe {
    using namespace rendering;

    Camera::Camera(Window* mWindow, Shader* mShader)
    {
        this->nearClip = 0.1f;
        this->farClip = 100.0f;
        this->mWindow = mWindow;
        this->mShader = mShader;
    }

    Matrix4 Camera::GetViewMat()
    {
        auto world = this->World();

        auto this_pos = (glm::vec3)world.position.Get();
        return glm::lookAt(this_pos, (glm::vec3)world.GetForward() + this_pos, (glm::vec3)world.GetUp());
    }

    PerspectiveCamera::PerspectiveCamera(Window* mWindow, Shader* mShader) : Camera(mWindow, mShader)
    {
    }

    Matrix4 PerspectiveCamera::getproj()
    {
        return glm::perspective(glm::radians(this->angles), (float)mWindow->Get_dimentions().y / mWindow->Get_dimentions().x, this->nearClip, this->farClip);
    }

    OrthographicCamera::OrthographicCamera(Window* mWindow, Shader* mShader) : Camera(mWindow, mShader)
    {
    }

    Matrix4 OrthographicCamera::getproj()
    {
        return glm::ortho(-orthoRange, orthoRange, -orthoRange, orthoRange, this->nearClip, this->farClip);
    }

}