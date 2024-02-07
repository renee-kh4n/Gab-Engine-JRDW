#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include <iostream>

#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"

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


class GLobject {
public:
    glm::vec3 pos = glm::vec3(0,0,-5);
    glm::vec3 scale = glm::vec3(1);
    glm::vec3 rot = glm::vec3(0);

    void Key_Callback(int key, int scancode, int action, int mods)
    {
        if (action == GLFW_PRESS) {
            switch (key)
            {
            case GLFW_KEY_W:
                this->pos.y += 0.1f;
                break;
            case GLFW_KEY_A:
                this->pos.x -= 0.1f;
                break;
            case GLFW_KEY_S:
                this->pos.y -= 0.1f;
                break;
            case GLFW_KEY_D:
                this->pos.x += 0.1f;
                break;
            case GLFW_KEY_Q:
                this->scale -= 0.1f;
                break;
            case GLFW_KEY_E:
                this->scale += 0.1f;
                break;
            case GLFW_KEY_UP:
                this->rot.y += 30;
                break;
            case GLFW_KEY_LEFT:
                this->rot.x -= 30;
                break;
            case GLFW_KEY_DOWN:
                this->rot.y -= 30;
                break;
            case GLFW_KEY_RIGHT:
                this->rot.x += 30;
                break;
            default:
                break;
            }
        }
    }

    GLuint VAO, VBO, EBO;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> material;
    std::string warning, error;
    tinyobj::attrib_t attributes;
    std::vector<GLuint> mesh_indices;

    GLobject(std::string path) {
        bool success = tinyobj::LoadObj(&attributes, &shapes, &material, &warning, &error, path.c_str());
        for (size_t m_i = 0; m_i < shapes[0].mesh.indices.size(); m_i++)
        {
            mesh_indices.push_back(shapes[0].mesh.indices[m_i].vertex_index);
        }

        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        glBindVertexArray(VAO);

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

void check_attached_shaders(GLuint program)
{
    GLsizei count = 0;
    GLuint shaders[] = { 0, 0, 0, 0 };
    glGetAttachedShaders(program, 4, &count, shaders);
    printf("\tnumber of shaders: %d\n", count);
    for (int i = 0; i < count; i++) printf("\tshader_id: %d\n", shaders[i]);
}

auto win_x = 700;
auto win_y = 700;
glm::mat4 proj_ortho;
glm::mat4 proj_pers;
auto curFOV = 60.0f;
void setPerspectiveFOV(float angles) {
    curFOV = angles;
    proj_pers = glm::perspective(glm::radians(angles), (float)win_y / win_x, 0.1f, 100.0f);
}

void Key_Callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (action == GLFW_PRESS) {
        switch (key)
        {
        case GLFW_KEY_Z:
            setPerspectiveFOV(curFOV - 2.0f);
            break;
        case GLFW_KEY_X:
            setPerspectiveFOV(curFOV + 2.0f);
            break;
        }
    }

    for (size_t o_i = 0; o_i < objs.size(); o_i++)
    {
        objs[o_i]->Key_Callback(key, scancode, action, mods);
    }
}

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    
    
    window = glfwCreateWindow(win_x, win_y, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }
    setPerspectiveFOV(60.0f);

    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, Key_Callback);

    gladLoadGL();

    auto vertShader = TryCompileShader("Shaders/sample.vert", GL_VERTEX_SHADER);
    auto fragShader = TryCompileShader("Shaders/sample.frag", GL_FRAGMENT_SHADER);
    
    GLuint shaderProg = glCreateProgram();
    glAttachShader(shaderProg, fragShader);
    glAttachShader(shaderProg, vertShader);
    check_attached_shaders(shaderProg);

    glLinkProgram(shaderProg);

    objs.push_back(new GLobject("3D/bunny.obj"));

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
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
            tmat = proj_pers * tmat;
            setMat("transform", tmat);
        }

        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        for (size_t o_i = 0; o_i < objs.size(); o_i++)
        {
            glBindVertexArray(objs[o_i]->VAO);
            glDrawElements(GL_TRIANGLES, objs[o_i]->mesh_indices.size(), GL_UNSIGNED_INT, 0);
        }
        /* Swap front and back buffers */
        glfwSwapBuffers(window);

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