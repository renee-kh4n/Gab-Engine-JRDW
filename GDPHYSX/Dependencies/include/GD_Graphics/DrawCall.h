#pragma once

#include "Texture.h"
#include "Material.h"
#include "Mesh.h"


namespace gde {
    class DrawCall {
    public:
        glm::vec3 pos = glm::vec3(0, 0, 0);
        glm::vec3 scale = glm::vec3(1);
        glm::vec3 rot = glm::vec3(0);

        Mesh* m_mesh;
        Material* m_material;

        DrawCall(Mesh* mesh, Material* material, glm::vec3 pos = glm::vec3(0, 0, 0), glm::vec3 scale = glm::vec3(1, 1, 1), glm::vec3 rot = glm::vec3(0, 0, 0));
    };
}