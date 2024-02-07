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
        stbi_set_flip_vertically_on_load(true);
        tex_bytes = stbi_load(path, &img_width, &img_height, &colorchannels, 0);
        glGenTextures(1, &texID);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texID);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img_width, img_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, tex_bytes);
        glGenerateMipmap(GL_TEXTURE_2D);
        stbi_image_free(tex_bytes);
    }
};
class GLobject {
public:
    glm::vec3 pos = glm::vec3(0,0,-5);
    glm::vec3 scale = glm::vec3(1);
    glm::vec3 rot = glm::vec3(0);

    GLuint VAO, VBO, EBO, VBO_UV;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> material;
    std::string warning, error;
    tinyobj::attrib_t attributes;
    std::vector<GLuint> mesh_indices;
    GLfloat UV[16]{
        0.f, 1.f,
        0.f, 0.f,
        1.f, 1.f,
        1.f, 0.f,
        1.f, 1.f,
        1.f, 0.f,
        0.f, 1.f,
        0.f, 0.f
    };

    GLobject(std::string path) {
        bool success = tinyobj::LoadObj(&attributes, &shapes, &material, &warning, &error, path.c_str());
        for (size_t m_i = 0; m_i < shapes[0].mesh.indices.size(); m_i++)
        {
            mesh_indices.push_back(shapes[0].mesh.indices[m_i].vertex_index);
        }

        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &VBO_UV);
        glGenBuffers(1, &EBO);

        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO_UV);
        glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * (sizeof(UV) / sizeof(UV[0])), &UV[0], GL_DYNAMIC_DRAW);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(2);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER,
            sizeof(GL_FLOAT) * attributes.vertices.size(),
            attributes.vertices.data(),
            GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER,
            sizeof(GLuint) * mesh_indices.size(),
            mesh_indices.data(),
            GL_STATIC_DRAW);

        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }
};
static std::vector<GLobject*> objs;

void Key_Callback(GLFWwindow* window, int key, int scancode, int action, int mods);

static struct Window {
    GLFWwindow* window;
    int win_x = 700;
    int win_y = 700;

    Window() {
        /* Create a windowed mode window and its OpenGL context */
        window = glfwCreateWindow(win_x, win_y, "Hello World", NULL, NULL);
        if (!window)
        {
            glfwTerminate();
        }

        /* Make the window's context current */
        glfwMakeContextCurrent(window);
        glfwSetKeyCallback(window, Key_Callback);
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

void Key_Callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    switch (key)
    {
    case GLFW_KEY_W:
        mView.cameraPos += mView.CamF * 0.1f;
        break;
    case GLFW_KEY_S:
        mView.cameraPos -= mView.CamF * 0.1f;
        break;
    case GLFW_KEY_A:
        mView.cameraPos -= mView.CamR() * 0.1f;
        break;
    case GLFW_KEY_D:
        mView.cameraPos += mView.CamR() * 0.1f;
        break;
    case GLFW_KEY_Q:
        mView.cameraPos -= mView.WorldUp * 0.1f;
        break;
    case GLFW_KEY_E:
        mView.cameraPos += mView.WorldUp * 0.1f;
        break;
    case GLFW_KEY_UP:
        mView.RotateCam(1, mView.CamR());
        break;
    case GLFW_KEY_DOWN:
        mView.RotateCam(-1, mView.CamR());
        break;
    case GLFW_KEY_LEFT:
        mView.RotateCam(1, mView.WorldUp);
        break;
    case GLFW_KEY_RIGHT:
        mView.RotateCam(-1, mView.WorldUp);
        break;
    case GLFW_KEY_Z:
        mView.setPerspectiveFOV(mView.curFOV - 0.1f);
        break;
    case GLFW_KEY_C:
        mView.setPerspectiveFOV(mView.curFOV + 0.1f);
        break;
    }
}

int main(void)
{
    /* Initialize the library */
    if (!glfwInit())
        return -1;
    mWindow = new Window();


#pragma region Shader
    gladLoadGL();

    auto vertShader = TryCompileShader("Shaders/sample.vert", GL_VERTEX_SHADER);
    auto fragShader = TryCompileShader("Shaders/sample.frag", GL_FRAGMENT_SHADER);
    
    GLuint shaderProg = glCreateProgram();
    glAttachShader(shaderProg, fragShader);
    glAttachShader(shaderProg, vertShader);
    check_attached_shaders(shaderProg);

    glLinkProgram(shaderProg);
#pragma endregion

    auto newObj = new GLobject("3D/myCube.obj");
    auto newTex = new Texture("Tex/ayaya.png");
    objs.push_back(newObj);

    glEnable(GL_DEPTH_TEST);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(mWindow->window))
    {

#pragma region Camera
        mView.setPerspectiveFOV(60.0f);
        glm::mat4 viewMat = glm::lookAt(mView.cameraPos, mView.CamF + mView.cameraPos, mView.WorldUp);
#pragma endregion

        glUseProgram(shaderProg); //Use shader

        auto setFloat = [shaderProg](const char* id, float value) {
            unsigned int xLoc = glGetUniformLocation(shaderProg, id);
            glUniform1f(xLoc, value);
        };
        auto setMat = [shaderProg](const char* id, glm::mat4 value) {
            unsigned int xLoc = glGetUniformLocation(shaderProg, id);
            glUniformMatrix4fv(xLoc, 1, GL_FALSE, glm::value_ptr(value));
        };
        
        setFloat("r", 0.5);

        for (size_t o_i = 0; o_i < objs.size(); o_i++)
        {
            glm::mat4 tmat = glm::mat4(1.0f);
            tmat = glm::translate(tmat, objs[o_i]->pos);
            tmat = glm::scale(tmat, objs[o_i]->scale);
            tmat = glm::rotate(tmat, glm::radians(objs[o_i]->rot.x), glm::vec3(0, 1, 0));
            tmat = glm::rotate(tmat, glm::radians(objs[o_i]->rot.y), glm::vec3(1, 0, 0));
            tmat = mView.proj_pers * viewMat * tmat;
            setMat("transform", tmat);
        }

        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        for (size_t o_i = 0; o_i < objs.size(); o_i++)
        {
            GLuint tex0Address = glGetUniformLocation(shaderProg, "tex0");
            glBindTexture(GL_TEXTURE_2D, newTex->texID);
            glUniform1i(tex0Address, 0);

            glBindVertexArray(objs[o_i]->VAO);
            glDrawElements(GL_TRIANGLES, objs[o_i]->mesh_indices.size(), GL_UNSIGNED_INT, 0);
        }
        /* Swap front and back buffers */
        glfwSwapBuffers(mWindow->window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    for (size_t o_i = 0; o_i < objs.size(); o_i++)
    {
        glDeleteVertexArrays(1, &objs[o_i]->VAO);
        glDeleteBuffers(1, &objs[o_i]->VBO);
        glDeleteBuffers(1, &objs[o_i]->EBO);
    }
    
    glfwTerminate();
    return 0;
}