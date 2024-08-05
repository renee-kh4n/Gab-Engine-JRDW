#pragma once

#include "util.h"
#include "Shader.h"
#include "Texture.h"

namespace gde {
    namespace rendering {
        class Skybox {
        public:
            Shader* shader;
            TextureCubeMap* textureCubeMap;

            GLuint VAO, VBO, EBO;

            const float skyboxVertices[36]{
                -1.f, -1.f, 1.f, //0
                1.f, -1.f, 1.f,  //1
                1.f, -1.f, -1.f, //2
                -1.f, -1.f, -1.f,//3
                -1.f, 1.f, 1.f,  //4
                1.f, 1.f, 1.f,   //5
                1.f, 1.f, -1.f,  //6
                -1.f, 1.f, -1.f  //7
            };

            //Skybox Indices
            const unsigned int skyboxIndices[36]{
                1,2,6,
                6,5,1,

                0,4,7,
                7,3,0,

                4,5,6,
                6,7,4,

                0,3,2,
                2,1,0,

                0,1,5,
                5,4,0,

                3,7,6,
                6,2,3
            };

            Skybox();
            void Render(glm::mat4 viewMat, glm::mat4 proj);
        };
    }
}