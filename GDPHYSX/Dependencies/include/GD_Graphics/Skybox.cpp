#include "Skybox.h"

namespace gde {
    namespace rendering {
        Skybox::Skybox()
        {
            glGenVertexArrays(1, &VAO);
            glGenBuffers(1, &VBO);
            glGenBuffers(1, &EBO);

            glBindVertexArray(VAO);

            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);

            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GL_INT) * 36, &skyboxIndices, GL_STATIC_DRAW);
            glEnableVertexAttribArray(0);

            glBindBuffer(GL_ARRAY_BUFFER, 0);
            glBindVertexArray(0);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        }

        void Skybox::Render(glm::mat4 viewMat, glm::mat4 proj)
        {
            glDepthMask(GL_FALSE);
            glDepthFunc(GL_LEQUAL);
            glUseProgram(this->shader->shaderID);

            glm::mat4 sky_view = glm::mat4(1.f);
            sky_view = glm::mat4(glm::mat3(viewMat));

            glUniformMatrix4fv(glGetUniformLocation(this->shader->shaderID, "view"), 1, GL_FALSE, glm::value_ptr(sky_view));
            glUniformMatrix4fv(glGetUniformLocation(this->shader->shaderID, "projection"), 1, GL_FALSE, glm::value_ptr(proj));

            glBindVertexArray(this->VAO);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_CUBE_MAP, this->textureCubeMap->texID);
            glUniform1i(glGetUniformLocation(this->shader->shaderID, "skybox"), 0);

            glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
            glDepthMask(GL_TRUE);
            glDepthFunc(GL_LESS);
        }
    }
}