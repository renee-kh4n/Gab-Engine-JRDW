#include "GDutil.h"

#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "GDShader.cpp"
#include "GDTexture.cpp"
#include "GDObject.cpp"

class GLskybox {
public:
    Shader* shader;
    TextureCubeMap* textureCubeMap;

    GLuint VAO, VBO, EBO;

    float skyboxVertices[36]{
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
    unsigned int skyboxIndices[36]{
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

    GLskybox() {
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
};

class GLobject {
public:
    glm::vec3 pos = glm::vec3(0, 0, 0);
    glm::vec3 scale = glm::vec3(1);
    glm::vec3 rot = glm::vec3(0);

    Shader* shader;
    Texture* texture;

    GLuint VAO, VBO;
    tinyobj::attrib_t attributes;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> material;
    std::string warning, error;

    std::vector<GLfloat> fullVertexData;

    GLobject(std::string path) {
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
};

static std::vector<GLobject*> objs;

static struct Window {
    GLFWwindow* window;
    int win_x = 700;
    int win_y = 700;

    Window() {
        /* Create a windowed mode window and its OpenGL context */
        window = glfwCreateWindow(win_x, win_y, "Sword summoner", NULL, NULL);
        if (!window)
        {
            glfwTerminate();
        }

        /* Make the window's context current */
        glfwMakeContextCurrent(window);
    }
}*mWindow;

static struct View {
    glm::mat4 proj_ortho;
    glm::mat4 proj_pers;
    float curFOV = 90.0f;
    void setPerspectiveFOV(float angles) {
        curFOV = angles;
        proj_pers = glm::perspective(glm::radians(angles), (float)mWindow->win_y / mWindow->win_x, 0.1f, 100.0f);
    }
    glm::vec3 cameraPos = glm::vec3(0, 0, 12.0f);
    glm::vec3 WorldUp = glm::vec3(0, 1, 0);

    glm::vec3 CamF = glm::vec3(0, 0, -1);
    glm::vec3 CamR() {
        return glm::cross(CamF, WorldUp);
    }

    void RotateCam(float degree, glm::vec3 axis) {
        CamF = glm::vec3(glm::rotate(glm::translate(glm::mat4(1), mView.CamF), glm::radians(degree), axis) * glm::vec4(mView.CamF, 0));
    }
} mView;

void CameraControl(float speed)
{
    if (glfwGetKey(mWindow->window, GLFW_KEY_W) == GLFW_PRESS)
        mView.cameraPos += mView.CamF * (0.5f * speed);
    if (glfwGetKey(mWindow->window, GLFW_KEY_S) == GLFW_PRESS)
        mView.cameraPos -= mView.CamF * (0.5f * speed);
    if (glfwGetKey(mWindow->window, GLFW_KEY_A) == GLFW_PRESS)
        mView.cameraPos -= mView.CamR() * (0.5f * speed);
    if (glfwGetKey(mWindow->window, GLFW_KEY_D) == GLFW_PRESS)
        mView.cameraPos += mView.CamR() * (0.5f * speed);
    if (glfwGetKey(mWindow->window, GLFW_KEY_Q) == GLFW_PRESS)
        mView.cameraPos -= mView.WorldUp * 0.5f * speed;
    if (glfwGetKey(mWindow->window, GLFW_KEY_E) == GLFW_PRESS)
        mView.cameraPos += mView.WorldUp * 0.5f * speed;
    if (glfwGetKey(mWindow->window, GLFW_KEY_UP) == GLFW_PRESS)
        mView.RotateCam(2 * speed, mView.CamR());
    if (glfwGetKey(mWindow->window, GLFW_KEY_DOWN) == GLFW_PRESS)
        mView.RotateCam(-2 * speed, mView.CamR());
    if (glfwGetKey(mWindow->window, GLFW_KEY_LEFT) == GLFW_PRESS)
        mView.RotateCam(2 * speed, mView.WorldUp);
    if (glfwGetKey(mWindow->window, GLFW_KEY_RIGHT) == GLFW_PRESS)
        mView.RotateCam(-2 * speed, mView.WorldUp);
    if (glfwGetKey(mWindow->window, GLFW_KEY_Z) == GLFW_PRESS)
        mView.setPerspectiveFOV(mView.curFOV - (0.1f * speed));
    if (glfwGetKey(mWindow->window, GLFW_KEY_C) == GLFW_PRESS)
        mView.setPerspectiveFOV(mView.curFOV + (0.1f * speed));
}

int main(void)
{
    /* Initialize the library */
    if (!glfwInit())
        return -1;
    mWindow = new Window();

    gladLoadGL();
    glEnable(GL_DEPTH_TEST);

#pragma region Other object setup
    //Shaders
    auto litShader = new Shader("Shaders/lit.vert", "Shaders/lit.frag");

    //Textures
    auto swordTex = new Texture("Tex/partenza.jpg");
    auto cubeaccaTex = new Texture("Tex/cubeacca.jpg");

    //Light setup
    glm::vec3 pLightPos = glm::vec3(0, 1, 1);
    glm::vec3 pLightColor = glm::vec3(1, 1, 1) * 0.0f;
    glm::vec3 dLightDir = glm::vec3(-1, -1, 0);
    glm::vec3 dLightColor = glm::vec3(0, 1, 0) * 1.0f;
    glm::vec3 ambientLightColor = glm::vec3(0.3, 0.2, 0.05);
    float ambientLightIntensity = 0.5f;
#pragma endregion

#pragma region Object setup
    //Skybox setup
    auto skyboxObj = new GLskybox();
    skyboxObj->shader = new Shader("Shaders/skybox.vert", "Shaders/skybox.frag");
    skyboxObj->textureCubeMap = new TextureCubeMap("Tex/Skybox/rainbow");
    
    //Sword and cube setup
    auto swordObj = new GLobject("3D/quiz.obj");
    swordObj->shader = litShader;
    swordObj->texture = swordTex;
    swordObj->scale *= 0.1f;
    objs.push_back(swordObj);
#pragma endregion

#pragma region Time
    auto lastFrameT = 0.0f;
    auto currentFrameT = 0.0f;
    auto deltaTime = 0.0f;
#pragma endregion

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(mWindow->window))
    {
        //Update Time
        currentFrameT = glfwGetTime();
        deltaTime = currentFrameT - lastFrameT;
        lastFrameT = currentFrameT;

#pragma region Quiz 2 Code
        for (auto obj : objs)
        {
            obj->rot.x += deltaTime * 180;
        }
#pragma endregion

#pragma region Camera
        CameraControl(deltaTime * 20);

        mView.setPerspectiveFOV(60.0f);
        glm::mat4 viewMat = glm::lookAt(mView.cameraPos, mView.CamF + mView.cameraPos, mView.WorldUp);
#pragma endregion

        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //skybox
        glDepthMask(GL_FALSE);
        glDepthFunc(GL_LEQUAL);
        glUseProgram(skyboxObj->shader->shaderID);

        glm::mat4 sky_view = glm::mat4(1.f);
        sky_view = glm::mat4(glm::mat3(viewMat));

        glUniformMatrix4fv(glGetUniformLocation(skyboxObj->shader->shaderID, "view"), 1, GL_FALSE, glm::value_ptr(sky_view));
        glUniformMatrix4fv(glGetUniformLocation(skyboxObj->shader->shaderID, "projection"), 1, GL_FALSE, glm::value_ptr(mView.proj_pers));

        //something wrong here
        glBindVertexArray(skyboxObj->VAO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxObj->textureCubeMap->texID);
        glUniform1i(glGetUniformLocation(skyboxObj->shader->shaderID, "skybox"), 0);

        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
        glDepthMask(GL_TRUE);
        glDepthFunc(GL_LESS);

        for (auto obj : objs)
        {
            glUseProgram(obj->shader->shaderID);

            glActiveTexture(GL_TEXTURE1);
            GLuint tex0Address = glGetUniformLocation(obj->shader->shaderID, "tex0");
            glBindTexture(GL_TEXTURE_2D, obj->texture->texID);
            glUniform1i(tex0Address, 1);

            glActiveTexture(GL_TEXTURE0);
            GLuint skyboxAddress = glGetUniformLocation(obj->shader->shaderID, "skybox");
            glBindTexture(GL_TEXTURE_2D, skyboxObj->textureCubeMap->texID);
            glUniform1i(skyboxAddress, 0);

            auto setFloat = [obj](const char* id, float value) {
                unsigned int xLoc = glGetUniformLocation(obj->shader->shaderID, id);
                glUniform1f(xLoc, value);
            };
            auto setMat = [obj](const char* id, glm::mat4 value) {
                unsigned int xLoc = glGetUniformLocation(obj->shader->shaderID, id);
                glUniformMatrix4fv(xLoc, 1, GL_FALSE, glm::value_ptr(value));
            };
            auto setVec3 = [obj](const char* id, glm::vec3 value) {
                unsigned int xLoc = glGetUniformLocation(obj->shader->shaderID, id);
                glUniform3fv(xLoc, 1, glm::value_ptr(value));
            };

            glm::mat4 tmat = glm::mat4(1.0f);
            tmat = glm::translate(tmat, obj->pos);
            tmat = glm::scale(tmat, obj->scale);
            tmat = glm::rotate(tmat, glm::radians(obj->rot.x), glm::vec3(0, 1, 0));
            tmat = glm::rotate(tmat, glm::radians(obj->rot.y), glm::vec3(1, 0, 0));
            glm::mat4 tmat_VM = tmat;
            glm::mat4 tmat_PVM = mView.proj_pers * viewMat * tmat;
            setMat("transform_model", tmat);
            setMat("transform_projection", tmat_PVM);

            setVec3("dLightDir", dLightDir);
            setVec3("dLightColor", dLightColor);
            setVec3("pLightPos", pLightPos);
            setVec3("pLightColor", pLightColor);
            setVec3("ambientLightColor", ambientLightColor);
            setFloat("ambientLightIntensity", ambientLightIntensity);

            setVec3("cameraPos", mView.cameraPos);
            setFloat("specStrength", 0.9f);
            setFloat("specPhong", 12);

            glBindVertexArray(obj->VAO);
            glDrawArrays(GL_TRIANGLES, 0, obj->fullVertexData.size() / 8);
        }

        glfwSwapBuffers(mWindow->window);
        /* Poll for and process events */
        glfwPollEvents();
    }

    //Clean up
    for (size_t o_i = 0; o_i < objs.size(); o_i++)
    {
        glDeleteVertexArrays(1, &objs[o_i]->VAO);
        glDeleteBuffers(1, &objs[o_i]->VBO);
    }
    
    glfwTerminate();
    return 0;
}