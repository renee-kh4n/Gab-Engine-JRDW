#pragma once

#include "../../Objects/Object.h"

#include "../Window/Window.h"

#include "../Graphics/Shader.h"

namespace gde {
    using namespace rendering;

    /// <summary>
    /// Generic camera class with assignable project matrix and utility functions for rendering.
    /// </summary>
    struct Camera : Object {
        float nearClip;
        float farClip;

        Window* mWindow;
        Shader* mShader;

        Camera(Window* mWindow, Shader* mShader);

        Vector3 WorldUp = Vector3(0, 1, 0);

        glm::mat4 GetViewMat();
        virtual glm::mat4 getproj() = 0;
    };

    struct OrthographicCamera : public Camera {
        float orthoRange;

        OrthographicCamera(Window* mWindow, Shader* mShader);

        // Inherited via GDCamera
        virtual glm::mat4 getproj() override;
    };

    struct PerspectiveCamera : public Camera {
        float angles;

        PerspectiveCamera(Window* mWindow, Shader* mShader);

        // Inherited via GDCamera
        virtual glm::mat4 getproj() override;
    };
}