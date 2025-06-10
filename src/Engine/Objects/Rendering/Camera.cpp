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

    Vector2 Camera::WorldToScreen(Vector3 worldpos) {
        auto inv_proj = glm::inverse(this->getproj());
        auto inv_view = glm::inverse(this->GetViewMat());
        auto initial_pos = inv_proj * inv_view * Vector4(worldpos, 1.0f);
        auto final_pos = Vector3(initial_pos.x, -initial_pos.y, initial_pos.z) * (1.0f / initial_pos.w);

        //final_pos.x *= (0.5f / static_cast<float>(this->mWindow->Get_dimentions().x));
        //final_pos.y *= (0.5f / static_cast<float>(this->mWindow->Get_dimentions().y));

        return Vector2(final_pos);
    }

    Vector3 Camera::ScreenToRay(Vector2 normalizedscreenpos)
    {
        auto inv_proj = glm::inverse(this->getproj());
        auto inv_view = glm::inverse(this->GetViewMat());
        auto initialdir = inv_view * inv_proj * Vector4(normalizedscreenpos.x, normalizedscreenpos.y, 1, 1);
        Vector3 finaldir = Vector3(initialdir.x, initialdir.y, initialdir.z) * (1.0f / initialdir.w);
        finaldir -= this->World().position.Get();

        return finaldir.Normalize();
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