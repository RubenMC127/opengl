#include <iostream>
#include "glad.h"
#include <GLFW/glfw3.h>

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }
}

GLFWwindow* initialize(int width, int height)
{
    /* Initialize the library */
    if (!glfwInit())
        return NULL;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* Create a windowed mode window and its OpenGL context */
    GLFWwindow* window = glfwCreateWindow(640, 480, "Window!", NULL, NULL);
    if (!window)
    {
        return NULL;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    /* Glad */
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Couldn't load opengl" << std::endl;
        return NULL;
    }

    glClearColor(0.25f, 0.5f, 0.75f, 1.0f);
    return window;
}

int main(void)
{
    GLFWwindow* window = initialize(640, 480);

    if (!window) {
        glfwTerminate();
        return -1;
    }

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        /* Handles key input events */
        processInput(window);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}