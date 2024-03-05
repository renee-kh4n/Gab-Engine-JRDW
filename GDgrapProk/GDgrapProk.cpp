#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include <iostream>

#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define _USE_MATH_DEFINES
#include <cmath>

std::string LoadTxtFile(std::string path) {
    std::fstream fsrc(path);
    std::stringstream fbuff;
    fbuff << fsrc.rdbuf();

    return fbuff.str();
}

GLuint TryCompileShader(std::string path, int compile_type) {
    auto shaderStr = LoadTxtFile(path);
    const char* shaderptr = shaderStr.c_str();

    GLuint shaderint = glCreateShader(compile_type);
    glShaderSource(shaderint, 1, &shaderptr, NULL);
    glCompileShader(shaderint);

    GLint isCompiled = 0;
    glGetShaderiv(shaderint, GL_COMPILE_STATUS, &isCompiled);

    if (isCompiled == GL_FALSE) {
        GLint maxLength = 0;
        glGetShaderiv(shaderint, GL_INFO_LOG_LENGTH, &maxLength);

        std::vector<GLchar> errorLog(maxLength);
        glGetShaderInfoLog(shaderint, maxLength, &maxLength, &errorLog[0]);

        for (size_t e_i = 0; e_i < errorLog.size(); e_i++)
        {
            std::cout << errorLog[e_i];
        }

        glDeleteShader(shaderint);
    }

    return shaderint;
}
void check_attached_shaders(GLuint program)
{
    GLsizei count = 0;
    GLuint shaders[] = { 0, 0, 0, 0 };
    glGetAttachedShaders(program, 4, &count, shaders);
    printf("\tnumber of shaders: %d\n", count);
    for (int i = 0; i < count; i++) printf("\tshader_id: %d\n", shaders[i]);
}

struct Texture {
    GLuint texID;
    stbi_uc* tex_bytes;
    int img_width;
    int img_height;
    int colorchannels;

    Texture(const char* path) {
        std::string pathstr = path;

        stbi_set_flip_vertically_on_load(true);
        tex_bytes = stbi_load(path, &img_width, &img_height, &colorchannels, 0);
        glGenTextures(1, &texID);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texID);

        if(pathstr.substr(pathstr.size() - 4, 4).compare(".jpg") == 0)
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, img_width, img_height, 0, GL_RGB, GL_UNSIGNED_BYTE, tex_bytes);
        if (pathstr.substr(pathstr.size() - 4, 4).compare(".png") == 0)
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img_width, img_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, tex_bytes);

        glGenerateMipmap(GL_TEXTURE_2D);
        stbi_image_free(tex_bytes);
    }
};

class Shader {
public:
    GLuint shaderID;

    Shader(std::string vert, std::string frag) {
        auto vertShader = TryCompileShader(vert, GL_VERTEX_SHADER);
        auto fragShader = TryCompileShader(frag, GL_FRAGMENT_SHADER);

        shaderID = glCreateProgram();
        glAttachShader(shaderID, vertShader);
        glAttachShader(shaderID, fragShader);
        check_attached_shaders(shaderID);

        glLinkProgram(shaderID);
    }
};

class GLobject {
public:
    glm::vec3 pos = glm::vec3(0,0,0);
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

            fullVertexData.push_back(attributes.texcoords[(vData.texcoord_index * 2)]);
            fullVertexData.push_back(attributes.texcoords[(vData.texcoord_index * 2) + 1]);
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
    float curFOV = 60.0f;
    void setPerspectiveFOV(float angles) {
        curFOV = angles;
        proj_pers = glm::perspective(glm::radians(angles), (float)mWindow->win_y / mWindow->win_x, 0.1f, 100.0f);
    }
    glm::vec3 cameraPos = glm::vec3(0, 0, 10.0f);
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
    auto sampleShader = new Shader("Shaders/sample.vert", "Shaders/sample.frag");
    auto swordTex = new Texture("Tex/partenza.jpg");
    auto cubeaccaTex = new Texture("Tex/cubeacca.jpg");

    glm::vec3 pLightPos = glm::vec3(0, 1, 1);
    glm::vec3 pLightColor = glm::vec3(1, 1, 1) * 3.0f;
    glm::vec3 ambientLightColor = glm::vec3(0.3, 0.2, 0.05);
    float ambientLightIntensity = 0.2f;
#pragma endregion

#pragma region Object setup
    auto swordObj = new GLobject("3D/djSword.obj");
    swordObj->shader = sampleShader;
    swordObj->texture = swordTex;
    swordObj->scale *= 0.1f;
    objs.push_back(swordObj);
#pragma endregion

#pragma region Time
    auto lastFrameT = 0.0f;
    auto currentFrameT = 0.0f;
    auto deltaTime = 0.0f;

    auto summonCD = 0.0f;
#pragma endregion

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(mWindow->window))
    {
#pragma region Programming Challenge 1 Code
        summonCD -= deltaTime;

        if (summonCD <= 0 && glfwGetKey(mWindow->window, GLFW_KEY_SPACE) == GLFW_PRESS) {
            auto swordObj = new GLobject("3D/djSword.obj");
            swordObj->shader = sampleShader;
            swordObj->texture = swordTex;
            swordObj->pos = mView.cameraPos + (mView.CamF * 5.0f);
            swordObj->scale *= 0.1f;
            objs.push_back(swordObj);

            summonCD = 3.0f;
        }
#pragma endregion

        //Update Time
        currentFrameT = glfwGetTime();
        deltaTime = currentFrameT - lastFrameT;
        lastFrameT = currentFrameT;

#pragma region Camera
        CameraControl(deltaTime * 20);

        mView.setPerspectiveFOV(60.0f);
        glm::mat4 viewMat = glm::lookAt(mView.cameraPos, mView.CamF + mView.cameraPos, mView.WorldUp);
#pragma endregion

        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        for (auto obj : objs)
        {
            glUseProgram(obj->shader->shaderID);
            GLuint tex0Address = glGetUniformLocation(obj->shader->shaderID, "tex0");
            glBindTexture(GL_TEXTURE_2D, obj->texture->texID);

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

            setVec3("pLightPos", pLightPos);
            setVec3("pLightColor", pLightColor);
            setVec3("ambientLightColor", ambientLightColor);
            setFloat("ambientLightIntensity", ambientLightIntensity);

            setVec3("cameraPos", mView.cameraPos);
            setFloat("specStrength", 0.4f);
            setFloat("specPhong", 9);

            glUniform1i(tex0Address, 0);

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