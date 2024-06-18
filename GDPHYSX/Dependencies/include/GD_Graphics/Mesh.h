#pragma once

#include "tiny_obj_loader.h"
#include "util.h"

namespace gde {
    namespace rendering {
        struct Mesh {
            GLuint VAO, VBO;
            tinyobj::attrib_t attributes;
            std::vector<tinyobj::shape_t> shapes;
            std::vector<tinyobj::material_t> material;
            std::string warning, error;

            std::vector<GLfloat> fullVertexData;
            Mesh(std::string path);
        };
    }
}