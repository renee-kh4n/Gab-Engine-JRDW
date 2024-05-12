#pragma once

#include "util.h"
#include "Window.h"
#include "Shader.h"

namespace gde {
    /// <summary>
    /// Generic camera class with assignable project matrix and utility functions for rendering.
    /// </summary>
    struct Camera {
        float nearClip;
        float farClip;

        virtual glm::mat4 getproj() = 0;
        Window* mWindow;
        Shader* mShader;

        Camera(Window* mWindow, Shader* mShader);

        glm::vec3 cameraPos = glm::vec3(0, 0, 12.0f);
        glm::vec3 WorldUp = glm::vec3(0, 1, 0);

        glm::vec3 CamF = glm::vec3(0, 0, -1);
        glm::vec3 CamUp();
        glm::vec3 CamR();

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