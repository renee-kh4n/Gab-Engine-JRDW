#include "GDObject.h"

GDObject::GDObject(std::string path) {
    bool success = tinyobj::LoadObj(&attributes, &shapes, &material, &warning, &error, path.c_str());
    for (size_t m_i = 0; m_i < shapes[0].mesh.indices.size(); m_i++)
    {
        tinyobj::index_t vData = shapes[0].mesh.indices[m_i];

        fullVertexData.push_back(attributes.vertices[(vData.vertex_index * 3)]);
        fullVertexData.push_back(attributes.vertices[(vData.vertex_index * 3) + 1]);
        fullVertexData.push_back(attributes.vertices[(vData.vertex_index * 3) + 2]);

        if (attributes.normals.size() > 0) {
            fullVertexData.push_back(attributes.normals[(vData.normal_index * 3)]);
            fullVertexData.push_back(attributes.normals[(vData.normal_index * 3) + 1]);
            fullVertexData.push_back(attributes.normals[(vData.normal_index * 3) + 2]);
        }
        else {
            fullVertexData.push_back(0);
            fullVertexData.push_back(0);
            fullVertexData.push_back(0);
        }

        if (attributes.texcoords.size() > 0) {
            fullVertexData.push_back(attributes.texcoords[(vData.texcoord_index * 2)]);
            fullVertexData.push_back(attributes.texcoords[(vData.texcoord_index * 2) + 1]);
        }
        else {
            fullVertexData.push_back(0.5f);
            fullVertexData.push_back(0.5f);
        }
    }

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * fullVertexData.size(), fullVertexData.data(), GL_DYNAMIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    GLintptr normalPtr = 3 * sizeof(float);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)normalPtr);
    glEnableVertexAttribArray(1);

    GLintptr uvPtr = 6 * sizeof(float);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)uvPtr);
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}