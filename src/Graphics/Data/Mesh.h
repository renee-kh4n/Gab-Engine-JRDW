#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>

namespace gbe {
    namespace gfx {
        struct Mesh {
            GLuint VAO, VBO;
            tinyobj::attrib_t attributes;
            std::vector<tinyobj::shape_t> shapes;
            std::vector<tinyobj::material_t> material;
            std::string warning, error;

            std::vector<std::vector<glm::vec3>> triangles;

            std::vector<GLfloat> fullVertexData;
            Mesh(std::string path);
        };
    }
}