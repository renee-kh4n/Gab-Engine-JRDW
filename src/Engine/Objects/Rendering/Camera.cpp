#include "Camera.h"
#include "Camera.h"

namespace gbe {
    using namespace gfx;

    Camera::Camera(Window* mWindow)
    {
        this->nearClip = 0.1f;
        this->farClip = 100.0f;
        this->mWindow = mWindow;
    }

    Matrix4 Camera::GetViewMat()
    {
        auto world = this->World();

        auto this_pos = (glm::vec3)world.position.Get();
        return glm::lookAt(this_pos, (glm::vec3)world.GetForward() + this_pos, (glm::vec3)world.GetUp());
    }

    Vector3 Camera::ScreenToRay(Vector2 normalizedscreenpos)
    {
        auto initialdir = Vector4(normalizedscreenpos.x, normalizedscreenpos.y, 1, 1) * glm::inverse(this->GetViewMat()) * glm::inverse(this->getproj());
        auto finaldir = Vector3(initialdir.x, initialdir.y, initialdir.z);

        return finaldir;
    }

    PerspectiveCamera::PerspectiveCamera(Window* mWindow) : Camera(mWindow)
    {
    }

    Matrix4 PerspectiveCamera::getproj()
    {
        return glm::perspective(glm::radians(this->angles), (float)mWindow->Get_dimentions().x / mWindow->Get_dimentions().y, this->nearClip, this->farClip);
    }

    OrthographicCamera::OrthographicCamera(Window* mWindow) : Camera(mWindow)
    {
    }

    Matrix4 OrthographicCamera::getproj()
    {
        return glm::ortho(-orthoRange, orthoRange, -orthoRange, orthoRange, this->nearClip, this->farClip);
    }

}