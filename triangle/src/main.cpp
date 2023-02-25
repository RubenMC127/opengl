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

unsigned int createProgram() {
    const char* vertexShaderSource = "#version 450 core\n"
        "layout (location = 0) in vec3 vertexPosition;\n"
        "layout (location = 1) in vec3 vertexColor;\n"
        "layout (location = 0) out vec3 fragmentColor;\n"
        "void main()\n"
        "{\n"
        "   gl_Position = vec4(vertexPosition, 1.0);\n"
        "   fragmentColor = vertexColor;\n"
        "}\0";
    // NUL char, basically how you end a char*

    const char* fragmentShaderSource = "#version 450 core\n"
        "layout (location = 0) in vec3 fragmentColor;\n"
        "out vec4 finalColor;\n"
        "void main()\n"
        "{\n"
        "   finalColor = vec4(fragmentColor, 1.0);\n"
        "}\0";

    // returns an int pointing to OpenGL shader structure
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    int success;
    char errorLog[1024];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertexShader, 1024, NULL, errorLog);
        std::cout << "Vertex Shader compilation error: " << errorLog << '\n';
    }

    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragmentShader, 1024, NULL, errorLog);
        std::cout << "Fragment Shader compilation error: " << errorLog << '\n';
    }

    // OpenGL calls the shader pipeline a 'program' 
    unsigned int program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(program, 1024, NULL, errorLog);
        std::cout << "Program linking error: " << errorLog << '\n';
    }

    // Since it's linked we don't neet to keep them on its own
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return program;
}

int main(void)
{
    GLFWwindow* window = initialize(640, 480);

    if (!window) {
        glfwTerminate();
        return -1;
    }

    unsigned int program = createProgram();
    glUseProgram(program);

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

    glDeleteProgram(program);
    glfwTerminate();
    return 0;
}