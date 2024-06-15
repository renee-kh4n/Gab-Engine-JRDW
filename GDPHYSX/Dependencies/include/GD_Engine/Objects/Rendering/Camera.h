#pragma once

#include <GD_Graphics/util.h>
#include <GD_Graphics/Window.h>
#include <GD_Graphics/Shader.h>

#include "../../Objects/Object.h"

namespace gde {
    /// <summary>
    /// Generic camera class with assignable project matrix and utility functions for rendering.
    /// </summary>
    struct Camera : Object {
        float nearClip;
        float farClip;

        Window* mWindow;
        Shader* mShader;

        Camera(Window* mWindow, Shader* mShader);

        glm::vec3 WorldUp = glm::vec3(0, 1, 0);

        glm::vec3 CamF = glm::vec3(0, 0, -1);
        glm::vec3 CamUp();
        glm::vec3 CamR();
        glm::mat4 GetViewMat();
        virtual glm::mat4 getproj() = 0;

        void RotateCam(float degree, glm::vec3 axis);
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