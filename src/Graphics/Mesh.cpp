#include "Mesh.h"

using namespace gbe;
using namespace rendering;

Mesh::Mesh(std::string path) {
    //Specify the data length of a singular vertex
    int totalVertexDataLength = 11;

    //Load the obj file
    bool success = tinyobj::LoadObj(&attributes, &shapes, &material, &warning, &error, path.c_str());
    if (success == false)
        throw "Invalid Mesh";

    //Loop per triangle
    for (size_t m_i = 0; m_i < shapes[0].mesh.indices.size(); m_i += 3)
    {
        //Variables for tangent calculation
        //Store vacant data indices for tangent calculation
        std::vector<size_t> tanIndices = {};
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

            //Try insert normal data
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

            //Try insert UV data
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

            //Take note of the vacant slots for tangent data
            tanIndices.push_back(fullVertexData.size());
            fullVertexData.push_back(0);
            fullVertexData.push_back(0);
            fullVertexData.push_back(0);
        }

        //Calculate for the current triangle's tangent
        auto deltaPos1 = vertices[1] - vertices[0];
        auto deltaPos2 = vertices[2] - vertices[0];

        glm::vec2 deltaUV1 = uvs[1] - uvs[0];
        glm::vec2 deltaUV2 = uvs[2] - uvs[0];

        float r = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV1.y * deltaUV2.x);
        glm::vec3 tan = (deltaPos1 * deltaUV2.y - deltaPos2 * deltaUV1.y) * r;

        //Set the tangent for all 3 vertices
        for (size_t v_i = 0; v_i < 3; v_i++)
        {
            auto tanindex0 = tanIndices[v_i];
            fullVertexData[tanindex0] = tan.x;
            fullVertexData[tanindex0 + 1] = tan.y;
            fullVertexData[tanindex0 + 2] = tan.z;
        }
    }

    //OpenGL object loading
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * fullVertexData.size(), fullVertexData.data(), GL_DYNAMIC_DRAW);

    //Enable all vertex data inputs
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

    //Finalize object load
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
