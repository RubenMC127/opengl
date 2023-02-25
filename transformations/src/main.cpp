#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <glm/gtc/matrix_transform.hpp>

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
    GLFWwindow* window = glfwCreateWindow(width, height, "Window!", NULL, NULL);
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

    glViewport(0, 0, width, height);

    return window;
}

unsigned int createProgram() {
    const char* vertexShaderSource = "#version 450 core\n"
        "layout (location = 0) in vec3 vertex;\n"
        "layout (location = 1) in vec2 texCoords;\n"
        "layout (location = 0) out vec2 fTexCoords;\n"
        "uniform mat4 model;\n"
        "uniform mat4 projection;\n"
        "void main()\n"
        "{\n"
        "   gl_Position = projection * model * vec4(vertex, 1.0);\n"
        "   fTexCoords = vec2(texCoords.x, 1.0-texCoords.y);\n" // for some reason they are loaded y-inverted
        "}\0";
    // NUL char, basically how you end a char*

    const char* fragmentShaderSource = "#version 450 core\n"
        "layout (location = 0) in vec2 fTexCoords;\n"
        "out vec4 fragColor;\n"
        "uniform sampler2D basicTexture;\n"
        "void main()\n"
        "{\n"
        "   fragColor = texture(basicTexture, fTexCoords);\n"
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

vector<float> makeTriangle() {
    // x,y,z,s,t
    std::vector<float> vertices = { {
        -0.25f, -0.25f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
        0.25f, -0.25f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
        0.0f, 0.25f, 0.0f, 0.0f, 1.0f, 0.0f, 0.5f, 1.0f,
    } };
    return vertices;
}

vector<float> makeCube() {
    //Make Cube
	//x,y,z,s,t
	std::vector<float> vertices = { {
			-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, // bottom
			 0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
			 0.5f,  0.5f, -0.5f, 1.0f, 1.0f,

			 0.5f,  0.5f, -0.5f, 1.0f, 1,
			-0.5f,  0.5f, -0.5f, 0.0f, 1,
			-0.5f, -0.5f, -0.5f, 0.0f, 0,

			-0.5f, -0.5f,  0.5f, 0.0f, 0.0f, //top
			 0.5f, -0.5f,  0.5f, 1.0f, 0.0f,
			 0.5f,  0.5f,  0.5f, 1.0f, 1.0f,

			 0.5f,  0.5f,  0.5f, 1.0f, 1.0f,
			-0.5f,  0.5f,  0.5f, 0.0f, 1.0f,
			-0.5f, -0.5f,  0.5f, 0.0f, 0.0f,

			-0.5f,  0.5f,  0.5f, 1.0f, 0.0f, //left
			-0.5f,  0.5f, -0.5f, 1.0f, 1.0f,
			-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,

			-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
			-0.5f, -0.5f,  0.5f, 0.0f, 0.0f,
			-0.5f,  0.5f,  0.5f, 1.0f, 0.0f,

			 0.5f,  0.5f,  0.5f, 1.0f, 0.0f, //right
			 0.5f,  0.5f, -0.5f, 1.0f, 1.0f,
			 0.5f, -0.5f, -0.5f, 0.0f, 1.0f,

			 0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
			 0.5f, -0.5f,  0.5f, 0.0f, 0.0f,
			 0.5f,  0.5f,  0.5f, 1.0f, 0.0f,

			-0.5f, -0.5f, -0.5f, 0.0f, 1.0f, //back
			 0.5f, -0.5f, -0.5f, 1.0f, 1.0f,
			 0.5f, -0.5f,  0.5f, 1.0f, 0.0f,

			 0.5f, -0.5f,  0.5f, 1.0f, 0.0f,
			-0.5f, -0.5f,  0.5f, 0.0f, 0.0f,
			-0.5f, -0.5f, -0.5f, 0.0f, 1.0f,

			-0.5f,  0.5f, -0.5f, 0.0f, 1.0f, //front
			 0.5f,  0.5f, -0.5f, 1.0f, 1.0f,
			 0.5f,  0.5f,  0.5f, 1.0f, 0.0f,

			 0.5f,  0.5f,  0.5f, 1.0f, 0.0f,
			-0.5f,  0.5f,  0.5f, 0.0f, 0.0f,
			-0.5f,  0.5f, -0.5f, 0.0f, 1.0f
	} };
    return vertices;
}

int main(void)
{
    int width = 640;
    int height = 480;
    float aspectRatio = (float) width / (float) height;
    GLFWwindow* window = initialize(width, height);

    if (!window) {
        glfwTerminate();
        return -1;
    }

    unsigned int program = createProgram();
    glUseProgram(program);

    // data format
    
    std::vector<float> vertices = makeCube();
    int vertexCount = vertices.size()/5;

    unsigned int VBO;
    glCreateBuffers(1, &VBO);

    unsigned int VAO;
    glCreateVertexArrays(1, &VAO);
    // Binding VBO to index 0 of VAO
    glVertexArrayVertexBuffer(VAO, 0, VBO, 0, 5 * sizeof(float));
    // Assigning vertices array to VBO
    glNamedBufferStorage(VBO, vertices.size() * sizeof(float), vertices.data(), GL_DYNAMIC_STORAGE_BIT);

    // Enable and defining VAO[0] attributes
    // Defining Position Attribute at index 0 of attrib
    glEnableVertexArrayAttrib(VAO, 0);
    glVertexArrayAttribFormat(VAO, 0, 3, GL_FLOAT, GL_FALSE, 0);
    glVertexArrayAttribBinding(VAO, 0, 0);

    // Defining texture coordinates Attribute at index 2 of 
    glEnableVertexArrayAttrib(VAO, 1);
    glVertexArrayAttribFormat(VAO, 1, 2, GL_FLOAT, GL_FALSE, 3 * sizeof(float)); // 3 last floats of the vertex
    glVertexArrayAttribBinding(VAO, 1, 0);

    // texture
    glUniform1i(glGetUniformLocation(program, "basicTexture"), 0);

    // load image
    int texWidth, texHeight, nrChannels;
    unsigned char* data = stbi_load("textures/alpaca.jpg", &texWidth, &texHeight, &nrChannels, STBI_rgb_alpha);
    
    unsigned int texture;
    glCreateTextures(GL_TEXTURE_2D, 1, &texture);
    glTextureStorage2D(texture, 1, GL_RGBA8, texWidth, texHeight);
    glTextureSubImage2D(texture, 0, 0, 0, texWidth, texHeight, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glTextureParameteri(texture, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTextureParameteri(texture, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTextureParameteri(texture, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTextureParameteri(texture, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    stbi_image_free(data);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Handles key input events */
        processInput(window);
        /* Poll for and process events */
        glfwPollEvents();
        /* Render here */
        glClearColor(0.25f, 0.5f, 0.75f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        // Binding Shader Program
        glUseProgram(program);
        // texture
        glBindTextureUnit(0, texture);

        // Binding VAO & Drawing!
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, vertexCount);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);
    }

    //glDeleteTextures(1, &texture);
    glDeleteBuffers(1, &VBO);
    glDeleteVertexArrays(1, &VAO);
    glDeleteProgram(program);
    glfwTerminate();
    return 0;
}