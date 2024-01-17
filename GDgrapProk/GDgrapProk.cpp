#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>

#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"

#define _USE_MATH_DEFINES
#include <cmath>

int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    auto win_x = 700;
    auto win_y = 400;
    window = glfwCreateWindow(win_x, win_y, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }
    auto pixelPerfectGLV2F = [win_x, win_y](float x, float y) {
        glVertex2f(x / win_x, y / win_y);
        };

    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    gladLoadGL();

    std::string path = "3D/bunny.obj";
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> material;
    std::string warning, error;

    tinyobj::attrib_t attributes;

    bool success = tinyobj::LoadObj(&attributes, &shapes, &material, &warning, &error, path.c_str());
    std::vector<GLuint> mesh_indices;
    for (size_t m_i = 0; m_i < shapes[0].mesh.indices.size(); m_i++)
    {
        mesh_indices.push_back(shapes[0].mesh.indices[m_i].vertex_index);
    }

    GLuint VAO, VBO, EBO;
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

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, mesh_indices.size(), GL_UNSIGNED_INT, 0);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

    glfwTerminate();
    return 0;
}