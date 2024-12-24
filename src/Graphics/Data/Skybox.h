#pragma once
#include "Math/gbe_math.h"
#include <glad/glad.h>

namespace gbe {
    namespace gfx {
        class Skybox {
        public:

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
            void Render(Matrix4 viewMat, Matrix4 proj);
        };
    }
}