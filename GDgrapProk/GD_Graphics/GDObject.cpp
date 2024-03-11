#include "GDObject.h"

GDObject::GDObject(std::string path) {
    int totalVertexDataLength = 11;

    bool success = tinyobj::LoadObj(&attributes, &shapes, &material, &warning, &error, path.c_str());
    for (size_t m_i = 0; m_i < shapes[0].mesh.indices.size(); m_i += 3)
    {

        glm::vec3 vertices[3] = { glm::vec3(), glm::vec3(), glm::vec3() };
        glm::vec3 normal0;

        glm::vec2 uvs[3] = { glm::vec2(), glm::vec2(), glm::vec2() };

        for (size_t v_i = 0; v_i < 3; v_i++)
        {
            tinyobj::index_t vData = shapes[0].mesh.indices[m_i + v_i];

            vertices[v_i].x = attributes.vertices[(vData.vertex_index * 3)];
            vertices[v_i].y = attributes.vertices[(vData.vertex_index * 3) + 1];
            vertices[v_i].z = attributes.vertices[(vData.vertex_index * 3) + 2];

            fullVertexData.push_back(vertices[v_i].x);
            fullVertexData.push_back(vertices[v_i].y);
            fullVertexData.push_back(vertices[v_i].z);

            if (attributes.normals.size() > 0) {
                if (v_i == 0) {
                    normal0 = glm::vec3(
                        attributes.normals[(vData.normal_index * 3)],
                        attributes.normals[(vData.normal_index * 3) + 1],
                        attributes.normals[(vData.normal_index * 3) + 2]
                    );
                }

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
                uvs[v_i].x = attributes.texcoords[(vData.texcoord_index * 2)];
                uvs[v_i].y = attributes.texcoords[(vData.texcoord_index * 2) + 1];

                fullVertexData.push_back(uvs[v_i].x);
                fullVertexData.push_back(uvs[v_i].y);
            }
            else {
                fullVertexData.push_back(0.5f);
                fullVertexData.push_back(0.5f);
            }

            fullVertexData.push_back(0);
            fullVertexData.push_back(0);
            fullVertexData.push_back(0);
        }

        glm::vec3 deltaPos;
        if (vertices[0] == vertices[1])
            deltaPos = vertices[2] - vertices[0];
        else
            deltaPos = vertices[1] - vertices[0];

        glm::vec2 deltaUV1 = uvs[1] - uvs[0];
        glm::vec2 deltaUV2 = uvs[2] - uvs[0];

        glm::vec3 tan; // tangents
        glm::vec3 bin; // binormal

        // avoid divion with 0
        if (deltaUV1.s != 0)
            tan = deltaPos / deltaUV1.s;
        else
            tan = deltaPos / 1.0f;

        tan = glm::normalize(tan - glm::dot(normal0, tan) * normal0);
        bin = glm::normalize(glm::cross(tan, normal0));

        for (size_t v_i = 0; v_i < 3; v_i++)
        {
            fullVertexData[m_i + (v_i * totalVertexDataLength) + 8] = tan.x;
            fullVertexData[m_i + (v_i * totalVertexDataLength) + 8 + 1] = tan.y;
            fullVertexData[m_i + (v_i * totalVertexDataLength) + 8 + 2] = tan.z;
        }
    }


    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * fullVertexData.size(), fullVertexData.data(), GL_DYNAMIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, totalVertexDataLength * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    GLintptr normalPtr = 3 * sizeof(float);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, totalVertexDataLength * sizeof(float), (void*)normalPtr);
    glEnableVertexAttribArray(1);

    GLintptr uvPtr = 6 * sizeof(float);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, totalVertexDataLength * sizeof(float), (void*)uvPtr);
    glEnableVertexAttribArray(2);

    GLintptr tanPtr = 8 * sizeof(float);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, totalVertexDataLength * sizeof(float), (void*)tanPtr);
    glEnableVertexAttribArray(3);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}