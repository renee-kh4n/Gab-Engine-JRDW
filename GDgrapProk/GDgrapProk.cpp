#include <GLFW/glfw3.h>

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

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        glBegin(GL_POLYGON);
        
        auto vertexCount = 5;
        auto shapeSize = (win_x < win_y) ? win_x : win_y;
        auto offsetRad = M_PI * 0.5f;

        for (size_t v_i = 0; v_i < vertexCount; v_i++)
        {
            auto t = (float)v_i / vertexCount;
            auto rad = (t * M_PI * 2) + offsetRad;
            auto v_x = cosf(rad) * shapeSize;
            auto v_y = sinf(rad) * shapeSize;

            pixelPerfectGLV2F(v_x, v_y);
        }

        glEnd();

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}